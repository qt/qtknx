TARGET = tst_qknxproject

CONFIG += testcase c++11
QT = core testlib knx knx-private

CONFIG -= app_bundle
RESOURCES += project.qrc
SOURCES += tst_qknxproject.cpp
