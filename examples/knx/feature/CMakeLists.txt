cmake_minimum_required(VERSION 3.15.0)
project(feature LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Knx)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network)

add_qt_gui_executable(feature
    deviceitem.cpp deviceitem.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)
target_include_directories(feature PUBLIC
    .
)
target_link_libraries(feature PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Knx
    Qt::Network
    Qt::Widgets
)

install(TARGETS feature
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
