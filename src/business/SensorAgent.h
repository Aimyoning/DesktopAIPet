#pragma once
#include "business/DeviceAgent.h"

enum class DataSource {
    Cpu,
    Memory
};

class SensorAgent : public DeviceAgent
{
    Q_OBJECT
    Q_DISABLE_COPY(SensorAgent)

public:
    explicit SensorAgent(const QString& name,
                         DataSource source = DataSource::Cpu,
                         int intervalMs = 2000,
                         QObject* parent = nullptr);

protected:
    QVariantMap generateData() override;

private:
    DataSource m_source;
    int readCpuUsage();
    int readMemoryUsage();
};
