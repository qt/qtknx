TARGET = QtKnx

QT = core
CONFIG += c++11
QT_FOR_PRIVATE += network core-private

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

include(dpt/dpt.pri)
include(netip/netip.pri)
include(knxproj/knxproj.pri)

DEFINES += QT_NO_CAST_FROM_ASCII

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
    qknxtunnelframefactory.h \
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
    qknxnpdufactory_p2p.cpp \
    qknxinterfaceobjecttype.cpp \
    qknxinterfaceobjectproperty.cpp \
    qknxlocaldevicemanagementframe.cpp \
    qknxlocaldevicemanagementframefactory.cpp \
    qknxinterfaceobjectpropertydatatype.cpp \
    qknxnpdufactory_multicast.cpp \
    qknxtunnelframefactory.cpp \
    qknxnpdufactory_broadcast.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
