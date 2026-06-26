#include "TcpServer.h"
#include <QJsonDocument>
#include <QJsonObject>

TcpServer::TcpServer(quint16 port, QObject* parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection,
            this, &TcpServer::onNewConnection);
}

bool TcpServer::start()
{
    if (!m_server->listen(QHostAddress::LocalHost, 12345)) {
        emit serverStatus("启动失败: " + m_server->errorString());
        return false;
    }
    emit serverStatus("监听端口 12345, 等待 Agent 连接...");
    return true;
}

void TcpServer::stop()
{
    m_server->close();
    for (auto* client : m_clients) {
        client->disconnectFromHost();
    }
}

// ===== 新连接 =====
void TcpServer::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket* client = m_server->nextPendingConnection();
        m_clients.append(client);

        connect(client, &QTcpSocket::readyRead,
                this, &TcpServer::onReadyRead);
        connect(client, &QTcpSocket::disconnected,
                this, &TcpServer::onClientDisconnected);

        emit serverStatus("Agent 已连接: " + client->peerAddress().toString());
    }
}

// ===== 收到数据 =====
void TcpServer::onReadyRead()
{
    auto* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    m_buffer.append(client->readAll());

    // 按换行切分（每条 JSON 一行）
    while (true) {
        int idx = m_buffer.indexOf('\n');
        if (idx < 0) break;
        QByteArray line = m_buffer.left(idx).trimmed();
        m_buffer.remove(0, idx + 1);
        if (!line.isEmpty()) parseAndEmit(line);
    }
}

void TcpServer::parseAndEmit(const QByteArray& data)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) return;

    QJsonObject obj = doc.object();
    emit agentDataReceived(
        obj["name"].toString(),
        obj["value"].toString(),
        obj["status"].toString(),
        obj["color"].toString()
        );
}

void TcpServer::onClientDisconnected()
{
    auto* client = qobject_cast<QTcpSocket*>(sender());
    if (client) m_clients.removeAll(client);
}
