TEMPLATE = app
CONFIG += c++11
INCLUDEPATH += .
TARGET = knxeditor
QT += knx widgets network core

FORMS += mainwindow.ui
SOURCES += main.cpp \
           mainwindow.cpp
HEADERS += mainwindow.h

target.path = $$[QT_INSTALL_EXAMPLES]/knx/knxeditor
INSTALLS += target
