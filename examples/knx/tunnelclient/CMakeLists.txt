# Generated from tunnelclient.pro.

cmake_minimum_required(VERSION 3.14)
project(tunnelclient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/knx/tunnelclient")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Knx)
find_package(Qt6 COMPONENTS Network)

add_executable(tunnelclient
    main.cpp
)
target_include_directories(tunnelclient PUBLIC
    .
)

target_link_libraries(tunnelclient PUBLIC
    Qt::Core
    Qt::Knx
    Qt::Network
)

install(TARGETS tunnelclient
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
