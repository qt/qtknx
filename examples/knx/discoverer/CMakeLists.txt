# Generated from discoverer.pro.

cmake_minimum_required(VERSION 3.14)
project(discoverer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/knx/discoverer")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Knx)
find_package(Qt6 COMPONENTS Network)

add_executable(discoverer
    main.cpp
)
target_include_directories(discoverer PUBLIC
    .
)

target_link_libraries(discoverer PUBLIC
    Qt::Core
    Qt::Knx
    Qt::Network
)

install(TARGETS discoverer
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
