#include "ui/DeviceCard.h"
#include <QLabel>
#include <QVBoxLayout>

DeviceCard::DeviceCard(const QString& name, const QString& value,
                       const QString& status, const QString& borderColor,
                       QWidget* parent)
    : QWidget(parent), m_name(name)
{
    // 创建标签，显示设备信息
    m_label = new QLabel(QString("%1\n%2\n%3").arg(name, value, status), this);
    m_label->setAlignment(Qt::AlignCenter);

    // 设置卡片样式：深色背景 + 彩色边框
    setMinimumSize(160, 80);
    setStyleSheet(
        QString("background:#16213e; border:2px solid %1; "
                "border-radius:8px; font-size:13px; color:#e0e0e0; padding:8px;")
            .arg(borderColor));

    // 布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(m_label);
}

DeviceCard::~DeviceCard() = default;

// ---- 更新设备数据 ----
void DeviceCard::updateData(const QString& value, const QString& status,
                            const QString& borderColor)
{
    m_label->setText(QString("%1\n%2\n%3").arg(m_name, value, status));
    setStyleSheet(
        QString("background:#16213e; border:2px solid %1; "
                "border-radius:8px; font-size:13px; color:#e0e0e0; padding:8px;")
            .arg(borderColor));
}
