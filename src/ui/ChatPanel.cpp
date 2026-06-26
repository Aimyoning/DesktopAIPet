#include "ui/ChatPanel.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>

ChatPanel::ChatPanel(QWidget* parent) : QWidget(parent)
{
    setupUI();
    setupConnections();
}

ChatPanel::~ChatPanel() = default;

// ---- 创建控件 + 布局 ----
void ChatPanel::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    // 标题
    QLabel* title = new QLabel("🤖 Robot", this);
    title->setStyleSheet("font-size:16px; font-weight:bold; color:#00d4ff; padding:4px;");

    // 聊天记录（只读）
    m_history = new QTextEdit(this);
    m_history->setReadOnly(true);
    m_history->setPlaceholderText("我正在等待你的消息...");
    m_history->setStyleSheet(
        "background:#0d1117; color:#c9d1d9; border:1px solid #30363d; "
        "border-radius:6px; padding:10px; font-size:14px;"

        // ── 垂直滚动条样式 ──
        "QScrollBar:vertical {"
        "  background:#0d1117; width:10px; margin:0;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background:#30363d; border-radius:5px; min-height:30px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "  background:#484f58;"                         // 鼠标悬停变亮
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height:0px;"                                 // 去掉上下箭头
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "  background:none;"                            // 轨道空白区透明
        "}");


    // 输入行
    QHBoxLayout* inputRow = new QHBoxLayout;
    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("输入消息，按 Enter 发送...");
    m_input->setStyleSheet(
        "background:#0d1117; color:#e0e0e0; border:1px solid #30363d; "
        "border-radius:4px; padding:10px; font-size:14px;");

    m_sendBtn = new QPushButton("发送", this);
    m_sendBtn->setStyleSheet(
        "background:#238636; color:white; border:none; border-radius:4px; "
        "padding:10px 24px; font-weight:bold;");

    inputRow->addWidget(m_input, 1);   // stretch=1：输入框占满
    inputRow->addWidget(m_sendBtn);

    layout->addWidget(title);
    layout->addWidget(m_history, 1);   // stretch=1：聊天记录占满剩余空间
    layout->addLayout(inputRow);
}

// ---- 信号槽连接 ----
void ChatPanel::setupConnections()
{
    // 发送按钮 → 读取输入框文本 → 发射 messageSent 信号
    connect(m_sendBtn, &QPushButton::clicked, this, [this]() {
        QString text = m_input->text().trimmed();
        if (!text.isEmpty()) {
            emit messageSent(text);   // 发射信号，通知外部处理
        }
    });

    // Enter 键 → 同上
    connect(m_input, &QLineEdit::returnPressed, this, [this]() {
        QString text = m_input->text().trimmed();
        if (!text.isEmpty()) {
            emit messageSent(text);
        }
    });
}

// ---- 显示用户消息（绿色） ----
void ChatPanel::appendUserMessage(const QString& text)
{
    m_history->append(
        QString("<span style='color:#00ff00;'>👤 你: %1</span>")
            .arg(text.toHtmlEscaped()));

    // 自动滚到底部
    QScrollBar* sb = m_history->verticalScrollBar();
    sb->setValue(sb->maximum());
}

// ---- 显示 AI 回复（浅蓝色） ----
void ChatPanel::appendAIReply(const QString& text)
{
    m_history->append(
        QString("<span style='color:#88c0d0;'>🤖 AI: %1</span>")
            .arg(text.toHtmlEscaped().replace("\n", "<br>")));

    QScrollBar* sb = m_history->verticalScrollBar();
    sb->setValue(sb->maximum());
}

// ---- 清空输入框 ----
void ChatPanel::clearInput()
{
    m_input->clear();
}
