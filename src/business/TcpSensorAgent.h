#pragma once
#include "business/TcpAgent.h"

enum class DataSource { Cpu, Memory };

class TcpSensorAgent : public TcpAgent
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpSensorAgent)

public:
    explicit TcpSensorAgent(const QString& name,
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
