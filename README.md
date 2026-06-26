# DesktopAIPet — AI 桌面助手 & Agent 设备监控系统

基于 **Qt 6.11 + C++17 + CMake** 开发的 Windows 桌面应用，集成 DeepSeek AI 对话与多线程设备监控。

## 功能特性

- **AI 对话** — DeepSeek API 驱动，支持上下文记忆，SQLite 持久化聊天记录
- **角色动画** — GIF 动画小人，空闲/说话状态自动切换
- **设备监控** — 4 路独立 Agent 线程，实时采集 CPU/内存使用率与模拟电机转速
- **异常诊断** — 数据越界自动触发 AI 分析建议
- **深色主题** — 自定义 QSS 样式

## 项目架构

```
src/
├── ui/                  # UI 层
│   ├── DesktopAIPet     # 主窗口
│   ├── PetWidget        # GIF 角色组件
│   ├── ChatPanel        # 聊天面板
│   └── DeviceCard       # 设备状态卡片
└── business/            # 业务层
    ├── AiClient         # AI API 客户端
    ├── DeviceAgent      # Agent 基类
    ├── SensorAgent      # CPU/内存传感器
    └── MotorAgent       # 电机模拟
```

## 技术栈

| 类别 | 技术 |
|------|------|
| 框架 | Qt 6.11 (Widgets + Network + Sql) |
| 构建 | CMake + MinGW 64-bit |
| AI | DeepSeek API |
| 持久化 | SQLite |
| 并发 | QThread + moveToThread |

## 构建

```bash
git clone git@github.com:Aimyoning/DesktopAIPet.git
# Qt Creator 打开 CMakeLists.txt → Ctrl+R
```
