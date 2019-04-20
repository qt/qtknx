TEMPLATE = app
TARGET = device
INCLUDEPATH += .

CONFIG += c++11
QT += knx widgets network core

FORMS += mainwindow.ui

HEADERS += deviceitem.h \
           mainwindow.h

SOURCES += deviceitem.cpp \
           main.cpp \
           mainwindow.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/knx/device
INSTALLS += target
