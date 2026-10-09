cmake_minimum_required(VERSION 3.16)
project(tests VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Knx KnxPrivate Network Test)

qt_standard_project_setup()

if(NOT TARGET Qt6::Test)
    return()
endif()

add_subdirectory(auto)
