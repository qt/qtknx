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

#include "qknxinterfaceobjectproperty.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxInterfaceObjectProperty

    \inmodule QtKnx
    \ingroup qtknx-device-management

    \brief The QKnxInterfaceObjectProperty class holds the properties of a KNX
    interface object.

    A KNX interface object holds information about device functionality.
    This class represents the properties of interface objects of the type
    \l QKnxInterfaceObjectType::System. Each property stores a particular
    functionality. \l General properties are common to all KNX interface
    objects.

    \sa {Qt KNX Device Management Classes}
*/

/*!
    \typedef QKnxInterfaceObjectProperty::AssociationTable

    A synonym for QKnxInterfaceObjectProperty::General.
*/

/*!
    \typedef QKnxInterfaceObjectProperty::FileServer

    A synonym for QKnxInterfaceObjectProperty::General.
*/

/*!
    \typedef QKnxInterfaceObjectProperty::InterfaceProgram

    A synonym for QKnxInterfaceObjectProperty::General.
*/

/*!
    \typedef QKnxInterfaceObjectProperty::KnxObjectAssociationTable

    A synonym for QKnxInterfaceObjectProperty::General.
*/

/*!
    \enum QKnxInterfaceObjectProperty::General
    This enum holds the properties that are common to all KNX interface objects.

    \value ObjectType
           The type of the KNX interface object.
           One of \l QKnxInterfaceObjectType::System.
    \value ObjectName
           The name of the KNX interface object.
    \value Semaphor
           A semaphor
    \value GroupObjectReference
           A reference to a group object that this property is relative to,
           according to the \c <ObjectReference>.<Property> pattern.
    \value LoadStateControl
           A load state control
    \value RunStateControl
           A run state control
    \value TableReference
           A reference to a table.
    \value ServiceControl
           A service control
    \value FirmwareRevision
           The revision number of the firmware installed on the device.
    \value ServicesSupported
           The services supported by the device.
    \value SerialNumber
           The serial number of the device.
    \value ManufacturerId
           The ID of the manufacturer of the device.
    \value ProgramVersion
           The program version.
    \value DeviceControl
           A device control
    \value OrderInfo
           Order information
    \value PeiType
           The Physical External Interface (PEI) type of the interface between
           an application module and a communication module.
    \value PortConfiguration
           Port configuration
    \value PollGroupSettings
           Poll group settings
    \value ManufacturerData
           Additional data defined by the manufacturer of the device.
    \value Enable
           Enable
    \value Description
           A description
    \value File
           A file
    \value Table
           A table
    \value Enrol
           Enrollment information.
    \value Version
           A version number.
    \value GroupObjectLink
    \value McbTable
           An MCB table
    \value ErrorCode
           An error code.
    \value ObjectIndex
           The index of the object.
    \value DownloadCounter
           A download counter
*/

/*!
    \enum QKnxInterfaceObjectProperty::Device
    This enum describes the properties specific to device interface objects.

    \value RoutingCount
    \value MaxRetryCount
    \value ErrorFlags
    \value ProgMode
    \value ProductId
    \value MaxApduLengthDevice
    \value SubnetAddress
    \value DeviceAddress
    \value PbConfig
    \value AddressReport
    \value AddressCheck
    \value ObjectValue
    \value ObjectLink
    \value Application
    \value Parameter
    \value ObjectAddress
    \value PsuType
    \value PsuStatus
    \value PsuEnable
    \value DomainAddress
    \value IoList
    \value MgtDescriptor01
    \value PL110Parameter
    \value RfRepeatCounter
    \value ReceiveBlockTable
    \value RandomPauseTable
    \value ReceiveBlockNumber
    \value HardwareType
    \value RetransmitterNumber
    \value SerialNumberTable
    \value BibatMasterAddress
    \value RfDomainAddressDevice
    \value DeviceDescriptor
    \value MeteringFilterTable
    \value GroupTelegramRateLimitTimeBase
    \value GroupTelegramRateLimitNumberOfTelegrams
    \value Channel01Parameter
    \value Channel02Parameter
    \value Channel03Parameter
    \value Channel04Parameter
    \value Channel05Parameter
    \value Channel06Parameter
    \value Channel07Parameter
    \value Channel08Parameter
    \value Channel09Parameter
    \value Channel10Parameter
    \value Channel11Parameter
    \value Channel12Parameter
    \value Channel13Parameter
    \value Channel14Parameter
    \value Channel15Parameter
    \value Channel16Parameter
    \value Channel17Parameter
    \value Channel18Parameter
    \value Channel19Parameter
    \value Channel20Parameter
    \value Channel21Parameter
    \value Channel22Parameter
    \value Channel23Parameter
    \value Channel24Parameter
    \value Channel25Parameter
    \value Channel26Parameter
    \value Channel27Parameter
    \value Channel28Parameter
    \value Channel29Parameter
    \value Channel30Parameter
    \value Channel31Parameter
    \value Channel32Parameter
*/

