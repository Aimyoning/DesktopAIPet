cmake_minimum_required(VERSION 3.19)
project(DesktopAIPet LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)  # 加了 Network
qt_standard_project_setup()

qt_add_executable(DesktopAIPet
    WIN32 MACOSX_BUNDLE
    main.cpp
    DesktopAIPet.h
    DesktopAIPet.cpp
    PetWidget.h
    PetWidget.cpp
    ChatPanel.h
    ChatPanel.cpp
    DeviceCard.h
    DeviceCard.cpp
    AiClient.h
    AiClient.cpp
    DeviceAgent.h
    DeviceAgent.cpp
    SensorAgent.h
    SensorAgent.cpp
    MotorAgent.h
    MotorAgent.cpp
    SensorAgent.h
    SensorAgent.cpp
    MotorAgent.h
    MotorAgent.cpp
    icon.rc
)
# 将 assets 目录复制到编译输出目录（方便程序找到 GIF）
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
target_link_libraries(DesktopAIPet PRIVATE Qt6::Core Qt6::Widgets Qt6::Network)

install(TARGETS DesktopAIPet
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
qt_generate_deploy_app_script(TARGET DesktopAIPet OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR)
install(SCRIPT ${deploy_script})
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Sql)   # 加 Sql

target_link_libraries(DesktopAIPet PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Sql)  # 加 Qt6::Sql
