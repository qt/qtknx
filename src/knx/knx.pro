TARGET = QtKnx

QT = core
CONFIG += c++11
QT_FOR_PRIVATE += network core-private

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

include(dpt/dpt.pri)
include(netip/netip.pri)

PUBLIC_HEADERS += \
    qknxglobal.h \
    qknxaddress.h \
    qknxadditionalinfo.h \
    qknxtraits.h \
    qknxcontrolfield.h \
    qknxextendedcontrolfield.h \
    qknxutils.h \
    qknxbytestore.h \
    qknxbytestoreref.h \
    qknxcemiframe.h \
    qknxtunnelframe.h \
    qknxcemi.h \
    qknxnpdu.h \
    qknxnpdufactory.h \
    qknxinterfaceobjecttype.h \
    qknxinterfaceobjectproperty.h \
    qknxlocaldevicemanagementframe.h \
    qknxlocaldevicemanagementframefactory.h \
    qknxinterfaceobjectpropertydatatype.h

PRIVATE_HEADERS +=

SOURCES += \
    qknxaddress.cpp \
    qknxadditionalinfo.cpp \
    qknxcontrolfield.cpp \
    qknxextendedcontrolfield.cpp \
    qknxbytestoreref.cpp \
    qknxbytestore.cpp \
    qknxcemiframe.cpp \
    qknxtunnelframe.cpp \
    qknxnpdu.cpp \
    qknxnpdufactory.cpp \
    qknxinterfaceobjecttype.cpp \
    qknxinterfaceobjectproperty.cpp \
    qknxlocaldevicemanagementframe.cpp \
    qknxlocaldevicemanagementframefactory.cpp \
    qknxinterfaceobjectpropertydatatype.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
