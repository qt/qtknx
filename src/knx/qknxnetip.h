/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIP_H
#define QKNXNETIP_H

#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxNetIp final
{
    enum class HostProtocol : quint8
    {
        Unknown = 0x00,
        IpV4_Udp = 0x01,
        IpV4_Tcp = 0x02
    };

    enum class ConnectionType : quint8
    {
        Unknown = 0x00,
        DeviceManagement = 0x03,
        Tunnel = 0x04,
        RemoteLogging = 0x06,
        RemoteConfiguration = 0x07,
        ObjectServer = 0x08
    };

    enum class DescriptionType : quint8
    {
        Unknown = 0x00,
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
        Unknown = 0x0000,

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
        KnxConnection = 0x27,
        TunnelingLayer = 0x29
    };

    enum class DeviceState : quint8
    {
        KnxFault = 0x00,
        IpFault = 0x01
    };

    static bool isStructType(QKnxNetIp::HostProtocol type)
    {
        switch (type) {
        case QKnxNetIp::HostProtocol::IpV4_Udp:
        case QKnxNetIp::HostProtocol::IpV4_Tcp:
            return true;
        case QKnxNetIp::HostProtocol::Unknown:
        default:
            break;
        }
        return false;
    }

    static bool isStructType(QKnxNetIp::ConnectionType type)
    {
        switch (type) {
        case QKnxNetIp::ConnectionType::DeviceManagement:
        case QKnxNetIp::ConnectionType::Tunnel:
        case QKnxNetIp::ConnectionType::RemoteLogging:
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
        case QKnxNetIp::ConnectionType::ObjectServer:
            return true;
        case QKnxNetIp::ConnectionType::Unknown:
        default:
            break;
        }
        return false;
    }

    static bool isStructType(QKnxNetIp::DescriptionType type)
    {
        switch (type) {
        case QKnxNetIp::DescriptionType::DeviceInfo:
        case QKnxNetIp::DescriptionType::SupportedServiceFamilies:
        case QKnxNetIp::DescriptionType::IpConfiguration:
        case QKnxNetIp::DescriptionType::CurrentIpConfiguration:
        case QKnxNetIp::DescriptionType::KnxAddresses:
        case QKnxNetIp::DescriptionType::ManufactorData:
            return true;
        case QKnxNetIp::DescriptionType::NotUsed:
        case QKnxNetIp::DescriptionType::Unknown:
        default:
            break;
        }
        return false;
    }

    static bool isFrameType(QKnxNetIp::ServiceType type)
    {
        switch (type) {
        case QKnxNetIp::ServiceType::SearchRequest:
        case QKnxNetIp::ServiceType::SearchResponse:
        case QKnxNetIp::ServiceType::DescriptionRequest:
        case QKnxNetIp::ServiceType::DescriptionResponse:
        case QKnxNetIp::ServiceType::ConnectRequest:
        case QKnxNetIp::ServiceType::ConnectResponse:
        case QKnxNetIp::ServiceType::ConnectionStateRequest:
        case QKnxNetIp::ServiceType::ConnectionStateResponse:
        case QKnxNetIp::ServiceType::DisconnectRequest:
        case QKnxNetIp::ServiceType::DisconnectResponse:
        case QKnxNetIp::ServiceType::DeviceConfigurationRequest:
        case QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge:
        case QKnxNetIp::ServiceType::TunnelingRequest:
        case QKnxNetIp::ServiceType::TunnelingAcknowledge:
        case QKnxNetIp::ServiceType::RoutingIndication:
        case QKnxNetIp::ServiceType::RoutingLostMessage:
        case QKnxNetIp::ServiceType::RoutingBusy:
            return true;
        case QKnxNetIp::ServiceType::Unknown:
        default:
            break;
        }
        return false;
    }
};
Q_DECLARE_TYPEINFO(QKnxNetIp::HostProtocol, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ConnectionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DescriptionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ServiceType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Error, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DeviceState, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
