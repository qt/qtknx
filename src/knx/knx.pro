TARGET = QtKnx

CONFIG += c++11
QT_FOR_PRIVATE = network

QMAKE_DOCS = $$PWD/doc/qtknx.qdocconf

PUBLIC_HEADERS += \
    qknxglobal.h \
    qknxaddress.h \
    qknxadditionalinfo.h \
    qknxtraits.h \
    qknxcontrolfield.h \
    qknxextendedcontrolfield.h \
    qknxnetipframe.h \
    qknxnetipframeheader.h \
    qknxnetiphpai.h \
    qknxnetipdevicedib.h \
    qknxnetipservicefamiliesdib.h \
    qknxnetipconfigdib.h \
    qknxnetipcurrentconfigdib.h \
    qknxnetipknxaddressesdib.h \
    qknxnetipmanufacturerdib.h \
    qknxnetip.h \
    qknxutils.h \
    qknxnetipcri.h \
    qknxnetipcrd.h \
    qknxnetipstruct.h \
    qknxbytestore.h \
    qknxnetippayload.h \
    qknxnetipstructheader.h \
    qknxnetipconnectionstaterequest.h \
    qknxnetipconnectionstateresponse.h \
    qknxnetipconnectrequest.h \
    qknxnetipconnectresponse.h \
    qknxnetipdescriptionrequest.h \
    qknxnetipdescriptionresponse.h \
    qknxnetipdeviceconfigurationacknowledge.h \
    qknxnetipdeviceconfigurationrequest.h \
    qknxnetipdisconnectrequest.h \
    qknxnetipdisconnectresponse.h \
    qknxnetiproutingbusy.h \
    qknxnetiproutingindication.h \
    qknxnetiproutinglostmessage.h \
    qknxnetipsearchrequest.h \
    qknxnetipsearchresponse.h \
    qknxnetiptunnelingacknowledge.h \
    qknxnetiptunnelingrequest.h \
    qknxnetippackage.h \
    qknxbytestoreref.h \
    qknxnetipstructref.h \
    qknxcemi.h

PRIVATE_HEADERS +=

SOURCES += \
    qknxaddress.cpp \
    qknxadditionalinfo.cpp \
    qknxcontrolfield.cpp \
    qknxextendedcontrolfield.cpp \
    qknxnetipframe.cpp \
    qknxnetipdevicedib.cpp \
    qknxnetiphpai.cpp \
    qknxnetipservicefamiliesdib.cpp \
    qknxnetipconfigdib.cpp \
    qknxnetipcurrentconfigdib.cpp \
    qknxnetipknxaddressesdib.cpp \
    qknxnetipmanufacturerdib.cpp \
    qknxnetipcri.cpp \
    qknxnetipcrd.cpp \
    qknxnetipstruct.cpp \
    qknxnetipframeheader.cpp \
    qknxnetipstructheader.cpp \
    qknxnetipconnectionstaterequest.cpp \
    qknxnetipconnectionstateresponse.cpp \
    qknxnetipconnectrequest.cpp \
    qknxnetipconnectresponse.cpp \
    qknxnetipdescriptionrequest.cpp \
    qknxnetipdescriptionresponse.cpp \
    qknxnetipdeviceconfigurationacknowledge.cpp \
    qknxnetipdeviceconfigurationrequest.cpp \
    qknxnetipdisconnectrequest.cpp \
    qknxnetipdisconnectresponse.cpp \
    qknxnetiproutingbusy.cpp \
    qknxnetiproutingindication.cpp \
    qknxnetiproutinglostmessage.cpp \
    qknxnetipsearchrequest.cpp \
    qknxnetipsearchresponse.cpp \
    qknxnetiptunnelingacknowledge.cpp \
    qknxnetiptunnelingrequest.cpp \
    qknxbytestoreref.cpp \
    qknxbytestore.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
