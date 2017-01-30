TARGET = QtKnx

CONFIG += c++11
QT_FOR_PRIVATE = network

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

PUBLIC_HEADERS += \
    qknxglobal.h \
    qknxaddress.h \
    qknxadditionalinfo.h \
    qknxtypecheck.h \
    qknxcontrolfield1.h \
    qknxcontrolfield2.h

PRIVATE_HEADERS +=

SOURCES += \
    qknxaddress.cpp \
    qknxadditionalinfo.cpp \
    qknxcontrolfield1.cpp \
    qknxcontrolfield2.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
