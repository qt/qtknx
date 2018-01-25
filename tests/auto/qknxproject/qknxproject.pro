TARGET = tst_qknxproject

QT = core testlib knx
CONFIG += testcase c++11

CONFIG -= app_bundle
RESOURCES += project.qrc
SOURCES += tst_qknxproject.cpp