/*!
    \enum QKnxInterfaceObjectProperty::GroupAddressTable
    This enum describes the properties specific to group address table interface
    objects.

    \value ExtendedFrameFormat
           The group address table contains extended addresses, which means that
           interface objects are referenced using the format
           \c <ObjectType>.<InstanceNumber>.
    \value AddressTable1
    \value GroupResponserTable
*/

/*!
    \enum QKnxInterfaceObjectProperty::ApplicationProgram
    This enum describes the properties specific to application program interface
    objects.

    \value ParameterReference
           A reference to a parameter.
*/

/*!
    \enum QKnxInterfaceObjectProperty::Router
    This enum describes the properties specific to router interface objects.

    \value LineStatus
            This enum value has been deprecated. Use \l MediumStatus instead.
    \value MediumStatus
            The connection state of the TP1 connection for an IP/TP1 KNXnet/IP
            router at the secondary side. For a KNXnet/IP tunneling server on
            TP1, this is the connection state of the TP1 connection.
    \value MainLcConfig
    \value SubLcConfig
    \value MainLcGroupConfig
    \value SubLcGroupConfig
    \value RouteTableControl
    \value CouplerServerControl
    \value MaxRouterApduLength
    \value L2CouplerType
    \value Medium
    \value FilterTableUse
    \value RfEnableSbc
*/

/*!
    \enum QKnxInterfaceObjectProperty::LteAddressRoutingTable
    This enum describes the properties specific to Logical Tag Extended (LTE)
    address routing table interface objects.

    \value LteRouteSelect
           The selected route.
    \value LteRouteTable
           Available routes.
*/

/*!
    \enum QKnxInterfaceObjectProperty::CemiServer
    This enum describes the properties specific to Common External Message
    Interface (cEMI) server interface objects.

    \value MediumType
           The type of the medium used for the connection.
    \value CommonMode
    \value MediumAvailability
           The availability of the medium used for the connection.
    \value AdditionalInfoTypes
    \value TimeBase
    \value TransparencyModeEnabled
    \value BiBatNextBlock
    \value RfModeSelect
           RF mode selection.
    \value RfModeSupport
           RF mode support.
    \value RfFilteringModeSelectCemiServer
    \value RfFilteringModeSupport
    \value MaxInterfaceApduLength
            The management server (MaS) property holds the maximum length of
            the APDU that it supports for conveying a cEMI message from a cEMI
            client over the KNXnet/IP tunneling interface to the field level
            medium (e.g. KNX TP, KNX PL, KNX RF).
    \value MaxLocalApduLength
            The management server property holds the maximum length of the
            APDU that it supports for conveying a cEMI message from a cEMI
            client over the KNXnet/IP tunneling interface to its own cEMI
            device management interface.
*/

/*!
    \enum QKnxInterfaceObjectProperty::GroupObjectTable
    This enum describes the properties specific to group object table interface
    objects.

    \value GroupObjectTableProperty
    \value ExtendedGroupObjectReference
*/

/*!
    \enum QKnxInterfaceObjectProperty::PollingMaster
    This enum describes the properties specific to polling master interface
    objects.

    \value PollingState
           Polling state.
    \value PollingSlaveAddress
           The address of the polling slave.
    \value PollingCycle
           Polling cycle.
*/

