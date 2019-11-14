# Generated from device.pro.

cmake_minimum_required(VERSION 3.14)
project(device LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/knx/device")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Knx)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network)

add_qt_gui_executable(device
    deviceitem.cpp deviceitem.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)
target_include_directories(device PUBLIC
    .
)

target_link_libraries(device PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Knx
    Qt::Network
    Qt::Widgets
)

install(TARGETS device
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
