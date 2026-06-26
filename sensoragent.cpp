#include "SensorAgent.h"
#include <QProcess>

namespace {
// 阈值
constexpr int kCpuNormalMax    = 60;
constexpr int kCpuWarnMax      = 85;
constexpr int kMemNormalMax    = 70;
constexpr int kMemWarnMax      = 90;

// 状态颜色
const QString kColorNormal  = QStringLiteral("#059669");
const QString kColorWarning = QStringLiteral("#d97706");
const QString kColorDanger  = QStringLiteral("#dc2626");
const QString kColorUnknown = QStringLiteral("#6b7280");
}

// ============================================================
// 构造函数
// ============================================================
SensorAgent::SensorAgent(const QString& name, DataSource source,
                         int intervalMs, QObject* parent)
    : DeviceAgent(name, intervalMs, parent)
    , m_source(source)
{
}

// ============================================================
// 按来源读取数据
// ============================================================
QVariantMap SensorAgent::generateData()
{
    int usage = (m_source == DataSource::Cpu)
    ? readCpuUsage()
    : readMemoryUsage();

    int normalMax = (m_source == DataSource::Cpu) ? kCpuNormalMax : kMemNormalMax;
    int warnMax   = (m_source == DataSource::Cpu) ? kCpuWarnMax   : kMemWarnMax;

    QString value;
    QString status;
    QString color;

    if (usage < 0) {
        value  = QStringLiteral("--%");
        status = QStringLiteral("⚪ 无数据");
        color  = kColorUnknown;
    } else {
        value = QString("%1%").arg(usage);
        if (usage <= normalMax) {
            status = QStringLiteral("🟢 正常");
            color  = kColorNormal;
        } else if (usage <= warnMax) {
            status = QStringLiteral("🟡 注意");
            color  = kColorWarning;
        } else {
            status = QStringLiteral("🔴 高负载");
            color  = kColorDanger;
        }
    }

    QVariantMap data;
    data["value"]  = value;
    data["status"] = status;
    data["color"]  = color;
    return data;
}

// ============================================================
// wmic 读 CPU 使用率
// ============================================================
int SensorAgent::readCpuUsage()
{
    QProcess process;
    process.start("wmic", QStringList()
                              << "cpu" << "get" << "loadpercentage" << "/value");

    if (!process.waitForFinished(3000)) return -1;

    QString output = QString::fromLocal8Bit(
                         process.readAllStandardOutput()).trimmed();
    int idx = output.indexOf('=');
    if (idx < 0) return -1;

    bool ok = false;
    int usage = output.mid(idx + 1).trimmed().toInt(&ok);
    return ok ? usage : -1;
}

// ============================================================
// wmic 读内存使用率（总量 - 空闲）/ 总量 × 100
// ============================================================
int SensorAgent::readMemoryUsage()
{
    // ① 读取总内存
    QProcess process;
    process.start("wmic", QStringList()
                              << "computersystem" << "get" << "totalphysicalmemory" << "/value");
    if (!process.waitForFinished(3000)) return -1;
    QString totalOut = QString::fromLocal8Bit(
                           process.readAllStandardOutput()).trimmed();
    int tidx = totalOut.indexOf('=');
    if (tidx < 0) return -1;
    bool ok = false;
    qint64 totalMemory = totalOut.mid(tidx + 1).trimmed().toLongLong(&ok);
    if (!ok || totalMemory <= 0) return -1;

    // ② 读取空闲内存
    process.start("wmic", QStringList()
                              << "os" << "get" << "freephysicalmemory" << "/value");
    if (!process.waitForFinished(3000)) return -1;
    QString freeOut = QString::fromLocal8Bit(
                          process.readAllStandardOutput()).trimmed();
    int fidx = freeOut.indexOf('=');
    if (fidx < 0) return -1;
    qint64 freeMemory = freeOut.mid(fidx + 1).trimmed().toLongLong(&ok);
    if (!ok) return -1;

    // ③ 计算使用率：wmic 返回的单位是字节，free 是 KB
    // totalphysicalmemory 单位：字节
    // freephysicalmemory 单位：KB
    double usedMemBytes = totalMemory - (freeMemory * 1024.0);
    return static_cast<int>(usedMemBytes / totalMemory * 100.0);
}