/*!
    \enum QKnxInterfaceObjectProperty::KnxNetIpParameter
    This enum describes the properties specific to KNXnet/IP parameter interface
    objects.

    \value ProjectInstallationId
    \value KnxIndividualAddress
           The individual address that uniquely identifies a device in the
           KNXnet/IP network.
    \value AdditionalIndividualAddresses
           Additional individual addresses.
    \value CurrentIpAssignmentMethod
           The method used to assign the current IP address of the device.
    \value IpAssignmentMethod
           The method used to assign IP addresses.
    \value IpCapabilities
           The capabilities of the IP network.
    \value CurrentIpAddress
           The current IP address.
    \value CurrentSubnetMask
           The current subnet mask.
    \value CurrentDefaultGateway
           The current default gateway.
    \value IpAddress
           The IP address.
    \value SubnetMask
           The subnet mask.
    \value DefaultGateway
           The default gateway.
    \value DhcpBootpServer
           The DHCP boot server.
    \value MacAddress
           The media access control (MAC) address of the device.
    \value SystemSetupMulticastAddress
           The system setup multicast address.
    \value RoutingMulticastAddress
           The routing multicast address.
    \value Ttl
    \value KnxNetIpDeviceCapabilities
           The capabilities of the KNXnet/IP device.
    \value KnxNetIpDeviceState
           The state of the KNXnet/IP device.
    \value KnxNetIpRoutingCapabilities
           The routing capabilities of the KNXnet/IP network.
    \value PriorityFifoEnabled
    \value QueueOverflowToIp
    \value QueueOverflowToKnx
    \value MsgTransmitToIp
    \value MsgTransmitToKnx
    \value FriendlyName
    \value RoutingBusyWaitTime
    \value TunnelingAddresses
            Indicates which individual addresses the management server (MaS)
            will use for tunneling connections from the set of the additional
            individual addresses and its own individual address.
    \value BackboneKey
    \value DeviceAuthenticationCode
    \value PasswordHashes
    \value SecuredServiceFamilies
    \value MulticastLatencyTolerance
    \value SyncLatencyFraction
    \value TunnellingUsers
*/

/*!
    \enum QKnxInterfaceObjectProperty::Security
    This enum describes the properties specific to security interface objects.

    \value SecurityMode
    \value P2pKeyTable
    \value GroupKeyTable
    \value SecurityIndividualAddressTable
    \value SecurityFailuresLog
    \value SkiTool
    \value SecurityReport
    \value SecurityReportControl
    \value SequenceNumberSending
    \value ZoneKeysTable
    \value GoSecurityFlags
*/

/*!
    \enum QKnxInterfaceObjectProperty::RfMedium
    This enum describes the properties specific to RF medium interface objects.

    \value RfMultiType
    \value RfMultiPhysicalFeatures
    \value RfMultiCallChannel
    \value RfMultiObjectLink
    \value RfMultiExtendedGroupAddressRepeated
    \value RfDomainAddressRfMedium
    \value RfRetransmitter
    \value RfSecurityReportControl
    \value RfFilteringModeSelectRfMedium
    \value RfBidirTimeout
    \value RfDiagSaFilterTable
    \value RfDiagQualityTable
    \value RfDiagProbe
    \value TransmissionMode
    \value ReceptionMode
    \value TestSignal
    \value FastAcknowledge
    \value FastAcknowledgeActivate
*/

/*!
    \fn qint16 QKnxInterfaceObjectProperty::operator qint16() const

    Returns the property as a 16-bit unsigned integer.
*/

/*!
    \fn QKnxInterfaceObjectProperty::QKnxInterfaceObjectProperty(qint16 property)

    Creates the KNX interface object property \a property.
*/

/*!
    Returns \c true if \a property is listed in this class.
*/
bool QKnxInterfaceObjectProperty::isProperty(QKnxInterfaceObjectProperty property)
{
    return property < QKnxInterfaceObjectProperty::Invalid;
}

