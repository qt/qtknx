TEMPLATE = app
TARGET = group
INCLUDEPATH += .

CONFIG += c++11
QT += knx widgets network core

FORMS += mainwindow.ui

HEADERS += deviceitem.h \
           groupaddressvalidater.h \
           mainwindow.h

SOURCES += deviceitem.cpp \
           groupaddressvalidater.cpp \
           main.cpp \
           mainwindow.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/knx/group
INSTALLS += target
