cmake_minimum_required(VERSION 3.16)
project(examples VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Knx Network Widgets)

qt_standard_project_setup()

if(NOT TARGET Qt::Knx)
    return()
endif()
if(NOT TARGET Qt::Network)
    return()
endif()
add_subdirectory(knx)
