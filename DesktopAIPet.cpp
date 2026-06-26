#include "DesktopAIPet.h"
#include "PetWidget.h"
#include "ChatPanel.h"
#include "DeviceCard.h"
#include "AiClient.h"
#include "SensorAgent.h"
#include "MotorAgent.h"
#include <QThread>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

// ============================================================
// 构造函数
// ============================================================
DesktopAIPet::DesktopAIPet(QWidget* parent)
    : QWidget(parent)
    , m_aiClient(new AiClient(this))
    , m_cooldown1(new QTimer(this))
    , m_cooldown2(new QTimer(this))
    , m_cooldown3(new QTimer(this))
    , m_cooldown4(new QTimer(this))
{
    setupUI();
    setupAgents();              // 创建 Agent 线程
    setupConnections();
    applyQSS();
    restoreChatHistory();


    setWindowTitle("AI 桌面助手 — DesktopAIPet");
    resize(900, 600);
    setMinimumSize(700, 450);

}

// ============================================================
// 析构函数：安全停止所有 Agent 线程
// ============================================================
DesktopAIPet::~DesktopAIPet()
{
    // 遍历 4 个线程，通知退出 + 等待结束
    for (QThread* thread : {m_thread1, m_thread2, m_thread3, m_thread4}) {
        if (thread) {
            thread->quit();           // ① 发送退出信号
            thread->wait(3000);       // ② 最多等 3 秒
        }
    }
}

// ---- 组装 UI（不变） ----
void DesktopAIPet::setupUI()
{
    QHBoxLayout* mainRow = new QHBoxLayout;
    mainRow->setContentsMargins(12, 12, 12, 12);
    mainRow->setSpacing(12);

    m_petWidget = new PetWidget(this);
    mainRow->addWidget(m_petWidget, 1);

    m_chatPanel = new ChatPanel(this);
    mainRow->addWidget(m_chatPanel, 3);

    QHBoxLayout* bottomRow = new QHBoxLayout;
    bottomRow->setSpacing(10);

    // 设备卡片初始值（Agent 启动后会动态覆盖）
    m_device1 = new DeviceCard("CPU使用率", "--", "等待中", "#6b7280", this);
    m_device2 = new DeviceCard("内存使用率", "--", "等待中", "#6b7280", this);
    m_device3 = new DeviceCard("电机A",   "--", "等待中", "#6b7280", this);
    m_device4 = new DeviceCard("电机B",   "--", "等待中", "#6b7280", this);

    bottomRow->addWidget(m_device1);
    bottomRow->addWidget(m_device2);
    bottomRow->addWidget(m_device3);
    bottomRow->addWidget(m_device4);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->addLayout(mainRow, 1);
    root->addLayout(bottomRow);
}

// ============================================================
// 创建 Agent + 线程 + 移动 + 启动
// ============================================================
void DesktopAIPet::setupAgents()
{
    // ── ① 创建 4 个 Agent 对象（无 parent，因为要 moveToThread） ──
    m_sensor1 = new SensorAgent("CPU使用率", DataSource::Cpu);
    m_sensor2 = new SensorAgent("内存使用率", DataSource::Memory);
    m_motorA  = new MotorAgent("电机A");      // 默认 1.5 秒采集
    m_motorB  = new MotorAgent("电机B");

    // ── ② 创建 4 个线程（parent 设 this，窗口关闭时自动回收） ──
    m_thread1 = new QThread(this);
    m_thread2 = new QThread(this);
    m_thread3 = new QThread(this);
    m_thread4 = new QThread(this);

    // ── ③ Agent 移动到各自线程 ──
    m_sensor1->moveToThread(m_thread1);
    m_sensor2->moveToThread(m_thread2);
    m_motorA ->moveToThread(m_thread3);
    m_motorB ->moveToThread(m_thread4);

    // ── ④ 线程启动 → 自动调用 Agent::start() 开始采集 ──
    connect(m_thread1, &QThread::started, m_sensor1, &SensorAgent::start);
    connect(m_thread2, &QThread::started, m_sensor2, &SensorAgent::start);
    connect(m_thread3, &QThread::started, m_motorA,  &MotorAgent::start);
    connect(m_thread4, &QThread::started, m_motorB,  &MotorAgent::start);

    // ── ⑤ Agent 数据 → 更新对应 DeviceCard ──
    // 注意：dataReported 在子线程发射，UI 更新必须在主线程
    //       Qt 的 AutoConnection 会自动识别跨线程，以 QueuedConnection 处理
    connect(m_sensor1, &SensorAgent::dataReported, this,
            [this](const QString& /*name*/, const QString& value,
                   const QString& status, const QString& color) {
                m_device1->updateData(value, status, color);
        onAgentData("CPU使用率", value, status);
            });
    connect(m_sensor2, &SensorAgent::dataReported, this,
            [this](const QString& /*name*/, const QString& value,
                   const QString& status, const QString& color) {
                m_device2->updateData(value, status, color);
        onAgentData("内存使用率", value, status);
            });
    connect(m_motorA, &MotorAgent::dataReported, this,
            [this](const QString& /*name*/, const QString& value,
                   const QString& status, const QString& color) {
                m_device3->updateData(value, status, color);
        onAgentData("电机A", value, status);
            });
    connect(m_motorB, &MotorAgent::dataReported, this,
            [this](const QString& /*name*/, const QString& value,
                   const QString& status, const QString& color) {
                m_device4->updateData(value, status, color);
        onAgentData("电击B", value, status);
            });

    // ── ⑥ 启动全部线程 ──
    m_thread1->start();
    m_thread2->start();
    m_thread3->start();
    m_thread4->start();
}

