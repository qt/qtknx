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

#include "qknxnetip.h"

QT_BEGIN_NAMESPACE

/*!
    \namespace QKnx::NetIp

    \inmodule QtKnx
    \brief The QKnx::NetIp namespace defines constants, methods, and enumerations
    related to KNXnet/IP communication.
*/

/*!
    \variable QKnx::NetIp::Constants::DefaultPort
    \brief The default KNXnet/IP port number used for UDP/IPv4.
*/

/*!
    \variable QKnx::NetIp::Constants::MulticastAddress
    \brief The default KNXnet/IP system setup multicast address.
*/

/*!
    \enum QKnx::NetIp::HostProtocol

    This enum describes the KNXnet/IP host protocol codes for an IP network.

    \value Unknown     The supported KNXnet/IP device host protocol is unknown.
    \value UDP_IPv4    The supported KNXnet/IP device host protocol is UDP/IPv4.
    \value TCP_IPv4    The supported KNXnet/IP device host protocol is TCP/IPv4.
*/


/*!
    \fn QKnx::NetIp::isStructType(QKnx::NetIp::HostProtocol type)

    Returns \c true if the specified \a type is a part of the \l HostProtocol
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isStructType(QKnxNetIp::HostProtocol type)
{
    switch (type) {
    case QKnxNetIp::HostProtocol::UDP_IPv4:
    case QKnxNetIp::HostProtocol::TCP_IPv4:
        return true;
    case QKnxNetIp::HostProtocol::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::ConnectionType

    This enum describes the supported KNXnet/IP connection types.

    \value Unknown
    The supported connection type is unknown.
    \value DeviceManagement
    The connection type used to configure a KNXnet/IP device.
    \value Tunnel
    The connection type used to send KNX telegrams between two KNXnet/IP devices.
    \value RemoteLogging
    The connection type used for configuration and data transfer with a remote
    logging server.
    \value RemoteConfiguration
    The connection type used for data transfer with a remote configuration server.
    \value ObjectServer
    The connection type used for configuration and data transfer with an object
    server in a KNXnet/IP device.
*/

/*!
    \fn QKnx::NetIp::isStructType(QKnx::NetIp::ConnectionType type)

    Returns \c true if the specified \a type is a part of the \l ConnectionType
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isStructType(QKnxNetIp::ConnectionType type)
{
    switch (type) {
    case QKnxNetIp::ConnectionType::DeviceManagement:
    case QKnxNetIp::ConnectionType::Tunnel:
    case QKnxNetIp::ConnectionType::RemoteLogging:
    case QKnxNetIp::ConnectionType::RemoteConfiguration:
    case QKnxNetIp::ConnectionType::ObjectServer:
        return true;
    case QKnxNetIp::ConnectionType::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::DescriptionType

    This enum contains values to self-describe a KNXnet/IP server device.

    \value Unknown                  An unknown description type.
    \value DeviceInfo               Device information, such as the KNX medium.
    \value SupportedServiceFamilies Service families supported by the device.
    \value IpConfiguration          IP configuration related device information.
    \value CurrentIpConfiguration   Current IP configuration of the device.
    \value KnxAddresses
           All individual addresses assigned to the device.
    \value ManufacturerData
           A structure for further data defined by a device manufacturer.
    \value NotUsed                  This value may not be used.
*/

/*!
    \fn QKnxNetIp::isStructType(QKnx::NetIp::DescriptionType type)

    Returns \c true if the specified \a type is a part of the \l DescriptionType
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isStructType(QKnxNetIp::DescriptionType type)
{
    switch (type) {
    case QKnxNetIp::DescriptionType::DeviceInfo:
    case QKnxNetIp::DescriptionType::SupportedServiceFamilies:
    case QKnxNetIp::DescriptionType::IpConfiguration:
    case QKnxNetIp::DescriptionType::CurrentIpConfiguration:
    case QKnxNetIp::DescriptionType::KnxAddresses:
    case QKnxNetIp::DescriptionType::ManufacturerData:
        return true;
    case QKnxNetIp::DescriptionType::NotUsed:
    case QKnxNetIp::DescriptionType::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::ServiceFamily

    This enum holds the service family that the communication is related to.
    The service family is the high octet of the values of the \l ServiceType
    enumeration.

    \value Unknown                      Unknown service family.
    \value Core                         Core service family.
    \value DeviceManagement             Device management service family.
    \value IpTunneling                  Tunneling service family.
    \value IpRouting                    Routing service family.
    \value RemoteLogging                Remote logging service family.
    \value RemoteConfigAndDiagnosis
           Remote configuration and diagnosis service family.
    \value ObjectServer                 Object server service family.
*/

