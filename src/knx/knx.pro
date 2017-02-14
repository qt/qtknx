TARGET = QtKnx

CONFIG += c++11
QT_FOR_PRIVATE = network

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

PUBLIC_HEADERS += \
    qknxglobal.h \
    qknxaddress.h \
    qknxadditionalinfo.h \
    qknxtypecheck.h \
    qknxcontrolfield.h \
    qknxextendedcontrolfield.h \
    qknxnetipframe.h \
    qknxnetipheader.h \
    qknxnetiphpai.h \
    qknxnetipstructure.h \
    qknxnetipdevicedib.h

PRIVATE_HEADERS +=

SOURCES += \
    qknxaddress.cpp \
    qknxadditionalinfo.cpp \
    qknxcontrolfield.cpp \
    qknxextendedcontrolfield.cpp \
    qknxnetipframe.cpp \
    qknxnetipstructure.cpp \
    qknxnetipdevicedib.cpp \
    qknxnetiphpai.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