/*!
    Returns \c true if \a property is a \l General property; \c false
    otherwise.
*/
bool QKnxInterfaceObjectProperty::isGeneralProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::General::ObjectType:
    case QKnxInterfaceObjectProperty::General::ObjectName:
    case QKnxInterfaceObjectProperty::General::Semaphor:
    case QKnxInterfaceObjectProperty::General::GroupObjectReference:
    case QKnxInterfaceObjectProperty::General::LoadStateControl:
    case QKnxInterfaceObjectProperty::General::RunStateControl:
    case QKnxInterfaceObjectProperty::General::TableReference:
    case QKnxInterfaceObjectProperty::General::ServiceControl:
    case QKnxInterfaceObjectProperty::General::FirmwareRevision:
    case QKnxInterfaceObjectProperty::General::ServicesSupported:
    case QKnxInterfaceObjectProperty::General::SerialNumber:
    case QKnxInterfaceObjectProperty::General::ManufacturerId:
    case QKnxInterfaceObjectProperty::General::ProgramVersion:
    case QKnxInterfaceObjectProperty::General::DeviceControl:
    case QKnxInterfaceObjectProperty::General::OrderInfo:
    case QKnxInterfaceObjectProperty::General::PeiType:
    case QKnxInterfaceObjectProperty::General::PortConfiguration:
    case QKnxInterfaceObjectProperty::General::PollGroupSettings:
    case QKnxInterfaceObjectProperty::General::ManufacturerData:
    case QKnxInterfaceObjectProperty::General::Enable:
    case QKnxInterfaceObjectProperty::General::Description:
    case QKnxInterfaceObjectProperty::General::File:
    case QKnxInterfaceObjectProperty::General::Table:
    case QKnxInterfaceObjectProperty::General::Enrol:
    case QKnxInterfaceObjectProperty::General::Version:
    case QKnxInterfaceObjectProperty::General::GroupObjectLink:
    case QKnxInterfaceObjectProperty::General::McbTable:
    case QKnxInterfaceObjectProperty::General::ErrorCode:
    case QKnxInterfaceObjectProperty::General::ObjectIndex:
    case QKnxInterfaceObjectProperty::General::DownloadCounter:
        return true;
    };
    return false;
}

