# Generated from knxeditor.pro.

cmake_minimum_required(VERSION 3.14)
project(knxeditor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/knx/knxeditor")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Knx)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network)

add_qt_gui_executable(knxeditor
    localdevicemanagement.cpp localdevicemanagement.h localdevicemanagement.ui
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    tunneling.cpp tunneling.h tunneling.ui
    tunnelingfeatures.cpp tunnelingfeatures.h tunnelingfeatures.ui
)
target_include_directories(knxeditor PUBLIC
    .
)

target_link_libraries(knxeditor PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Knx
    Qt::Network
    Qt::Widgets
)

install(TARGETS knxeditor
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
