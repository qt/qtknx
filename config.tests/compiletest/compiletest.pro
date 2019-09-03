 gcc:!clang:lessThan(QT_GCC_MAJOR_VERSION, 5) {
    SOURCES += main_less_5.cpp
} else {
    SOURCES += main.cpp
}
