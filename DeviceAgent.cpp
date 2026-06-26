#include "DeviceAgent.h"
#include <QTimer>

// 默认采集间隔（子类未指定时的回退值）
namespace {
constexpr int kDefaultIntervalMs = 2000;  // 2 秒
}

// ============================================================
// 构造函数
// ============================================================
DeviceAgent::DeviceAgent(const QString& name, int intervalMs, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_timer(new QTimer(this))          // parent 设置 this，对象树自动析构
    , m_intervalMs(intervalMs > 0 ? intervalMs : kDefaultIntervalMs)
{
    // 定时器超时 → 调用 generateData() → 发射信号
    connect(m_timer, &QTimer::timeout, this, [this]() {
        QVariantMap result = generateData();           // ① 子类生成数据

        emit dataReported(
            m_name,
            result.value("value").toString(),          // ② 数值
            result.value("status").toString(),         // ③ 状态
            result.value("color").toString()           // ④ 颜色
            );
    });
}

// ============================================================
// 启动采集
// ============================================================
void DeviceAgent::start()
{
    m_timer->start(m_intervalMs);  // 按设定间隔触发定时器
}

// ============================================================
// 停止采集
// ============================================================
void DeviceAgent::stop()
{
    m_timer->stop();  // 停止定时器，不再生成数据
}