/*!
    \fn QKnxNetIp::isServiceFamily(QKnx::NetIp::ServiceFamily family)

    Returns \c true if the specified \a family is a part of the \l ServiceFamily
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isServiceFamily(QKnxNetIp::ServiceFamily family)
{
    switch (family) {
    case QKnxNetIp::ServiceFamily::Core:
    case QKnxNetIp::ServiceFamily::DeviceManagement:
    case QKnxNetIp::ServiceFamily::IpTunneling:
    case QKnxNetIp::ServiceFamily::IpRouting:
    case QKnxNetIp::ServiceFamily::RemoteLogging:
    case QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis:
    case QKnxNetIp::ServiceFamily::ObjectServer:
        return true;
    case QKnxNetIp::ServiceFamily::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::ServiceType

    This enum describes KNXnet/IP frame service type identifiers.

    The KNXnet/IP service type identifier defines the kind of action to
    be performed and the type of the data payload contained in the
    KNXnet/IP body. The high octet of the KNXnet/IP service type identifier
    denotes the \l ServiceFamily.

    \value Unknown
           The service type of the KNXnet/IP frame is unknown.
    \value SearchRequest
           The service type sent by a KNXnet/IP client to search for available
           KNXnet/IP servers.
    \value SearchResponse
           The service type sent by a KNXnet/IP server when responding to a
           search request.
    \value DescriptionRequest
           The service type sent by a KNXnet/IP client to a KNXnet/IP server to
           retrieve information about capabilities and supported services.
    \value DescriptionResponse
           The service type sent by a KNXnet/IP server in response to a
           description request to provide information about the KNXnet/IP server
           implementation.
    \value ConnectRequest
           The service type sent by a KNXnet/IP client to establish a
           communication channel with a KNXnet/IP server.
    \value ConnectResponse
           The service type sent by a KNXnet/IP server in response to a connect
           request frame.
    \value ConnectionStateRequest
           The service type sent by a KNXnet/IP client requesting the connection
           state of an established connection with a KNXnet/IP server.
    \value ConnectionStateResponse
           The service type sent by a KNXnet/IP server when receiving a
           connection state request for an established connection.
    \value DisconnectRequest
           The service type sent by a KNXnet/IP device to terminate an
           established connection.
    \value DisconnectResponse
           The service type sent by a KNXnet/IP device in response to a
           disconnect request.
    \value DeviceConfigurationRequest
           The service type sent to read or write KNXnet/IP device configuration
           data.
    \value DeviceConfigurationAcknowledge
           The service type sent by a KNXnet/IP device to confirm the reception
           of a device configuration request.
    \value TunnelingRequest
           The service type used for sending and receiving single KNX frames
           between a KNXnet/IP client and server.
    \value TunnelingAcknowledge
           The service type sent by a KNXnet/IP client or server to confirm the
           reception of the tunneling request.
    \value TunnelingFeatureGet
           The service feature type used by the KNXnet/IP tunneling client to
           read the value of a feature from the KNXnet/IP tunneling server.
    \value TunnelingFeatureResponse
           The service feature type used by the KNXnet/IP tunneling server to
           respond to a feature get or set request by the KNXnet/IP tunneling
           client.
    \value TunnelingFeatureSet
           The service feature type used by the KNXnet/IP tunneling client to
           set the value of a feature of the KNXnet/IP tunneling server.
    \value TunnelingFeatureInfo
           The service feature type used by the KNXnet/IP tunneling server to
           inform the KNXnet/IP tunneling client on a value of an interface feature.
    \value RoutingIndication
           The service type used for sending KNX frames over IP networks.
    \value RoutingLostMessage
           The service type used to indicate that KNXnet/IP routing frames were
           lost.
    \value RoutingBusy
           The service type used to indicate that a KNXnet/IP router or KNX IP
           device received more datagrams than it can process.
*/

