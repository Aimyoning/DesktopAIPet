# DesktopAIPet — AI 桌面助手 & TCP 设备监控系统

基于 **Qt 6.11 + C++17 + CMake** 开发的 Windows 桌面应用，集成 DeepSeek AI 对话与 TCP 多线程设备监控。

## 功能特性

- **AI 对话** — DeepSeek API 驱动，上下文记忆，SQLite 持久化聊天记录
- **角色动画** — GIF 动画小人，空闲/说话状态自动切换
- **TCP 设备监控** — 4 路独立 Agent，通过 TCP JSON 协议上报 CPU/内存/电机数据
- **真实数据采集** — wmic 读取本机 CPU 使用率与内存占用
- **异常诊断** — 数据越界自动触发 AI 分析建议，4 个独立冷却器防刷屏
- **断线重连** — Agent TCP 断开后自动重连，3 秒间隔
- **深色主题** — 自定义 QSS 样式，GitHub 风格滚动条

## 项目架构

```
src/
├── ui/                    # UI 层
│   ├── DesktopAIPet       # 主窗口（信号槽中枢）
│   ├── PetWidget          # GIF 角色组件
│   ├── ChatPanel          # 聊天面板
│   └── DeviceCard         # 设备状态卡片
└── business/              # 业务层
    ├── AiClient           # DeepSeek API 客户端
    ├── TcpAgent           # TCP Agent 基类（断线重连）
    ├── TcpServer          # TCP 服务器（JSON 解析 + 路由）
    ├── TcpSensorAgent     # CPU/内存传感器 Agent
    └── TcpMotorAgent      # 电机模拟 Agent
```

## 数据流

```
Agent 子线程 (wmic/随机) → TCP JSON → TcpServer → agentDataReceived 信号 → UI 更新
```

## 技术栈

| 类别 | 技术 |
|------|------|
| 框架 | Qt 6.11 (Core + Widgets + Network + Sql) |
| 构建 | CMake + MinGW 64-bit (GCC 16.1.0) |
| AI | DeepSeek API (deepseek-chat) |
| 持久化 | SQLite (聊天记录) |
| 并发 | QThread + moveToThread + QTcpSocket |
| 通信 | QTcpServer / QTcpSocket, JSON 协议 |

## 构建

```bash
git clone git@github.com:Aimyoning/DesktopAIPet.git
# Qt Creator 打开 CMakeLists.txt → 配置 MinGW 64 Kit → Ctrl+R
```

## 运行前

设置环境变量（终端中执行）：

```bash
set DEEPSEEK_API_KEY=你的API Key
```

然后启动程序。
