#pragma once
#include "business/TcpAgent.h"

class TcpMotorAgent : public TcpAgent
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpMotorAgent)

public:
    explicit TcpMotorAgent(const QString& name, int intervalMs = 1500,
                           QObject* parent = nullptr);

protected:
    QVariantMap generateData() override;
};
