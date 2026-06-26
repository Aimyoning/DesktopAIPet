#ifndef CHATPANEL_H
#define CHATPANEL_H

#include <QWidget>

class QTextEdit;    // 聊天记录区
class QLineEdit;    // 输入框
class QPushButton;  // 发送按钮

/**
 * ChatPanel —— 聊天面板组件
 *
 * 职责：聊天记录显示 + 用户输入 + 发送信号
 * 不包含 AI 调用逻辑（后续由 AIClient 模块处理）
 */
class ChatPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPanel(QWidget* parent = nullptr);
    ~ChatPanel() override;

    // 显示消息（供外部调用）
    void appendUserMessage(const QString& text);     // 绿字用户消息
    void appendAIReply(const QString& text);         // 蓝字 AI 回复
    void clearInput();                               // 清空输入框

signals:
    // 用户点击发送 / 按 Enter → 发射此信号
    // 外部（DesktopAIPet）连接此信号，处理消息逻辑
    void messageSent(const QString& text);

private:
    void setupUI();
    void setupConnections();

    QTextEdit*   m_history;    // 聊天记录
    QLineEdit*   m_input;      // 输入框
    QPushButton* m_sendBtn;    // 发送按钮
};

#endif // CHATPANEL_H
