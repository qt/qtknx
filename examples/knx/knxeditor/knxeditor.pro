TEMPLATE = app
CONFIG += c++11
INCLUDEPATH += .
TARGET = knxeditor
QT += knx widgets network core

FORMS += mainwindow.ui \
         localdevicemanagement.ui \
         tunneling.ui \
         tunnelingfeatures.ui

SOURCES += main.cpp \
           mainwindow.cpp \
           localdevicemanagement.cpp \
           tunneling.cpp \
           tunnelingfeatures.cpp

HEADERS += mainwindow.h \
           localdevicemanagement.h \
           tunneling.h \
           tunnelingfeatures.h

target.path = $$[QT_INSTALL_EXAMPLES]/knx/knxeditor
INSTALLS += target
