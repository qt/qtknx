if(NOT TARGET Qt::Test)
    cmake_minimum_required(VERSION 3.15.0)
    project(QtKnxTests VERSION 6.0.0 LANGUAGES C CXX)
    find_package(Qt6 REQUIRED COMPONENTS BuildInternals Core Network Test Knx) # special case
    qt_set_up_standalone_tests_build()
endif()

qt_build_tests()
