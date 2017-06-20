TEMPLATE = app
TARGET = tunnelclient

INCLUDEPATH += .
QT = core knx network
CONFIG += c++11 console

SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/knx/tunnelclient
INSTALLS += target
