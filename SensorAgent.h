#pragma once
#include "DeviceAgent.h"

/**
 * SensorAgent —— 系统传感器代理
 *
 * 通过 wmic 读取真实系统数据（CPU 使用率 / 内存使用率），
 * 通过基类的 dataReported 信号上报给管理端。
 */

// 数据来源枚举
enum class DataSource {
    Cpu,     // CPU 使用率（%）
    Memory   // 内存使用率（%）
};

class SensorAgent : public DeviceAgent
{
    Q_OBJECT
    Q_DISABLE_COPY(SensorAgent)

public:
    /// @param name       传感器名称（如"CPU使用率"）
    /// @param source     数据来源（Cpu 或 Memory）
    /// @param intervalMs 采集间隔，默认 2000ms
    explicit SensorAgent(const QString& name,
                         DataSource source = DataSource::Cpu,
                         int intervalMs = 2000,
                         QObject* parent = nullptr);

protected:
    QVariantMap generateData() override;

private:
    DataSource m_source;             // 数据来源
    int readCpuUsage();              // wmic 读 CPU 使用率
    int readMemoryUsage();           // wmic 读内存使用率
};
