#include "business/MotorAgent.h"
#include <QRandomGenerator>

namespace {
constexpr int kMinRPM     = 0;
constexpr int kMaxRPM     = 3000;
constexpr int kNormalMin  = 500;
constexpr int kWarnMax    = 2500;

const QString kColorNormal  = QStringLiteral("#059669");
const QString kColorWarning = QStringLiteral("#d97706");
const QString kColorDanger  = QStringLiteral("#dc2626");
const QString kColorIdle    = QStringLiteral("#6b7280");
}

MotorAgent::MotorAgent(const QString& name, int intervalMs, QObject* parent)
    : DeviceAgent(name, intervalMs, parent)
{
}

QVariantMap MotorAgent::generateData()
{
    int rpm = QRandomGenerator::global()->bounded(kMaxRPM + 1);

    QString status;
    QString color;
    if (rpm == 0) {
        status = QStringLiteral("⚪ 停机");
        color  = kColorIdle;
    } else if (rpm < kNormalMin) {
        status = QStringLiteral("🟡 低速");
        color  = kColorWarning;
    } else if (rpm <= kWarnMax) {
        status = QStringLiteral("🟢 正常");
        color  = kColorNormal;
    } else {
        status = QStringLiteral("🔴 超速");
        color  = kColorDanger;
    }

    QVariantMap data;
    data["value"]  = QString("%1rpm").arg(rpm);
    data["status"] = status;
    data["color"]  = color;
    return data;
}
