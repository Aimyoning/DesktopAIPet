#include "TcpAgent.h"

namespace {
constexpr int kDefaultIntervalMs = 2000;
}

TcpAgent::TcpAgent(const QString& name, int intervalMs, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_timer(new QTimer(this))
    , m_intervalMs(intervalMs > 0 ? intervalMs : kDefaultIntervalMs)
    , m_socket(new QTcpSocket(this))
    , m_host("127.0.0.1")
    , m_port(12345)
{
    // 定时器触发 → 采集数据 → TCP 发送
    connect(m_timer, &QTimer::timeout, this, &TcpAgent::onTimerTick);

    // TCP 连接状态变化
    connect(m_socket, &QTcpSocket::connected,
            this, &TcpAgent::onConnected);
    connect(m_socket, &QTcpSocket::disconnected,
            this, &TcpAgent::onDisconnected);
}

void TcpAgent::connectToServer(const QString& host, quint16 port)
{
    m_host = host;
    m_port = port;
    m_socket->connectToHost(m_host, m_port);
}

void TcpAgent::start()
{
    // 先连接服务器（此时已在正确的线程中）
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        m_socket->connectToHost(m_host, m_port);
    }
    // 再启动定时器
    m_timer->start(m_intervalMs);
}


void TcpAgent::stop()
{
    m_timer->stop();
}

// ===== 定时器触发 =====
void TcpAgent::onTimerTick()
{
    QVariantMap data = generateData();
    sendData(data);
}

// ===== TCP 发送 JSON =====
void TcpAgent::sendData(const QVariantMap& data)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        return;  // 没连上，跳过
    }

    // 打包：{ "name":"CPU使用率", "value":"45%", "status":"🟢 正常", "color":"#059669" }
    QJsonObject json = QJsonObject::fromVariantMap(data);
    json["name"] = m_name;   // 附加设备名，服务器用来区分是哪个 Agent
    m_socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    m_socket->write("\n");   // 每条 JSON 用换行分隔
}

// ===== 连接成功 =====
void TcpAgent::onConnected()
{
    m_retryCount = 0;
    emit statusChanged(m_name, "已连接");
}

// ===== 断开 → 自动重连 =====
void TcpAgent::onDisconnected()
{
    emit statusChanged(m_name, "已断开");
    tryReconnect();
}

void TcpAgent::tryReconnect()
{
    if (kMaxRetries > 0 && m_retryCount >= kMaxRetries) {
        return;  // 达到最大重试次数，放弃
    }
    m_retryCount++;
    QTimer::singleShot(kReconnectDelayMs, this, [this]() {
        emit statusChanged(m_name, "重连中...");
        m_socket->connectToHost(m_host, m_port);
    });
}
