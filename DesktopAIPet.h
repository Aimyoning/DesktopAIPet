#pragma once
#include <QWidget>

// 前置声明
class PetWidget;
class ChatPanel;
class DeviceCard;
class AiClient;
class QThread;
class SensorAgent;
class MotorAgent;
#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMessageBox>
class DesktopAIPet : public QWidget
{
    Q_OBJECT

public:
    explicit DesktopAIPet(QWidget* parent = nullptr);
    ~DesktopAIPet() override;
    void setApiKey(const QString& key);
private:
    void setupUI();              // 组装子组件
    void setupConnections();     // 连接子组件信号
    void setupAgents();          // 创建 + 启动 Agent 线程  ← 新增
    void applyQSS();             // 全局深色主题
    void restoreChatHistory();   // 回放聊天历史
    void onAgentData(const QString& name, const QString& value ,const QString& status);  // 检查是否异常
    void triggerDiagnosis(const QString& name, const QString& value,const QString& status);  // 触发 AI 诊断
    void closeEvent(QCloseEvent* event) override;
    // ======== UI 子组件 ========
    PetWidget*   m_petWidget  = nullptr;
    ChatPanel*   m_chatPanel  = nullptr;
    DeviceCard*  m_device1    = nullptr;  // 传感器1 卡片
    DeviceCard*  m_device2    = nullptr;  // 传感器2 卡片
    DeviceCard*  m_device3    = nullptr;  // 电机A 卡片
    DeviceCard*  m_device4    = nullptr;  // 电机B 卡片

    // ======== 业务组件 ========
    AiClient*    m_aiClient   = nullptr;

    // ======== Agent + 线程 ========
    SensorAgent* m_sensor1    = nullptr;
    SensorAgent* m_sensor2    = nullptr;
    MotorAgent*  m_motorA     = nullptr;
    MotorAgent*  m_motorB     = nullptr;

    QThread*     m_thread1    = nullptr;  // 传感器1 线程
    QThread*     m_thread2    = nullptr;  // 传感器2 线程
    QThread*     m_thread3    = nullptr;  // 电机A 线程
    QThread*     m_thread4    = nullptr;  // 电机B 线程
    // ======== 异常诊断 ========
    QTimer* m_cooldown1 = nullptr;  // 传感器1 冷却
    QTimer* m_cooldown2 = nullptr;  // 传感器2 冷却
    QTimer* m_cooldown3 = nullptr;  // 电机A 冷却
    QTimer* m_cooldown4 = nullptr;  // 电机B 冷却

};