/*!
    Returns \c true if \a property is a dedicated \l Device property or a
    \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isDeviceProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::Device::RoutingCount:
    case QKnxInterfaceObjectProperty::Device::MaxRetryCount:
    case QKnxInterfaceObjectProperty::Device::ErrorFlags:
    case QKnxInterfaceObjectProperty::Device::ProgMode:
    case QKnxInterfaceObjectProperty::Device::ProductId:
    case QKnxInterfaceObjectProperty::Device::MaxApduLengthDevice:
    case QKnxInterfaceObjectProperty::Device::SubnetAddress:
    case QKnxInterfaceObjectProperty::Device::DeviceAddress:
    case QKnxInterfaceObjectProperty::Device::PbConfig:
    case QKnxInterfaceObjectProperty::Device::AddressReport:
    case QKnxInterfaceObjectProperty::Device::AddressCheck:
    case QKnxInterfaceObjectProperty::Device::ObjectValue:
    case QKnxInterfaceObjectProperty::Device::ObjectLink:
    case QKnxInterfaceObjectProperty::Device::Application:
    case QKnxInterfaceObjectProperty::Device::Parameter:
    case QKnxInterfaceObjectProperty::Device::ObjectAddress:
    case QKnxInterfaceObjectProperty::Device::PsuType:
    case QKnxInterfaceObjectProperty::Device::PsuStatus:
    case QKnxInterfaceObjectProperty::Device::PsuEnable:
    case QKnxInterfaceObjectProperty::Device::DomainAddress:
    case QKnxInterfaceObjectProperty::Device::IoList:
    case QKnxInterfaceObjectProperty::Device::MgtDescriptor01:
    case QKnxInterfaceObjectProperty::Device::PL110Parameter:
    case QKnxInterfaceObjectProperty::Device::RfRepeatCounter:
    case QKnxInterfaceObjectProperty::Device::ReceiveBlockTable:
    case QKnxInterfaceObjectProperty::Device::RandomPauseTable:
    case QKnxInterfaceObjectProperty::Device::ReceiveBlockNumber:
    case QKnxInterfaceObjectProperty::Device::HardwareType:
    case QKnxInterfaceObjectProperty::Device::RetransmitterNumber:
    case QKnxInterfaceObjectProperty::Device::SerialNumberTable:
    case QKnxInterfaceObjectProperty::Device::BibatMasterAddress:
    case QKnxInterfaceObjectProperty::Device::RfDomainAddressDevice:
    case QKnxInterfaceObjectProperty::Device::DeviceDescriptor:
    case QKnxInterfaceObjectProperty::Device::MeteringFilterTable:
    case QKnxInterfaceObjectProperty::Device::GroupTelegramRateLimitTimeBase:
    case QKnxInterfaceObjectProperty::Device::GroupTelegramRateLimitNumberOfTelegrams:
    case QKnxInterfaceObjectProperty::Device::Channel01Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel02Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel03Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel04Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel05Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel06Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel07Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel08Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel09Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel10Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel11Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel12Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel13Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel14Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel15Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel16Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel17Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel18Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel19Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel20Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel21Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel22Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel23Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel24Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel25Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel26Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel27Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel28Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel29Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel30Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel31Parameter:
    case QKnxInterfaceObjectProperty::Device::Channel32Parameter:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l GroupAddressTable property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isGroupAddressTableProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::GroupAddressTable::ExtendedFrameFormat:
    case QKnxInterfaceObjectProperty::GroupAddressTable::AddressTable1:
    case QKnxInterfaceObjectProperty::GroupAddressTable::GroupResponserTable:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l AssociationTable property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isAssociationTableProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l ApplicationProgram
    property or a \l General property; \c false otherwise.

*/
bool QKnxInterfaceObjectProperty::isApplicationProgramProperty(QKnxInterfaceObjectProperty property)
{
    if (property == QKnxInterfaceObjectProperty::ApplicationProgram::ParameterReference)
        return true;
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l InterfaceProgram property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isInterfaceProgramProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l KnxObjectAssociationTable
    property or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isKnxObjectAssociationTableProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l Router property
    or a \l General property; \c false otherwise.

*/
bool QKnxInterfaceObjectProperty::isRouterProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    // case QKnxInterfaceObjectProperty::Router::LineStatus: ### Qt6: remove
    case QKnxInterfaceObjectProperty::Router::MediumStatus:
    case QKnxInterfaceObjectProperty::Router::MainLcConfig:
    case QKnxInterfaceObjectProperty::Router::SubLcConfig:
    case QKnxInterfaceObjectProperty::Router::MainLcGroupConfig:
    case QKnxInterfaceObjectProperty::Router::SubLcGroupConfig:
    case QKnxInterfaceObjectProperty::Router::RouteTableControl:
    case QKnxInterfaceObjectProperty::Router::CouplerServerControl:
    case QKnxInterfaceObjectProperty::Router::MaxRouterApduLength:
    case QKnxInterfaceObjectProperty::Router::L2CouplerType:
    case QKnxInterfaceObjectProperty::Router::Medium:
    case QKnxInterfaceObjectProperty::Router::FilterTableUse:
    case QKnxInterfaceObjectProperty::Router::RfEnableSbc:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l LteAddressRoutingTable
    property or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isLteAddressRoutingTableProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::LteAddressRoutingTable::LteRouteSelect:
    case QKnxInterfaceObjectProperty::LteAddressRoutingTable::LteRouteTable:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l CemiServer property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isCemiServerProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::CemiServer::MediumType:
    case QKnxInterfaceObjectProperty::CemiServer::CommonMode:
    case QKnxInterfaceObjectProperty::CemiServer::MediumAvailability:
    case QKnxInterfaceObjectProperty::CemiServer::AdditionalInfoTypes:
    case QKnxInterfaceObjectProperty::CemiServer::TimeBase:
    case QKnxInterfaceObjectProperty::CemiServer::TransparencyModeEnabled:
    case QKnxInterfaceObjectProperty::CemiServer::BiBatNextBlock:
    case QKnxInterfaceObjectProperty::CemiServer::RfModeSelect:
    case QKnxInterfaceObjectProperty::CemiServer::RfModeSupport:
    case QKnxInterfaceObjectProperty::CemiServer::RfFilteringModeSelectCemiServer:
    case QKnxInterfaceObjectProperty::CemiServer::RfFilteringModeSupport:
    case QKnxInterfaceObjectProperty::CemiServer::MaxInterfaceApduLength:
    case QKnxInterfaceObjectProperty::CemiServer::MaxLocalApduLength:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l GroupObjectTable property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isGroupObjectTableProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::GroupObjectTable::GroupObjectTableProperty:
    case QKnxInterfaceObjectProperty::GroupObjectTable::ExtendedGroupObjectReference:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l PollingMaster property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isPollingMasterProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::PollingMaster::PollingState:
    case QKnxInterfaceObjectProperty::PollingMaster::PollingSlaveAddress:
    case QKnxInterfaceObjectProperty::PollingMaster::PollingCycle:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l KnxNetIpParameter property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isKnxNetIpParameterProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::ProjectInstallationId:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxIndividualAddress:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::AdditionalIndividualAddresses:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAssignmentMethod:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::IpAssignmentMethod:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::IpCapabilities:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAddress:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentSubnetMask:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentDefaultGateway:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::IpAddress:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::SubnetMask:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::DefaultGateway:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::DhcpBootpServer:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::MacAddress:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::SystemSetupMulticastAddress:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::RoutingMulticastAddress:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::Ttl:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpDeviceCapabilities:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpDeviceState:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpRoutingCapabilities:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::PriorityFifoEnabled:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::QueueOverflowToIp:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::QueueOverflowToKnx:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::MsgTransmitToIp:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::MsgTransmitToKnx:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::FriendlyName:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::RoutingBusyWaitTime:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::TunnelingAddresses:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::BackboneKey:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::DeviceAuthenticationCode:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::PasswordHashes:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::SecuredServiceFamilies:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::MulticastLatencyTolerance:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::SyncLatencyFraction:
    case QKnxInterfaceObjectProperty::KnxNetIpParameter::TunnellingUsers:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l FileServer property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isFileServerProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l Security property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isSecurityProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::Security::SecurityMode:
    case QKnxInterfaceObjectProperty::Security::P2pKeyTable:
    case QKnxInterfaceObjectProperty::Security::GroupKeyTable:
    case QKnxInterfaceObjectProperty::Security::SecurityIndividualAddressTable:
    case QKnxInterfaceObjectProperty::Security::SecurityFailuresLog:
    case QKnxInterfaceObjectProperty::Security::SkiTool:
    case QKnxInterfaceObjectProperty::Security::SecurityReport:
    case QKnxInterfaceObjectProperty::Security::SecurityReportControl:
    case QKnxInterfaceObjectProperty::Security::SequenceNumberSending:
    case QKnxInterfaceObjectProperty::Security::ZoneKeysTable:
    case QKnxInterfaceObjectProperty::Security::GoSecurityFlags:
        return true;
    }
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns \c true if \a property is a dedicated \l RfMedium property
    or a \l General property; \c false otherwise.
