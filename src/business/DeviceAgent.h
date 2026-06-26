#pragma once
#include <QObject>
#include <QVariantMap>

class QTimer;

class DeviceAgent : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DeviceAgent)

public:
    explicit DeviceAgent(const QString& name, int intervalMs,
                         QObject* parent = nullptr);
    ~DeviceAgent() override = default;

    const QString& name() const { return m_name; }

public slots:
    void start();
    void stop();

signals:
    void dataReported(const QString& name, const QString& value,
                      const QString& status, const QString& color);

protected:
    virtual QVariantMap generateData() = 0;

private:
    QString  m_name;
    QTimer*  m_timer;
    int      m_intervalMs;
};
