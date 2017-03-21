/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxNetIp final
{
    enum class HostProtocolCode : quint8
    {
        IpV4_Udp = 0x01,
        IpV4_Tcp = 0x02
    };

    enum class ConnectionTypeCode : quint8
    {
        DeviceManagementConnection = 0x03,
        TunnelConnection = 0x04,
        RemoteLoggingConnection = 0x06,
        RemoteConfigurationConnection = 0x07,
        ObjectServerConnection = 0x08
    };

    enum class DescriptionTypeCode : quint8
    {
        DeviceInfo = 0x01,
        SupportedServiceFamilies = 0x02,
        IpConfiguration = 0x03,
        CurrentIpConfiguration = 0x04,
        KnxAddresses = 0x05,
        ManufactorData = 0xfe,
        NotUsed = 0xff
    };

    enum class ServiceType : quint16
    {
        // KNXnet/IP Core service type identifiers
        SearchRequest = 0x0201,
        SearchResponse = 0x0202,
        DescriptionRequest = 0x0203,
        DescriptionResponse = 0x0204,
        ConnectRequest = 0x0205,
        ConnectResponse = 0x0206,
        ConnectionStateRequest = 0x0207,
        ConnectionStateResponse = 0x0208,
        DisconnectRequest = 0x0209,
        DisconnectResponse = 0x020a,

        // KNXnet/IP Device Management service type identifiers
        DeviceConfigurationRequest = 0x0310,
        DeviceConfigurationAcknowledge = 0x0311,

        // KNXnet/IP Tunneling service type identifiers
        TunnelingRequest = 0x0420,
        TunnelingAcknowledge = 0x0421,

        // KNXnet/IP Routing service type identifier
        RoutingIndication = 0x0530,
        RoutingLostMessage = 0x0531,
        RoutingBusy = 0x0352
    };

    enum class Error : quint8
    {
        None = 0x00,
        ConnectionId = 0x21,
        ConnectionType = 0x22,
        ConnectionOption = 0x23,
        NoMoreConnections = 0x24,
        DataConnection = 0x26,
        KnxConnection = 0x27
    };
};
Q_DECLARE_TYPEINFO(QKnxNetIp::HostProtocolCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ConnectionTypeCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DescriptionTypeCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ServiceType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Error, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
