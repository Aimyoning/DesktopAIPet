#include "ui/PetWidget.h"
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QFile>            // ← 新增：检测文件是否存在

namespace {
// GIF 资源路径
const QString kIdlePath = QStringLiteral("assets/idle.gif");
const QString kTalkPath = QStringLiteral("assets/talk.gif");
}

PetWidget::PetWidget(QWidget* parent) : QWidget(parent)
{
    // ---- 创建显示控件 ----
    m_display = new QLabel(this);
    m_display->setAlignment(Qt::AlignCenter);
    m_display->setMinimumSize(200, 280);
    m_display->setStyleSheet(
        "background:#16213e; border-radius:12px; font-size:48px; color:#00d4ff;");

    // ---- 状态文字 ----
    m_stateLabel = new QLabel("空闲中", this);
    m_stateLabel->setAlignment(Qt::AlignCenter);
    m_stateLabel->setStyleSheet("color:#888; font-size:12px;");

    // ---- 创建动画播放器 ----
    m_idleMovie = new QMovie(this);
    m_talkMovie = new QMovie(this);

    // ---- 加载 GIF ----
    if (QFile::exists(kIdlePath)) {
        m_idleMovie->setFileName(kIdlePath);
    } else {
        m_display->setText("🤖\n\n请将 idle.gif\n放入 assets/");  // 占位
    }

    if (QFile::exists(kTalkPath)) {
        m_talkMovie->setFileName(kTalkPath);
    }

    // ---- 布局 ----
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    layout->addWidget(m_display);
    layout->addWidget(m_stateLabel);
}

PetWidget::~PetWidget() = default;

// ---- 切换到空闲状态 ----
void PetWidget::setIdle()
{
    m_stateLabel->setText("空闲中");
    m_stateLabel->setStyleSheet("color:#888; font-size:12px;");

    // 有 GIF → 播放空闲动画；无 GIF → 显示 emoji
    if (m_idleMovie->isValid()) {
        m_display->setMovie(m_idleMovie);
        m_idleMovie->start();
    } else {
        m_display->setText("🤖");
    }
}

// ---- 切换到说话状态 ----
void PetWidget::setTalking()
{
    m_stateLabel->setText("说话中...");
    m_stateLabel->setStyleSheet("color:#00d4ff; font-size:12px;");

    // 有 GIF → 播放说话动画；无 GIF → 动感 emoji
    if (m_talkMovie->isValid()) {
        m_display->setMovie(m_talkMovie);
        m_talkMovie->start();
    } else {
        m_display->setText("🤖💬");
    }
}
