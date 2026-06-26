#include "business/AiClient.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

namespace {
// 请求超时时间（毫秒）
constexpr int kRequestTimeoutMs = 30000;
// System Prompt：设定 AI 角色身份
const QString kSystemPrompt =
    QStringLiteral("你是一个运行在用户桌面上的AI助手，名字叫'派大星的房子'，但是你底层调用的是deepseek的模型"
                   "你的回答要公正客观，不要被用户的情绪影响"
                   "你能分析设备数据、诊断故障、回答技术问题"
                   "回复控制在2-4句话，不要长篇大论");
}

// ============================================================
// 构造函数
// ============================================================
AiClient::AiClient(QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    // 初始化 SQLite
    QString dbPath = QCoreApplication::applicationDirPath() + "/chat_history.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning() << "打开数据库失败:" << m_db.lastError().text();
        return;
    }

    // 建表（第一次运行时创建）
    QSqlQuery query(m_db);
    query.exec(
        "CREATE TABLE IF NOT EXISTS chat_log ("
        "  id      INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  role    TEXT NOT NULL,"     // "user" 或 "assistant"
        "  content TEXT NOT NULL"
        ")"
        );

    // 加载上次的聊天记录到内存
    loadHistory();
}



// 设置 API Key
void AiClient::setApiKey(const QString& apiKey)
{
    m_apiKey = apiKey;
}


// 构建请求体 JSON
// OpenAI 兼容格式：
// {
//   "model": "deepseek-v4 flash",
//   "messages": [
//     {"role": "system", "content": "..."},
//     {"role": "user",   "content": "..."}
//   ],
//   "stream": false
// }
// ============================================================
QString AiClient::buildRequestBody(const QString& userMessage) const
{
    QJsonArray messages;


    QJsonObject sysMsg;
    sysMsg["role"]    = "system";
    sysMsg["content"] = kSystemPrompt;
    messages.append(sysMsg);

    // 中间：历史对话（让 AI 记住上下文）
    for (const auto& [role, content] : m_chatHistory) {
        QJsonObject histMsg;
        histMsg["role"]    = role;
        histMsg["content"] = content;
        messages.append(histMsg);
    }

    // 最后：当前用户消息
    QJsonObject userMsg;
    userMsg["role"]    = "user";
    userMsg["content"] = userMessage;
    messages.append(userMsg);

    QJsonObject body;
    body["model"]    = kModelName;
    body["messages"] = messages;
    body["stream"]   = false;

    return QString::fromUtf8(QJsonDocument(body).toJson(QJsonDocument::Compact));
}


// ============================================================
// 发送消息
// ============================================================
void AiClient::sendMessage(const QString& userMessage)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API Key 未设置！");
        return;
    }

    // 保存用户消息到内存
    m_chatHistory.append({"user", userMessage});
    // 保存用户消息到数据库
    saveTurn("user", userMessage);


    QNetworkRequest request(QUrl(QString::fromLatin1(kApiEndpoint)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",
                         QString("Bearer %1").arg(m_apiKey).toUtf8());
    request.setTransferTimeout(kRequestTimeoutMs);

    QNetworkReply* reply = m_networkManager->post(
        request, buildRequestBody(userMessage).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray choices = root["choices"].toArray();
        if (choices.isEmpty()) {
            emit errorOccurred("API 响应异常");
            return;
        }

        QString aiMessage = choices[0]
                                .toObject()["message"]
                                .toObject()["content"]
                                .toString();

        // 保存 AI 回复到历史
        m_chatHistory.append({"assistant", aiMessage});
        saveTurn("assistant", aiMessage);   // 同时存入数据库
        // 限制历史长度，防止 token 超限
        while (m_chatHistory.size() > kMaxHistoryTurns * 2) {
            m_chatHistory.removeFirst();
        }

        emit replyReceived(aiMessage);
    });
}
// ============================================================
// 从数据库加载历史聊天记录
// ============================================================
void AiClient::loadHistory()
{
    QSqlQuery query(m_db);
    query.exec("SELECT role, content FROM chat_log ORDER BY id ASC");

    while (query.next()) {
        QString role    = query.value(0).toString();
        QString content = query.value(1).toString();
        m_chatHistory.append({role, content});
    }
}

// ============================================================
// 保存一条对话到数据库
// ============================================================
void AiClient::saveTurn(const QString& role, const QString& content)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO chat_log (role, content) VALUES (?, ?)");
    query.addBindValue(role);
    query.addBindValue(content);
    query.exec();
}

