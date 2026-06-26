#include "MotorAgent.h"
#include <QRandomGenerator>

namespace {
// 转速范围
constexpr int kMinRPM     = 0;     // 最低转速（停机）
constexpr int kMaxRPM     = 3000;  // 最高转速
constexpr int kNormalMin  = 500;   // 正常转速下限
constexpr int kWarnMax    = 2500;  // 警告转速上限

// 状态颜色
const QString kColorNormal  = QStringLiteral("#059669");  // 绿色
const QString kColorWarning = QStringLiteral("#d97706");  // 黄色
const QString kColorDanger  = QStringLiteral("#dc2626");  // 红色
const QString kColorIdle    = QStringLiteral("#6b7280");  // 灰色（停机）
}

// ============================================================
// 构造函数
// ============================================================
MotorAgent::MotorAgent(const QString& name, int intervalMs, QObject* parent)
    : DeviceAgent(name, intervalMs, parent)
{
}

// ============================================================
// 生成模拟转速数据
// ============================================================
QVariantMap MotorAgent::generateData()
{
    // ① 随机生成 0~3000 之间的转速
    int rpm = QRandomGenerator::global()->bounded(kMaxRPM + 1);

    // ② 根据转速范围判断状态
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

    // ③ 打包返回
    QVariantMap data;
    data["value"]  = QString("%1rpm").arg(rpm);   // 如 "1850rpm"
    data["status"] = status;
    data["color"]  = color;
    return data;
}
