TARGET = QtKnx

QT = core
CONFIG += c++11
QT_FOR_PRIVATE += network core-private

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

include(dpt/dpt.pri)
include(knxproj/knxproj.pri)
include(netip/netip.pri)
include(tlstm/tlstm.pri)

DEFINES += QT_NO_CAST_FROM_ASCII

PUBLIC_HEADERS += \
    qknxadditionalinfo.h \
    qknxaddress.h \
    qknxbytestore.h \
    qknxbytestoreref.h \
    qknxcemi.h \
    qknxcemiframe.h \
    qknxconnectioninfo.h \
    qknxcontrolfield.h \
    qknxdeviceconfiguration.h \
    qknxextendedcontrolfield.h \
    qknxglobal.h \
    qknxinterfaceobjectproperty.h \
    qknxinterfaceobjectpropertydatatype.h \
    qknxinterfaceobjecttype.h \
    qknxlinklayerdevice.h \
    qknxlocaldevicemanagementframe.h \
    qknxlocaldevicemanagementframefactory.h \
    qknxnamespace.h \
    qknxtpdu.h \
    qknxtpdufactory.h \
    qknxtraits.h \
    qknxtransportlayer.h \
    qknxtunnelframe.h \
    qknxtunnelframefactory.h \
    qknxutils.h \
    qknxgroupaddressinfos.h \
    qknxgroupaddressinfo.h

PRIVATE_HEADERS += \
    qknxlinklayerdevice_p.h \
    qknxtransportlayer_p.h

SOURCES += \
    qknxadditionalinfo.cpp \
    qknxaddress.cpp \
    qknxbytestore.cpp \
    qknxbytestoreref.cpp \
    qknxcemiframe.cpp \
    qknxconnectioninfo.cpp \
    qknxcontrolfield.cpp \
    qknxdeviceconfiguration.cpp \
    qknxextendedcontrolfield.cpp \
    qknxinterfaceobjectproperty.cpp \
    qknxinterfaceobjectpropertydatatype.cpp \
    qknxinterfaceobjecttype.cpp \
    qknxlinklayerdevice.cpp \
    qknxlocaldevicemanagementframe.cpp \
    qknxlocaldevicemanagementframefactory.cpp \
    qknxtpdu.cpp \
    qknxtpdufactory_broadcast.cpp \
    qknxtpdufactory_multicast.cpp \
    qknxtpdufactory_p2p.cpp \
    qknxtransportlayer.cpp \
    qknxtunnelframe.cpp \
    qknxtunnelframefactory.cpp \
    qknxgroupaddressinfos.cpp \
    qknxgroupaddressinfo.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
