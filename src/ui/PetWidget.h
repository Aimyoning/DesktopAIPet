#ifndef PETWIDGET_H
#define PETWIDGET_H

#include <QWidget>

class QLabel;   // 前置声明，加快编译
class QMovie;   // GIF 动画播放器

/**
 * PetWidget —— GIF 角色显示组件
 *
 * 职责：显示 GIF 动画 + 切换空闲/说话状态
 * 不包含任何聊天或设备逻辑
 */
class PetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PetWidget(QWidget* parent = nullptr);
    ~PetWidget() override;

    // 切换角色状态
    void setIdle();      // 切换到空闲动画
    void setTalking();   // 切换到说话动画

private:
    QLabel* m_display;      // 显示 GIF 的标签
    QLabel* m_stateLabel;   // 状态文字（空闲中/说话中...）
    QMovie* m_idleMovie;    // 空闲 GIF
    QMovie* m_talkMovie;    // 说话 GIF
};

#endif // PETWIDGET_H