/*!
    \fn QKnxNetIp::isServiceType(QKnx::NetIp::ServiceType type)

    Returns \c true if the specified \a type is a part of the \l ServiceType
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isServiceType(QKnxNetIp::ServiceType type)
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
    case QKnxNetIp::ServiceType::TunnelingFeatureGet:
    case QKnxNetIp::ServiceType::TunnelingFeatureResponse:
    case QKnxNetIp::ServiceType::TunnelingFeatureSet:
    case QKnxNetIp::ServiceType::TunnelingFeatureInfo:
    case QKnxNetIp::ServiceType::RoutingIndication:
    case QKnxNetIp::ServiceType::RoutingLostMessage:
    case QKnxNetIp::ServiceType::RoutingBusy:
        return true;
    case QKnxNetIp::ServiceType::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::Error

    This enum describes the KNXnet/IP communication errors that can occur.

    \value None                 No error occurred.
    \value HostProtocolType
    The requested host protocol is not supported by the KNXnet/IP device.
    \value VersionNotSupported
    The requested protocol version is not supported by the KNXnet/IP device.
    \value SequenceNumber
    The received sequence number is out of sync.
    \value Error
    An undefined, possibly implementation specific error occurred.
    \value ConnectionId
    The KNXnet/IP server device cannot find an active data connection with the
    specified ID.
    \value ConnectionType
    The KNXnet/IP server device does not support the requested connection type.
    \value ConnectionOption
    The KNXnet/IP server device does not support one or more requested
    connection options.
    \value NoMoreConnections
    The KNXnet/IP server device cannot accept the new data connection because
    its maximum amount of concurrent connections is already used.
    \value NoMoreUniqueConnections
    The KNXnet/IP tunneling server could provide a connection (in contrast to
    \c NoMoreConnections) if only the KNXnet/IP tunneling address that would be
    assigned to the connection would be unique.
    \value DataConnection
    The KNXnet/IP server device detects an error concerning the data connection
    with the specified ID.
    \value KnxConnection
    The KNXnet/IP server device detects an error concerning the KNX connection
    with the specified ID.
    \value AuthorizationError
    The KNXnet/IP client is not authorized to use the requested individual
    address in the extended connection request information (CRI) structure.
    \value TunnelingLayer
    The requested tunneling layer is not supported by the KNXnet/IP server device.
    \value NoTunnelingAddress
    The address requested in the extended CRI structure is not a tunneling
    individual address.
    \value ConnectionInUse
    The individual address requested for this connection is already in use.
*/

/*!
    \enum QKnx::NetIp::DeviceState

    This enum describes the state information of a KNXnet/IP device.

    \value KnxFault     The state is set if the KNX network cannot be accessed.
    \value IpFault      The state is set if the IP network cannot be accessed.
*/

/*!
    \enum QKnx::NetIp::TunnelLayer

    This enum describes the KNX tunneling layer to establish.

    \value Unknown         The tunneling layer to establish is unknown.
    \value Link            Establish a data link layer tunnel to the KNX network.
    \value Raw             Establish a raw tunnel to the KNX network.
    \value Busmonitor      Establish a bus monitor tunnel to the KNX network.
*/

/*!
    \fn QKnxNetIp::isTunnelLayer(QKnx::NetIp::TunnelLayer layer)

    Returns \c true if the specified \a layer is a part of the \l TunnelLayer
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isTunnelLayer(QKnxNetIp::TunnelLayer layer)
{
    switch (layer) {
    case QKnxNetIp::TunnelLayer::Link:
    case QKnxNetIp::TunnelLayer::Raw:
    case QKnxNetIp::TunnelLayer::Busmonitor:
        return true;
    case QKnxNetIp::TunnelLayer::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::ProgrammingMode

    This enum describes the programming mode state of a KNX device.

    \value Inactive     The device is not in programming mode.
    \value Active       The device is in programming mode.
    \value Unknown      The programming state of the device is unknown.
*/

/*!
    Returns \c true if the specified \a mode is a part of the \l ProgrammingMode
    enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isProgrammingMode(ProgrammingMode mode)
{
    switch (mode) {
    case QKnxNetIp::ProgrammingMode::Inactive:
    case QKnxNetIp::ProgrammingMode::Active:
        return true;
    case QKnxNetIp::ProgrammingMode::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::AssignmentMethod

    This enum describes the enabled IP address assignment methods for setting
    the current IP address of a KNXnet/IP device.

    \value Unknown      The IP address assignment method is unknown.
    \value Manual       Manual IP address assignment.
    \value BootP        IP address assignment via BootP.
    \value Dhcp         IP address assignment via DHCP.
    \value AutoIp       IP address is self-assigned by the device.
*/

/*!
    Returns \c true if the specified \a method is a part of the
    \l AssignmentMethod enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isAssignmentMethod(AssignmentMethod method)
{
    switch (method) {
    case QKnxNetIp::AssignmentMethod::Manual:
    case QKnxNetIp::AssignmentMethod::BootP:
    case QKnxNetIp::AssignmentMethod::Dhcp:
    case QKnxNetIp::AssignmentMethod::AutoIp:
        return true;
    case QKnxNetIp::AssignmentMethod::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::Capability

    This enum describes the IP capabilities supported by the KNXnet/IP device.

    \value Unknown      The supported IP address assignment capability is unknown.
    \value BootP        The device supports IP address assignment via BootP.
    \value Dhcp         The device supports IP address assignment via DHCP.
    \value AutoIp       The device is capable of assigning itself a unicast IP
                        address in the range of 169.254.1.0 to 169.254.254.255.
*/

