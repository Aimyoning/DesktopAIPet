#ifndef DEVICECARD_H
#define DEVICECARD_H

#include <QWidget>

class QLabel;

/**
 * DeviceCard —— 单个设备状态卡片
 *
 * 职责：显示设备名 + 数值 + 状态 + 带颜色边框
 * 可复用的独立组件，4 个设备各创建一个实例
 */
class DeviceCard : public QWidget
{
    Q_OBJECT

public:
    // 构造时传入设备名、数值、状态文字、边框颜色
    explicit DeviceCard(const QString& name, const QString& value,
                        const QString& status, const QString& borderColor,
                        QWidget* parent = nullptr);
    ~DeviceCard() override;

    // 更新显示数据
    void updateData(const QString& value, const QString& status,
                    const QString& borderColor);

private:
    QLabel* m_label;    // 显示设备信息的标签
    QString m_name;     // 设备名
};

#endif // DEVICECARD_H
