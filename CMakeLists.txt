cmake_minimum_required(VERSION 3.19)
project(DesktopAIPet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Sql)

qt_add_executable(DesktopAIPet
    main.cpp
    icon.rc
    src/ui/DesktopAIPet.h      src/ui/DesktopAIPet.cpp
    src/ui/PetWidget.h         src/ui/PetWidget.cpp
    src/ui/ChatPanel.h         src/ui/ChatPanel.cpp
    src/ui/DeviceCard.h        src/ui/DeviceCard.cpp
    src/business/AiClient.h    src/business/AiClient.cpp
    src/business/TcpAgent.h
    src/business/TcpAgent.cpp
    src/business/TcpServer.h
    src/business/TcpServer.cpp
    src/business/TcpSensorAgent.h
    src/business/TcpSensorAgent.cpp
    src/business/TcpMotorAgent.h
    src/business/TcpMotorAgent.cpp



)

target_include_directories(DesktopAIPet PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(DesktopAIPet PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Sql)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
