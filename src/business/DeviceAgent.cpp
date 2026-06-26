#include "business/DeviceAgent.h"
#include <QTimer>

namespace {
constexpr int kDefaultIntervalMs = 2000;
}

DeviceAgent::DeviceAgent(const QString& name, int intervalMs, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_timer(new QTimer(this))
    , m_intervalMs(intervalMs > 0 ? intervalMs : kDefaultIntervalMs)
{
    connect(m_timer, &QTimer::timeout, this, [this]() {
        QVariantMap result = generateData();
        emit dataReported(
            m_name,
            result.value("value").toString(),
            result.value("status").toString(),
            result.value("color").toString()
        );
    });
}

void DeviceAgent::start()
{
    m_timer->start(m_intervalMs);
}

void DeviceAgent::stop()
{
    m_timer->stop();
}
