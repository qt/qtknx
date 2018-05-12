/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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
#include <QtKnx/qknxnamespace.h>

QT_BEGIN_NAMESPACE

namespace QKnx
{
    namespace NetIp
    {
        Q_KNX_EXPORT Q_NAMESPACE

        struct Constants
        {
            static constexpr const quint16 DefaultPort = 3671;
            static constexpr const char *MulticastAddress = "224.0.23.12";
        };

        enum class HostProtocol : quint8
        {
            Unknown = 0x00,
            UDP_IPv4 = 0x01,
            TCP_IPv4 = 0x02
        };
        Q_ENUM_NS(HostProtocol)
        Q_KNX_EXPORT bool isStructType(HostProtocol type);

        enum class ConnectionType : quint8
        {
            Unknown = 0x00,
            DeviceManagement = 0x03,
            Tunnel = 0x04,
            RemoteLogging = 0x06,
            RemoteConfiguration = 0x07,
            ObjectServer = 0x08
        };
        Q_ENUM_NS(ConnectionType)
        Q_KNX_EXPORT bool isStructType(ConnectionType type);

        enum class DescriptionType : quint8
        {
            Unknown = 0x00,
            DeviceInfo = 0x01,
            SupportedServiceFamilies = 0x02,
            IpConfiguration = 0x03,
            CurrentIpConfiguration = 0x04,
            KnxAddresses = 0x05,
            ManufacturerData = 0xfe,
            NotUsed = 0xff
        };
        Q_ENUM_NS(DescriptionType)
        Q_KNX_EXPORT bool isStructType(DescriptionType type);

        enum class ServiceFamily : quint8
        {
            Unknown = 0x00,
            Core = 0x02,
            DeviceManagement = 0x03,
            IpTunneling = 0x04,
            IpRouting = 0x05,
            RemoteLogging = 0x06,
            RemoteConfigAndDiagnosis = 0x07,
            ObjectServer = 0x08
        };
        Q_ENUM_NS(ServiceFamily)
        Q_KNX_EXPORT bool isServiceFamily(ServiceFamily family);

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
        Q_ENUM_NS(ServiceType)
        Q_KNX_EXPORT bool isServiceType(ServiceType type);

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
        Q_ENUM_NS(Error)

        enum class DeviceState : quint8
        {
            KnxFault = 0x00,
            IpFault = 0x01
        };
        Q_ENUM_NS(DeviceState)

        enum class TunnelLayer : quint8
        {
            Unknown = 0x00,
            Link = 0x02,
            Raw = 0x04,
            Busmonitor = 0x80
        };
        Q_ENUM_NS(TunnelLayer)
        Q_KNX_EXPORT bool isTunnelLayer(TunnelLayer layer);

        enum class ProgrammingMode : quint8
        {
            Inactive = 0x00,
            Active = 0x01,
            Unknown = 0xff
        };
        Q_ENUM_NS(ProgrammingMode)
        Q_KNX_EXPORT bool isProgrammingMode(ProgrammingMode mode);

        enum class AssignmentMethod : quint8
        {
            Unknown = 0x00,
            Manual = 0x01,
            BootP = 0x02,
            Dhcp = 0x04,
            AutoIp = 0x08
        };
        Q_ENUM_NS(AssignmentMethod)
        Q_DECLARE_FLAGS(AssignmentMethods, AssignmentMethod)
        Q_KNX_EXPORT bool isAssignmentMethod(AssignmentMethod method);

        enum class Capability : quint8
        {
            Unknown = 0x00,
            BootP = 0x01,
            Dhcp = 0x02,
            AutoIp = 0x04,
        };
        Q_ENUM_NS(Capability)
        Q_DECLARE_FLAGS(Capabilities, Capability)
        Q_KNX_EXPORT bool isCapability(Capability capability);

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
        Q_ENUM_NS(Timeout)

        namespace CemiServer
        {
            Q_KNX_EXPORT Q_NAMESPACE

            enum class ReturnCode : quint8
            {
                NoError = 0x00
            };
            Q_ENUM_NS(ReturnCode)

            enum class Error : quint8
            {
                Unspecified = 0x00,
                OutOfRange = 0x01,
                OutOfMaxRange = 0x02,
                OutOfMinRange = 0x03,
                Memory = 0x04,
                ReadOnly = 0x05,
                IllegalCommand = 0x06,
                NonExistingProperty = 0x07,
                TypeConflict = 0x08,
                PropertyIndexRangeError = 0x09,
                TemporaryNotWritable = 0x0a,
                None = 0xff  // Qt extension
            };
            Q_ENUM_NS(Error)
        }
    }
}
namespace QKnxNetIp = QKnx::NetIp;
namespace QKnxNetIpCemiServer = QKnxNetIp::CemiServer;

Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIp::AssignmentMethods)
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIp::Capabilities)

Q_DECLARE_TYPEINFO(QKnxNetIp::HostProtocol, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ConnectionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DescriptionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ServiceFamily, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ServiceType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Error, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::DeviceState, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::TunnelLayer, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::ProgrammingMode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::AssignmentMethod, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Capability, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIp::Timeout, Q_PRIMITIVE_TYPE);

Q_DECLARE_TYPEINFO(QKnxNetIpCemiServer::ReturnCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpCemiServer::Error, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
