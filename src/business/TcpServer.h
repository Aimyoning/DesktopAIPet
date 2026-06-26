#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

/**
 * TcpServer —— Agent 数据汇聚点
 *
 * 监听本地端口，接收所有 Agent 的 TCP JSON 数据，
 * 解析后发射 agentDataReceived 信号给主窗口更新 UI。
 */
class TcpServer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpServer)

public:
    /// @param port 监听端口，默认 12345
    explicit TcpServer(quint16 port = 12345, QObject* parent = nullptr);

    /// 启动监听
    bool start();

    /// 停止监听
    void stop();

signals:
    /// 收到 Agent 数据 → 转发给 UI 层
    void agentDataReceived(const QString& name, const QString& value,
                           const QString& status, const QString& color);

    /// Agent 连接/断开状态
    void agentStatusChanged(const QString& name, const QString& status);

    /// 服务器自身状态
    void serverStatus(const QString& msg);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    void parseAndEmit(const QByteArray& data);

    QTcpServer* m_server;
    QList<QTcpSocket*> m_clients;   // 已连接的 Agent socket
    QByteArray m_buffer;             // 粘包缓冲区
};
