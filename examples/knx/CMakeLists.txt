cmake_minimum_required(VERSION 3.16)
project(knx VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_standard_project_setup()

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Knx Network)

if(NOT TARGET Qt::Network)
    return()
endif()
add_subdirectory(discoverer)
add_subdirectory(knxproj)
add_subdirectory(tunnelclient)
add_subdirectory(router)
if(TARGET Qt::Widgets)
     add_subdirectory(knxeditor)
     add_subdirectory(group)
     add_subdirectory(device)
     add_subdirectory(feature)
endif()