*/
bool QKnxInterfaceObjectProperty::isRfMediumProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::RfMedium::RfMultiType:
    case QKnxInterfaceObjectProperty::RfMedium::RfMultiPhysicalFeatures:
    case QKnxInterfaceObjectProperty::RfMedium::RfMultiCallChannel:
    case QKnxInterfaceObjectProperty::RfMedium::RfMultiObjectLink:
    case QKnxInterfaceObjectProperty::RfMedium::RfMultiExtendedGroupAddressRepeated:
    case QKnxInterfaceObjectProperty::RfMedium::RfDomainAddressRfMedium:
    case QKnxInterfaceObjectProperty::RfMedium::RfRetransmitter:
    case QKnxInterfaceObjectProperty::RfMedium::RfSecurityReportControl:
    case QKnxInterfaceObjectProperty::RfMedium::RfFilteringModeSelectRfMedium:
    case QKnxInterfaceObjectProperty::RfMedium::RfBidirTimeout:
    case QKnxInterfaceObjectProperty::RfMedium::RfDiagSaFilterTable:
    case QKnxInterfaceObjectProperty::RfMedium::RfDiagQualityTable:
    case QKnxInterfaceObjectProperty::RfMedium::RfDiagProbe:
    case QKnxInterfaceObjectProperty::RfMedium::TransmissionMode:
    case QKnxInterfaceObjectProperty::RfMedium::ReceptionMode:
    case QKnxInterfaceObjectProperty::RfMedium::TestSignal:
    case QKnxInterfaceObjectProperty::RfMedium::FastAcknowledge:
    case QKnxInterfaceObjectProperty::RfMedium::FastAcknowledgeActivate:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

QT_END_NAMESPACE
