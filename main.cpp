#include "ui/DesktopAIPet.h"
#include <QApplication>
#include <QIcon>
#include <QProcessEnvironment>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 设置应用图标（任务栏 + 标题栏 + 快捷方式）
    app.setWindowIcon(QIcon("assets/robot.ico"));
    DesktopAIPet window;
    // ── 从环境变量读取 API Key ──
    QString apiKey = QProcessEnvironment::systemEnvironment()
                         .value("DEEPSEEK_API_KEY");
    if (apiKey.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("API Key 未设置");
        msgBox.setText(
            "<span style='color:#dc2626; font-size:13px;'>"
            "请设置环境变量 DEEPSEEK_API_KEY 后重新运行。<br><br>"
            "方法：终端中执行<br>"
            "&nbsp;&nbsp;set DEEPSEEK_API_KEY=你的Key"
            "</span>");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox { background-color:#1a1a2e; }"
            "QPushButton { background:#30363d; color:#c9d1d9; "
            "  border-radius:4px; padding:6px 20px; }");
        msgBox.exec();
    }
    window.setApiKey(apiKey);             // ← 通过公开方法设置，不再硬编码
    window.show();
    return app.exec();
}