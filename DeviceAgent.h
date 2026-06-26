#pragma once
#include <QObject>
#include <QVariantMap>

// 前向声明：减少编译依赖
class QTimer;

/**
 * DeviceAgent —— 设备代理基类
 *
 * 每个 Agent = 一个独立的工作线程 + 定时采集任务。
 * 子类只需重写 generateData() 返回模拟数据，
 * 基类负责定时器 + 信号上报。
 *
 * 使用方式（管理端）：
 *   1. auto* agent = new SensorAgent(...);
 *   2. auto* thread = new QThread(this);
 *   3. agent->moveToThread(thread);
 *   4. connect(thread, &QThread::started, agent, &DeviceAgent::start);
 *   5. thread->start();
 */
class DeviceAgent : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DeviceAgent)

public:
    /// @param name       设备名称，用于界面显示（如"传感器1"）
    /// @param intervalMs 数据采集间隔，单位毫秒
    explicit DeviceAgent(const QString& name, int intervalMs,
                         QObject* parent = nullptr);
    ~DeviceAgent() override = default;

    /// 设备名称（只读）
    const QString& name() const { return m_name; }

public slots:
    /// 启动定时采集（线程启动后由信号触发）
    void start();

    /// 停止定时采集
    void stop();

signals:
    /// 新数据上报：设备名称 + 数值 + 状态文字 + 状态颜色（#RRGGBB）
    void dataReported(const QString& name, const QString& value,
                      const QString& status, const QString& color);

protected:
    /// 子类重写此方法，返回模拟的设备数据
    /// @return QVariantMap {
    ///     "value"  → QString（数值文字，如"85°C"）
    ///     "status" → QString（状态文字，如"🟢 正常"）
    ///     "color"  → QString（颜色代码，如"#059669"）
    /// }
    virtual QVariantMap generateData() = 0;

private:
    QString  m_name;       // 设备名称
    QTimer*  m_timer;      // 定时器（由对象树管理，无需手动 delete）
    int      m_intervalMs; // 采集间隔（毫秒）
};