// ---- 信号槽连接（不变） ----
void DesktopAIPet::setupConnections()
{
    connect(m_chatPanel, &ChatPanel::messageSent, this, [this](const QString& text) {
        m_chatPanel->appendUserMessage(text);
        m_chatPanel->clearInput();
        m_petWidget->setTalking();
        m_aiClient->sendMessage(text);
    });

    connect(m_aiClient, &AiClient::replyReceived, this, [this](const QString& reply) {
        m_chatPanel->appendAIReply(reply);
        m_petWidget->setIdle();
    });

    connect(m_aiClient, &AiClient::errorOccurred, this, [this](const QString& err) {
        m_chatPanel->appendAIReply("抱歉，出了点问题：" + err);
        m_petWidget->setIdle();
    });
}

//QSS
void DesktopAIPet::applyQSS()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #1a1a2e;
            font-family: "Microsoft YaHei", "PingFang SC", sans-serif;
        }
    )");
}

// 回放聊天历史
void DesktopAIPet::restoreChatHistory()
{
    const auto& history = m_aiClient->chatHistory();
    for (const auto& [role, content] : history) {
        if (role == QStringLiteral("user")) {
            m_chatPanel->appendUserMessage(content);
        } else if (role == QStringLiteral("assistant")) {
            m_chatPanel->appendAIReply(content);
        }
    }
}
// ============================================================
// 收到 Agent 数据 → 如果是异常状态 → 触发 AI 诊断
// ============================================================
void DesktopAIPet::onAgentData(const QString& name, const QString& value,
                               const QString& status)
{
    if (!status.contains(QStringLiteral("正常"))) {
        // 根据设备名选对应冷却器
        QTimer* cooldown = nullptr;
        if (name == "CPU使用率")      cooldown = m_cooldown1;
        else if (name == "内存使用率") cooldown = m_cooldown2;
        else if (name == "电机A")   cooldown = m_cooldown3;
        else if (name == "电机B")   cooldown = m_cooldown4;

        if (cooldown && !cooldown->isActive()) {
            cooldown->start(5000);
            triggerDiagnosis(name, value, status);
        }
    }
}


// ============================================================
// 向 AI 发送诊断请求（带 5 秒冷却，防止刷屏）
// ============================================================
void DesktopAIPet::triggerDiagnosis(const QString& name, const QString& value,
                                    const QString& status)
{


    // 构造诊断提示词
    QString prompt = QStringLiteral(
                         "设备「%1」当前数据：%2，状态：%3。"
                         "请根据你的专业知识，分析可能的原因并给出简短建议（2-3句话）。")
                         .arg(name, value, status);

    // 发送给 AI（复用现有 AiClient，回复自动出现在聊天区）
    m_petWidget->setTalking();                 // 小人→说话态
    m_chatPanel->appendUserMessage(prompt);     // 提示词显示在聊天区
    m_aiClient->sendMessage(prompt);            // 异步发送
}
void DesktopAIPet::setApiKey(const QString& key)
{
    m_aiClient->setApiKey(key);
}

// ============================================================
// 关闭窗口 → 弹确认框
// ============================================================
void DesktopAIPet::closeEvent(QCloseEvent* event)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("退出确认");
    msgBox.setText(
        "<span style='color:#e0e0e0; font-size:13px;'>"
        "确定要关闭 AI 桌面助手吗？<br>"
        "所有 Agent 线程将停止。"
        "</span>");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(
        "QMessageBox { background-color:#1a1a2e; }"
        "QPushButton { background:#30363d; color:#c9d1d9; "
        "  border-radius:4px; padding:6px 20px; }");

    if (msgBox.exec() == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}
