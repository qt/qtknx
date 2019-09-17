# special case begin
# Protect against case when ZLIB is found via QtGui in a static build,
# which means that we can't do a qt_find_package() here due to CMake
# complaining about not being able to make the ZLIB target global,
# because ZLIB was found in the QtGui directory scope (root level) and
# not in this directory scope.
if(BUILD_SHARED_LIBS)
    qt_find_package(ZLIB REQUIRED PROVIDED_TARGETS ZLIB::ZLIB)
endif()

add_qt_module(Knx
    SOURCES
# core
        core/qknxbytearray.cpp
        core/qknxbytearray.h
# dpt
        dpt/qknx1bit.cpp
        dpt/qknx1bit.h
        dpt/qknx1bitcontrolled.cpp
        dpt/qknx1bitcontrolled.h
        dpt/qknx1byte.cpp
        dpt/qknx1byte.h
        dpt/qknx2bitset.cpp
        dpt/qknx2bitset.h
        dpt/qknx2bytefloat.cpp
        dpt/qknx2bytefloat.h
        dpt/qknx2bytesignedvalue.cpp
        dpt/qknx2bytesignedvalue.h
        dpt/qknx2byteunsignedvalue.cpp
        dpt/qknx2byteunsignedvalue.h
        dpt/qknx32bitset.cpp
        dpt/qknx32bitset.h
        dpt/qknx3bitcontrolled.cpp
        dpt/qknx3bitcontrolled.h
        dpt/qknx4bytefloat.cpp
        dpt/qknx4bytefloat.h
        dpt/qknx4bytesignedvalue.cpp
        dpt/qknx4bytesignedvalue.h
        dpt/qknx4byteunsignedvalue.cpp
        dpt/qknx4byteunsignedvalue.h
        dpt/qknx8bitset.cpp
        dpt/qknx8bitset.h
        dpt/qknx8bitsignedvalue.cpp
        dpt/qknx8bitsignedvalue.h
        dpt/qknx8bitunsignedvalue.cpp
        dpt/qknx8bitunsignedvalue.h
        dpt/qknxchar.cpp
        dpt/qknxchar.h
        dpt/qknxcharstring.cpp
        dpt/qknxcharstring.h
        dpt/qknxdatapointtype.cpp
        dpt/qknxdatapointtype.h
        dpt/qknxdatapointtype_p.h
        dpt/qknxdatapointtypefactory.cpp
        dpt/qknxdatapointtypefactory.h
        dpt/qknxdatetime.cpp
        dpt/qknxdatetime.h
        dpt/qknxelectricalenergy.cpp
        dpt/qknxelectricalenergy.h
        dpt/qknxentranceaccess.cpp
        dpt/qknxentranceaccess.h
        dpt/qknxscene.cpp
        dpt/qknxscene.h
        dpt/qknxstatusmode3.cpp
        dpt/qknxstatusmode3.h
        dpt/qknxtime.cpp
        dpt/qknxtime.h
        dpt/qknxutf8string.cpp
        dpt/qknxutf8string.h
        dpt/qknxvarstring.cpp
        dpt/qknxvarstring.h
# ets
        ets/manufacturers.cpp
        ets/manufacturers.h
# knxproj
        knxproj/qknxbuildings.cpp
        knxproj/qknxbuildings_p.h
        knxproj/qknxdeviceinstance.cpp
        knxproj/qknxdeviceinstance_p.h
        knxproj/qknxgroupaddresses.cpp
        knxproj/qknxgroupaddresses_p.h
        knxproj/qknxgroupaddressinfo.cpp
        knxproj/qknxgroupaddressinfo.h
        knxproj/qknxgroupaddressinfos.cpp
        knxproj/qknxgroupaddressinfos.h
        knxproj/qknxinstallation.cpp
        knxproj/qknxinstallation_p.h
        knxproj/qknxprojectinformation.cpp
        knxproj/qknxprojectinformation_p.h
        knxproj/qknxprojectroot.cpp
        knxproj/qknxprojectroot_p.h
        knxproj/qknxprojectutils.cpp
        knxproj/qknxprojectutils_p.h
        knxproj/qknxtopology.cpp
        knxproj/qknxtopology_p.h
        knxproj/qzip.cpp
        knxproj/qzipreader_p.h
        knxproj/qzipwriter_p.h
# netip
        netip/qknxbuilderdata_p.h
        netip/qknxnetip.cpp
        netip/qknxnetip.h
        netip/qknxnetipconfigdib.cpp
        netip/qknxnetipconfigdib.h
        netip/qknxnetipconnectionheader.cpp
        netip/qknxnetipconnectionheader.h
        netip/qknxnetipconnectionstaterequest.cpp
        netip/qknxnetipconnectionstaterequest.h
        netip/qknxnetipconnectionstateresponse.cpp
        netip/qknxnetipconnectionstateresponse.h
        netip/qknxnetipconnectrequest.cpp
        netip/qknxnetipconnectrequest.h
        netip/qknxnetipconnectresponse.cpp
        netip/qknxnetipconnectresponse.h
        netip/qknxnetipcrd.cpp
        netip/qknxnetipcrd.h
        netip/qknxnetipcri.cpp
        netip/qknxnetipcri.h
        netip/qknxnetipcurrentconfigdib.cpp
        netip/qknxnetipcurrentconfigdib.h
        netip/qknxnetipdescriptionrequest.cpp
        netip/qknxnetipdescriptionrequest.h
        netip/qknxnetipdescriptionresponse.cpp
        netip/qknxnetipdescriptionresponse.h
        netip/qknxnetipdeviceconfigurationacknowledge.cpp
        netip/qknxnetipdeviceconfigurationacknowledge.h
        netip/qknxnetipdeviceconfigurationrequest.cpp
        netip/qknxnetipdeviceconfigurationrequest.h
        netip/qknxnetipdevicedib.cpp
        netip/qknxnetipdevicedib.h
        netip/qknxnetipdevicemanagement.cpp
        netip/qknxnetipdevicemanagement.h
        netip/qknxnetipdisconnectrequest.cpp
        netip/qknxnetipdisconnectrequest.h
        netip/qknxnetipdisconnectresponse.cpp
        netip/qknxnetipdisconnectresponse.h
        netip/qknxnetipendpointconnection.cpp
        netip/qknxnetipendpointconnection.h
        netip/qknxnetipendpointconnection_p.h
        netip/qknxnetipextendeddevicedib.cpp
        netip/qknxnetipextendeddevicedib.h
        netip/qknxnetipframe.cpp
        netip/qknxnetipframe.h
        netip/qknxnetipframeheader.cpp
        netip/qknxnetipframeheader.h
        netip/qknxnetiphpai.cpp
        netip/qknxnetiphpai.h
        netip/qknxnetipknxaddressesdib.cpp
        netip/qknxnetipknxaddressesdib.h
        netip/qknxnetipmanufacturerdib.cpp
        netip/qknxnetipmanufacturerdib.h
        netip/qknxnetiprouter.cpp
        netip/qknxnetiprouter.h
        netip/qknxnetiprouter_p.cpp
        netip/qknxnetiprouter_p.h
        netip/qknxnetiproutingbusy.cpp
        netip/qknxnetiproutingbusy.h
        netip/qknxnetiproutingindication.cpp
        netip/qknxnetiproutingindication.h
        netip/qknxnetiproutinglostmessage.cpp
        netip/qknxnetiproutinglostmessage.h
        netip/qknxnetiproutingsystembroadcast.cpp
        netip/qknxnetiproutingsystembroadcast.h
        netip/qknxnetipsearchrequest.cpp
        netip/qknxnetipsearchrequest.h
        netip/qknxnetipsearchresponse.cpp
        netip/qknxnetipsearchresponse.h
        netip/qknxnetipsecureconfiguration.cpp
        netip/qknxnetipsecureconfiguration.h
        netip/qknxnetipsecureconfiguration_p.h
        netip/qknxnetipsecuredservicefamiliesdib.cpp
        netip/qknxnetipsecuredservicefamiliesdib.h
        netip/qknxnetipsecurewrapper.cpp
        netip/qknxnetipsecurewrapper.h
        netip/qknxnetipserverdescriptionagent.cpp
        netip/qknxnetipserverdescriptionagent.h
        netip/qknxnetipserverdescriptionagent_p.h
        netip/qknxnetipserverdiscoveryagent.cpp
        netip/qknxnetipserverdiscoveryagent.h
        netip/qknxnetipserverdiscoveryagent_p.cpp
        netip/qknxnetipserverdiscoveryagent_p.h
        netip/qknxnetipserverinfo.cpp
        netip/qknxnetipserverinfo.h
        netip/qknxnetipserverinfo_p.h
        netip/qknxnetipservicefamiliesdib.cpp
        netip/qknxnetipservicefamiliesdib.h
        netip/qknxnetipsessionauthenticate.cpp
        netip/qknxnetipsessionauthenticate.h
        netip/qknxnetipsessionrequest.cpp
        netip/qknxnetipsessionrequest.h
        netip/qknxnetipsessionresponse.cpp
        netip/qknxnetipsessionresponse.h
        netip/qknxnetipsessionstatus.cpp
        netip/qknxnetipsessionstatus.h
        netip/qknxnetipsrp.cpp
        netip/qknxnetipsrp.h
        netip/qknxnetipstruct.cpp
        netip/qknxnetipstruct.h
        netip/qknxnetipstructheader.cpp
        netip/qknxnetipstructheader.h
        netip/qknxnetiptestrouter_p.h
        netip/qknxnetiptimernotify.cpp
        netip/qknxnetiptimernotify.h
        netip/qknxnetiptunnel.cpp
        netip/qknxnetiptunnel.h
        netip/qknxnetiptunnelingacknowledge.cpp
        netip/qknxnetiptunnelingacknowledge.h
        netip/qknxnetiptunnelingfeatureget.cpp
        netip/qknxnetiptunnelingfeatureget.h
        netip/qknxnetiptunnelingfeatureinfo.cpp
        netip/qknxnetiptunnelingfeatureinfo.h
        netip/qknxnetiptunnelingfeatureresponse.cpp
        netip/qknxnetiptunnelingfeatureresponse.h
        netip/qknxnetiptunnelingfeatureset.cpp
        netip/qknxnetiptunnelingfeatureset.h
        netip/qknxnetiptunnelinginfodib.cpp
        netip/qknxnetiptunnelinginfodib.h
        netip/qknxnetiptunnelingrequest.cpp
        netip/qknxnetiptunnelingrequest.h
# ssl
        ssl/qknxcryptographicengine.cpp
        ssl/qknxcryptographicengine.h
        ssl/qknxkeyring.cpp
        ssl/qknxkeyring_p.h
        ssl/qknxsecurekey.cpp
        ssl/qknxsecurekey.h
        ssl/qknxssl_openssl.cpp
        ssl/qknxssl_p.h
# common
        qknxadditionalinfo.cpp
        qknxadditionalinfo.h
        qknxaddress.cpp
        qknxaddress.h
        qknxcontrolfield.cpp
        qknxcontrolfield.h
        qknxdevicemanagementframe.cpp
        qknxdevicemanagementframe.h
        qknxdevicemanagementframefactory.cpp
        qknxdevicemanagementframefactory.h
        qknxextendedcontrolfield.cpp
        qknxextendedcontrolfield.h
        qknxinterfaceobjectproperty.cpp
        qknxinterfaceobjectproperty.h
        qknxinterfaceobjectpropertydatatype.cpp
        qknxinterfaceobjectpropertydatatype.h
        qknxinterfaceobjecttype.cpp
        qknxinterfaceobjecttype.h
        qknxlinklayerframe.cpp
        qknxlinklayerframe.h
        qknxlinklayerframebuilder.cpp
        qknxlinklayerframebuilder.h
        qknxnamespace.cpp
        qknxnamespace.h
        qknxtpdu.cpp
        qknxtpdu.h
        qknxtpdufactory_broadcast.cpp
        qknxtpdufactory_multicast.cpp
        qknxtpdufactory_p.h
        qknxtpdufactory_p2p.cpp
        qknxtraits.h
        qknxutils.h
        qtknxglobal.h
        qtknxglobal_p.h
# special case end
    DEFINES
        QT_NO_CAST_FROM_ASCII
    INCLUDE_DIRECTORIES
        core
        dpt
        ets
        knxproj
        netip
    LIBRARIES
        Qt::CorePrivate
        Qt::Network
        Qt::NetworkPrivate
        ZLIB::ZLIB
    PUBLIC_LIBRARIES
        Qt::Core
)

#### Keys ignored in scope 1:.:.:knx.pro:<TRUE>:
# _LOADED = "qt_module"

## Scopes:
#####################################################################

# special case begin
extend_target(Knx CONDITION QT_FEATURE_opensslv11
    SOURCES
        ssl/qsslsocket_openssl11_symbols_p.h
        ssl/qsslsocket_openssl_symbols.cpp
        ssl/qsslsocket_openssl_symbols_p.h
    DEFINES
        OPENSSL_API_COMPAT=0x10100000L
)
# special case end

extend_target(Knx CONDITION QT_FEATURE_openssl_linked AND QT_FEATURE_opensslv11
    LIBRARIES
        OpenSSL::SSL
)

# special case begin
#extend_target(Knx CONDITION QT_FEATURE_opensslv11 AND NOT QT_FEATURE_openssl_linked
#    LIBRARIES
#        OpenSSL::SSL_nolink
#)
# special case end

extend_target(Knx CONDITION QT_FEATURE_opensslv11 AND WIN32
    LIBRARIES
        crypt32
)
add_qt_docs(
    doc/qtknx.qdocconf
)
