# Generated from knxproj.pro.

cmake_minimum_required(VERSION 3.14)
project(knxproj LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/knx/knxproj")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Knx)

add_executable(knxproj
    main.cpp
)
target_include_directories(knxproj PUBLIC
    .
)

target_link_libraries(knxproj PUBLIC
    Qt::Core
    Qt::Knx
)

install(TARGETS knxproj
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