/*!
    Returns \c true if the specified \a capability is a part of the
    \l Capability enumeration; otherwise returns \c false.
*/
bool QKnxNetIp::isCapability(Capability capability)
{
    switch (capability) {
    case QKnxNetIp::Capability::BootP:
    case QKnxNetIp::Capability::Dhcp:
    case QKnxNetIp::Capability::AutoIp:
        return true;
    case QKnxNetIp::Capability::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::NetIp::Timeout

    This enum describes the timeouts used during KNXnet/IP communication.

    \value HeartbeatTimeout
    Timeout used between sending the next connection state request frame to the
    KNXnet/IP server.
    \value ConnectionAliveTimeout
    Timeout before sending a disconnect request to the KNXnet/IP client after
    the last correctly received message frame.
    \value SearchTimeout
    Timeout after sending a search request that the KNXnet/IP client will wait
    for search response frames from KNXnet/IP servers.
    \value DescriptionTimeout
    Timeout after sending a description request that the KNXnet/IP client will
    wait for description response frames from KNXnet/IP servers.
    \value ConnectRequestTimeout
    The KNXnet/IP client will wait for 10 seconds to receive a connect response
    frame from the KNXnet/IP server.
    \value ConnectionStateRequestTimeout
    The KNXnet/IP client will wait for 10 seconds for a connection state
    response from the KNXnet/IP server after sending a connection state request.
    \value DisconnectRequestTimeout
    The KNXnet/IP client will wait for 10 seconds to receive a disconnect
    response frame from the KNXnet/IP server.
    \value DeviceConfigurationRequestTimeout
    The KNXnet/IP client will wait for 10 seconds for a device configuration
    response from the KNXnet/IP server after sending a device configuration
    request.
    \value TunnelingRequestTimeout
    The KNXnet/IP client will wait for 1 second for a tunneling acknowledge
    response from the KNXnet/IP server after sending a tunneling request.
    \value RoutingBusyWaitTime
    The timeout used to empty the incoming queue of a KNXnet/IP router or KNX
    IP device if the number of received datagrams exceeds the number of frames
    that the device can actually process.
*/

/*!
    \namespace QKnx::NetIp::CemiServer

    \inmodule QtKnx
    \brief The QKnx::NetIp::CemiServer namespace defines enumerations related to
    KNXnet/IP communication with a cEMI server.

    The common external message interface (cEMI) message format is a generic
    structure for transmitting KNX frames between cEMI servers and cEMI clients
    independently of the medium.
*/

/*!
    \enum QKnx::NetIp::CemiServer::ReturnCode

    This enum describes the possible error codes returned by the remote cEMI
    server after cEMI function properties service requests.

    A return code larger than \c NoError indicates the negative result of the
    function.

    \value NoError The function was successfully executed. The return code
                   indicates the positive result of the function.
*/

/*!
    \enum QKnx::NetIp::CemiServer::Error

    This enum describes the possible error codes returned by the remote cEMI
    server after cEMI service management requests.

    \value Unspecified
           The error that occurred is unknown. Can occur in negative read or
           write confirmation frames.
    \value OutOfRange
           Denotes a general write error if \c OutOfMaxRange or \c OutOfMinRange
           are not applicable. Can occur in negative write confirmation frames.
    \value OutOfMaxRange
           The value to write was too high. Can occur in negative write
           confirmation frames.
    \value OutOfMinRange
           The value to write was too low. Can occur in negative write
           confirmation frames.
    \value Memory
           The memory cannot be written or can only be written with faults.
           Can occur in negative write confirmation frames.
    \value ReadOnly
           Write access to a read-only or a write protected property. Can occur
           in negative write confirmation frames.
    \value IllegalCommand
           The used command is not valid or not supported by the cEMI server.
    \value NonExistingProperty
           Read or write access to an non-existing property. Can occur in
           negative read or write confirmation frames.
    \value TypeConflict
           Write access with a wrong data type (datapoint length).
    \value PropertyIndexRangeError
           Read or write access to a non-existing property-array index. Can
           occur in negative read or write confirmation frames.
    \value TemporaryNotWritable
           The property exists but cannot be set to a new value at the time.
           Can occur in negative write confirmation frames.
    \value None     No error occurred. Please do not use.
*/

QT_END_NAMESPACE
