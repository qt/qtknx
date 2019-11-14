# Generated from router.pro.

cmake_minimum_required(VERSION 3.14)
project(router LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/knx/router")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Knx)
find_package(Qt6 COMPONENTS Network)

add_executable(router
    main.cpp
)
target_include_directories(router PUBLIC
    .
)

target_link_libraries(router PUBLIC
    Qt::Core
    Qt::Knx
    Qt::Network
)

install(TARGETS router
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
