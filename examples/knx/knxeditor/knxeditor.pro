TEMPLATE = app
INCLUDEPATH += .
TARGET = knxeditor
CONFIG += c++11 console
QT += knx widgets network core

FORMS += mainwindow.ui \
         localdevicemanagement.ui

SOURCES += main.cpp \
           mainwindow.cpp \
           localdevicemanagement.cpp

HEADERS += mainwindow.h \
           localdevicemanagement.h

target.path = $$[QT_INSTALL_EXAMPLES]/knx/knxeditor
INSTALLS += target
