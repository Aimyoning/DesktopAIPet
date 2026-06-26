#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>

/**
 * TcpAgent —— 基于 TCP 的设备代理
 *
 * 与 DeviceAgent 区别：数据不通过 emit 信号发送，
 * 而是通过 QTcpSocket 以 JSON 格式上报给服务器。
 * 自带断线重连机制。
 */
class TcpAgent : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpAgent)

public:
    /// @param name       设备名称
    /// @param intervalMs 采集间隔（毫秒）
    explicit TcpAgent(const QString& name, int intervalMs,
                      QObject* parent = nullptr);
    ~TcpAgent() override = default;

    /// 设置要连接的服务器地址
    void connectToServer(const QString& host = "127.0.0.1",
                         quint16 port = 12345);

    const QString& name() const { return m_name; }

public slots:
    void start();
    void stop();

signals:
    /// 状态变化信号（断线→连接中→已连接→已断开）
    void statusChanged(const QString& name, const QString& status);

protected:
    /// 子类重写，返回数据（与 DeviceAgent 的 generateData 相同接口）
    virtual QVariantMap generateData() = 0;

private slots:
    void onConnected();
    void onDisconnected();
    void onTimerTick();

private:
    void sendData(const QVariantMap& data);
    void tryReconnect();

    QString     m_name;
    QTimer*     m_timer;
    int         m_intervalMs;
    QTcpSocket* m_socket;
    QString     m_host;
    quint16     m_port;

    // 常量
    static constexpr int kReconnectDelayMs = 3000;   // 重连间隔（3秒）
    static constexpr int kMaxRetries      = 0;       // 0 = 无限重试
    int m_retryCount = 0;
};
