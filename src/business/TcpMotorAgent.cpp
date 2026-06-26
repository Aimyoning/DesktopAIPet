#include "business/TcpMotorAgent.h"
#include <QRandomGenerator>

namespace {
constexpr int kMaxRPM=3000, kNormalMin=500, kWarnMax=2500;
const QString kGreen="#059669", kYellow="#d97706", kRed="#dc2626", kGray="#6b7280";
}

TcpMotorAgent::TcpMotorAgent(const QString& name, int intervalMs, QObject* parent)
    : TcpAgent(name, intervalMs, parent) {}

QVariantMap TcpMotorAgent::generateData()
{
    int rpm = QRandomGenerator::global()->bounded(kMaxRPM+1);
    QString st, cl;
    if (rpm == 0)               { st="⚪ 停机"; cl=kGray; }
    else if (rpm < kNormalMin)  { st="🟡 低速"; cl=kYellow; }
    else if (rpm <= kWarnMax)   { st="🟢 正常"; cl=kGreen; }
    else                        { st="🔴 超速"; cl=kRed; }
    return {{"value",QString("%1rpm").arg(rpm)},{"status",st},{"color",cl}};
}
