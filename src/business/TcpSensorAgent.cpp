#include "business/TcpSensorAgent.h"
#include <QProcess>

namespace {
constexpr int kCpuNormalMax = 60, kCpuWarnMax = 85;
constexpr int kMemNormalMax = 70, kMemWarnMax = 90;
const QString kGreen  = QStringLiteral("#059669");
const QString kYellow = QStringLiteral("#d97706");
const QString kRed    = QStringLiteral("#dc2626");
const QString kGray   = QStringLiteral("#6b7280");
}

TcpSensorAgent::TcpSensorAgent(const QString& name, DataSource source,
                               int intervalMs, QObject* parent)
    : TcpAgent(name, intervalMs, parent), m_source(source) {}

QVariantMap TcpSensorAgent::generateData()
{
    int usage = (m_source == DataSource::Cpu) ? readCpuUsage() : readMemoryUsage();
    int nMax  = (m_source == DataSource::Cpu) ? kCpuNormalMax : kMemNormalMax;
    int wMax  = (m_source == DataSource::Cpu) ? kCpuWarnMax   : kMemWarnMax;

    QString val, st, cl;
    if (usage < 0) {
        val="--%"; st="⚪ 无数据"; cl=kGray;
    } else {
        val = QString("%1%").arg(usage);
        if (usage <= nMax)      { st="🟢 正常"; cl=kGreen; }
        else if (usage <= wMax) { st="🟡 注意"; cl=kYellow; }
        else                    { st="🔴 高负载"; cl=kRed; }
    }
    return {{"value",val},{"status",st},{"color",cl}};
}

int TcpSensorAgent::readCpuUsage()
{
    QProcess p; p.start("wmic",{"cpu","get","loadpercentage","/value"});
    if(!p.waitForFinished(3000)) return -1;
    QString o = QString::fromLocal8Bit(p.readAllStandardOutput()).trimmed();
    int i=o.indexOf('='); if(i<0)return -1;
    bool ok; int v=o.mid(i+1).trimmed().toInt(&ok); return ok?v:-1;
}

int TcpSensorAgent::readMemoryUsage()
{
    QProcess p; p.start("wmic",{"computersystem","get","totalphysicalmemory","/value"});
    if(!p.waitForFinished(3000)) return -1;
    QString t=QString::fromLocal8Bit(p.readAllStandardOutput()).trimmed();
    int ti=t.indexOf('='); if(ti<0)return -1;
    bool ok; qint64 total=t.mid(ti+1).trimmed().toLongLong(&ok);
    if(!ok||total<=0) return -1;
    p.start("wmic",{"os","get","freephysicalmemory","/value"});
    if(!p.waitForFinished(3000)) return -1;
    QString f=QString::fromLocal8Bit(p.readAllStandardOutput()).trimmed();
    int fi=f.indexOf('='); if(fi<0)return -1;
    qint64 free=f.mid(fi+1).trimmed().toLongLong(&ok);
    if(!ok) return -1;
    return static_cast<int>((total-free*1024.0)/total*100.0);
}
