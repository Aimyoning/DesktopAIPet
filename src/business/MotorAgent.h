#pragma once
#include "business/DeviceAgent.h"

class MotorAgent : public DeviceAgent
{
    Q_OBJECT
    Q_DISABLE_COPY(MotorAgent)

public:
    explicit MotorAgent(const QString& name, int intervalMs = 1500,
                        QObject* parent = nullptr);

protected:
    QVariantMap generateData() override;
};
