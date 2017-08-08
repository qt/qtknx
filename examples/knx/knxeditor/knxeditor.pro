TEMPLATE = app
INCLUDEPATH += .
TARGET = knxeditor
CONFIG += c++11 console
QT += knx widgets network core

FORMS += mainwindow.ui
SOURCES += main.cpp \
           mainwindow.cpp
HEADERS += mainwindow.h

target.path = $$[QT_INSTALL_EXAMPLES]/knx/knxeditor
INSTALLS += target
