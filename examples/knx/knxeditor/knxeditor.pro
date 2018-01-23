TEMPLATE = app
CONFIG += c++11
INCLUDEPATH += .
TARGET = knxeditor
QT += knx widgets network core

FORMS += mainwindow.ui \
         localdevicemanagement.ui \
         tunneling.ui

SOURCES += main.cpp \
           mainwindow.cpp \
           localdevicemanagement.cpp \
           tunneling.cpp

HEADERS += mainwindow.h \
           localdevicemanagement.h \
           tunneling.h

target.path = $$[QT_INSTALL_EXAMPLES]/knx/knxeditor
INSTALLS += target
