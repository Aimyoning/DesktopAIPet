#pragma once
#include "DeviceAgent.h"

/**
 * MotorAgent —— 电机代理
 *
 * 模拟电机转速，每 1.5 秒生成一次随机 RPM，
 * 通过基类的 dataReported 信号上报给管理端。
 */
class MotorAgent : public DeviceAgent
{
    Q_OBJECT
    Q_DISABLE_COPY(MotorAgent)

public:
    /// @param name       电机名称（如"电机A"）
    /// @param intervalMs 采集间隔，默认 1500ms
    explicit MotorAgent(const QString& name, int intervalMs = 1500,
                        QObject* parent = nullptr);

protected:
    /// 生成随机转速 + 状态判断
    QVariantMap generateData() override;
};
