TARGET = QtKnx

QT = core
CONFIG += c++11
QT_FOR_PRIVATE += network core-private network-private

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

include(core/core.pri)
include(dpt/dpt.pri)
include(ets/ets.pri)
include(knxproj/knxproj.pri)
include(netip/netip.pri)
include(ssl/ssl.pri)

DEFINES += QT_NO_CAST_FROM_ASCII

PUBLIC_HEADERS += \
    qknxadditionalinfo.h \
    qknxaddress.h \
    qknxcontrolfield.h \
    qknxextendedcontrolfield.h \
    qtknxglobal.h \
    qknxinterfaceobjectproperty.h \
    qknxinterfaceobjectpropertydatatype.h \
    qknxinterfaceobjecttype.h \
    qknxlinklayerframe.h \
    qknxlinklayerframebuilder.h \
    qknxdevicemanagementframe.h \
    qknxdevicemanagementframefactory.h \
    qknxnamespace.h \
    qknxtpdu.h \
    qknxtraits.h \
    qknxutils.h

PRIVATE_HEADERS += \
    qtknxglobal_p.h \
    qknxtpdufactory_p.h

SOURCES += \
    qknxadditionalinfo.cpp \
    qknxaddress.cpp \
    qknxcontrolfield.cpp \
    qknxextendedcontrolfield.cpp \
    qknxinterfaceobjectproperty.cpp \
    qknxinterfaceobjectpropertydatatype.cpp \
    qknxinterfaceobjecttype.cpp \
    qknxlinklayerframe.cpp \
    qknxdevicemanagementframe.cpp \
    qknxdevicemanagementframefactory.cpp \
    qknxnamespace.cpp \
    qknxtpdu.cpp \
    qknxtpdufactory_broadcast.cpp \
    qknxtpdufactory_multicast.cpp \
    qknxtpdufactory_p2p.cpp \
    qknxlinklayerframebuilder.cpp


HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
