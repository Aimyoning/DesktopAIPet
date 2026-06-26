#ifndef AICLIENT_H
#define AICLIENT_H

#include <QObject>
#include <QSqlDatabase>  // SQLite 数据库连接
#include <QList>
#include <QPair>

// 前向声明
class QNetworkAccessManager;
class QNetworkReply;

/**
 * AiClient —— DeepSeek API 客户端
 *
 * 职责：封装 HTTP 调用，发送用户消息，流式接收 AI 回复
 * 不包含 UI 代码，纯业务逻辑
 */
class AiClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AiClient)

public:
    explicit AiClient(QObject* parent = nullptr);
    ~AiClient() override = default;

    /**
     * 发送消息到 AI
     * @param userMessage 用户输入的文本
     *
     * 调用后异步等待回复，收到回复时发射 replyReceived 信号
     */
    void sendMessage(const QString& userMessage);

    /**
     * 设置 API 配置
     * @param apiKey 你的 DeepSeek API Key
     */
    void setApiKey(const QString& apiKey);
    /// 获取已加载的聊天历史（供 DesktopAIPet 回放到界面）
    const QList<QPair<QString, QString>>& chatHistory() const { return m_chatHistory; }
signals:
    /// AI 回复已接收（完整文本）
    void replyReceived(const QString& fullReply);

    /// 网络请求出错
    void errorOccurred(const QString& errorMessage);

private:
    /// 构建请求体 JSON 字符串
    QString buildRequestBody(const QString& userMessage) const;

    QNetworkAccessManager* m_networkManager;  // HTTP 请求管理器
    QString m_apiKey;                          // API 密钥

    // 常量
    static constexpr const char* kApiEndpoint =
        "https://api.deepseek.com/v1/chat/completions";
    static constexpr const char* kModelName = "deepseek-chat";
    // 对话历史（保留上下文，AI 能记住之前说了什么）
    QList<QPair<QString, QString>> m_chatHistory;  // (role, content)
    static constexpr int kMaxHistoryTurns = 10;     // 最多保留 10 轮对话
    // ======== 聊天记录持久化 ========
    void loadHistory();       // 启动时从数据库加载历史
    void saveTurn(const QString& role, const QString& content);  // 保存一条

    QSqlDatabase m_db;        // 数据库连接



};

#endif // AICLIENT_H
