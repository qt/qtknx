cmake_minimum_required(VERSION 3.16)

project(qt_cmake_files)

find_package(Qt6 REQUIRED COMPONENTS Core Knx)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/test_Knx_includes.cpp"
    "#include <QKnxNetIpTunnel>\n"
    "#include <QtKnx/QKnxNetIpTunnel>\n"
    "int main() { return 0; }\n"
)

add_executable(test_Knx_includes "${CMAKE_CURRENT_BINARY_DIR}/test_Knx_includes.cpp")
target_link_libraries(test_Knx_includes PRIVATE Qt6::Knx)

add_test(NAME cmake_Knx_module_includes COMMAND test_Knx_includes)
