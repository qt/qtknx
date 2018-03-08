/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXNETIP_H
#define QKNXNETIP_H

#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxNetIp final
{
    static constexpr const quint16 DefaultPort = 3671;
    static constexpr const char *MulticastAddress = "224.0.23.12";

    enum class HostProtocol : quint8
    {
        Unknown = 0x00,
        UDP_IPv4 = 0x01,
        TCP_IPv4 = 0x02
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
        NoMoreUniqueConnections = 0x25,
        DataConnection = 0x26,
        KnxConnection = 0x27,
        TunnelingLayer = 0x29
    };

    enum class DeviceState : quint8
    {
        KnxFault = 0x00,
        IpFault = 0x01
    };

    enum class TunnelingLayer : quint8
    {
        Unknown = 0x00,
        Link = 0x02,
        Raw = 0x04,
        Busmonitor = 0x80
    };

    static bool isTunnelingLayer(TunnelingLayer layer)
    {
        switch (layer) {
        case QKnxNetIp::TunnelingLayer::Link:
        case QKnxNetIp::TunnelingLayer::Raw:
        case QKnxNetIp::TunnelingLayer::Busmonitor:
            return true;
        default:
            break;
        }
        return false;
    }

    static bool isStructType(QKnxNetIp::HostProtocol type)
    {
        switch (type) {
        case QKnxNetIp::HostProtocol::UDP_IPv4:
        case QKnxNetIp::HostProtocol::TCP_IPv4:
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

    enum Timeout
    {
        HeartbeatTimeout = 60000,
        ConnectionAliveTimeout = 120000,

        // KNXnet/IP Core service time out in ms
        SearchTimeout = 3000,
        DescriptionTimeout = 3000,

        ConnectRequestTimeout = 10000,
        ConnectionStateRequestTimeout = 10000,
        DisconnectRequestTimeout = 10000,

        // KNXnet/IP Device Management service time out in ms
        DeviceConfigurationRequestTimeout = 10000,

        // KNXnet/IP Tunneling service time out in ms
        TunnelingRequestTimeout = 1000,

        // KNXnet/IP routing service
        RoutingBusyWaitTime = 100
    };
};
Q_DECLARE_TYPEINFO(QKnxNetIp::HostProtocol, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ConnectionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DescriptionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ServiceType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Error, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DeviceState, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Timeout, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
