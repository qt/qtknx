TEMPLATE = app
TARGET = router

INCLUDEPATH += .
QT = core knx network
CONFIG += c++11 console

SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/knx/router
INSTALLS += target
