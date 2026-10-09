cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(QtKnx
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt KNX Libraries"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core)

qt_internal_project_setup()

if(NOT QT_FEATURE_system_zlib)
    find_package(Qt6 ${PROJECT_VERSION} QUIET CONFIG OPTIONAL_COMPONENTS ZlibPrivate)
endif()

qt_build_repo()
