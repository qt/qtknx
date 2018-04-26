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

#include "qknxinterfaceobjectproperty.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxInterfaceObjectProperty

    \inmodule QtKnx
    \brief This class holds information about the properties of the KNX interface object.

    KNX interface object holds information about the device functionalities.
    Different properties are storing different functionalities.
    Only properties of interface object of type \l QKnxInterfaceObjectType::System are listed here.
*/

/*!
    \enum QKnxInterfaceObjectProperty::General
    This enum describes the General properties of a KNX interface object.
    Those properties can be found in any interface object.

    \value ObjectType
    \value ObjectName
    \value Semaphor
    \value GroupObjectReference
    \value LoadStateControl
    \value RunStateControl
    \value TableReference
    \value ServiceControl
    \value FirmwareRevision
    \value ServicesSupported
    \value SerialNumber
    \value ManufacturerId
    \value ProgramVersion
    \value DeviceControl
    \value OrderInfo
    \value PeiType
    \value PortConfiguration
    \value PollGroupSettings
    \value ManufacturerData
    \value Enable
    \value Description
    \value File
    \value Table
    \value Enrol
    \value Version
    \value GroupObjectLink
    \value McbTable
    \value ErrorCode
    \value ObjectIndex
    \value DownloadCounter
*/

/*!
    \enum QKnxInterfaceObjectProperty::Device
    This enum describes the properties specific to Device interface object.

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
    \value Channel01Paramter
    \value Channel02Paramter
    \value Channel03Paramter
    \value Channel04Paramter
    \value Channel05Paramter
    \value Channel06Paramter
    \value Channel07Paramter
    \value Channel08Paramter
    \value Channel09Paramter
    \value Channel10Paramter
    \value Channel11Paramter
    \value Channel12Paramter
    \value Channel13Paramter
    \value Channel14Paramter
    \value Channel15Paramter
    \value Channel16Paramter
    \value Channel17Paramter
    \value Channel18Paramter
    \value Channel19Paramter
    \value Channel20Paramter
    \value Channel21Paramter
    \value Channel22Paramter
    \value Channel23Paramter
    \value Channel24Paramter
    \value Channel25Paramter
    \value Channel26Paramter
    \value Channel27Paramter
    \value Channel28Paramter
    \value Channel29Paramter
    \value Channel30Paramter
    \value Channel31Paramter
    \value Channel32Paramter
*/

/*!
    \enum QKnxInterfaceObjectProperty::GroupAddressTable
    This enum describes properties specific to Address Table interface object.

    \value ExtendedFrameFormat
    \value AddressTable1
    \value GroupResponserTable
*/

/*!
    \enum QKnxInterfaceObjectProperty::ApplicationProgram
    This enum describes properties specific to application program interface object.

    \value ParameterReference
*/

/*!
    \enum QKnxInterfaceObjectProperty::Router
    This enum describes properties specific to routers interface object.

    \value LineStatus
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
    This enum describes properties specific to LTE address routing table interface object.

    \value LteRouteSelect
    \value LteRouteTable
*/

/*!
    \enum QKnxInterfaceObjectProperty::CemiServer
    This enum describes properties specific to CEMI server interface object.

    \value MediumType
    \value CommonMode
    \value MediumAvailability
    \value AdditionalInfoTypes
    \value TimeBase
    \value TransparencyModeEnabled
    \value BiBatNextBlock
    \value RfModeSelect
    \value RfModeSupport
    \value RfFilteringModeSelectCemiServer
    \value RfFilteringModeSupport
*/

/*!
    \enum QKnxInterfaceObjectProperty::GroupObjectTable
    This enum describes properties specific to group object table interface object.

    \value GroupObjectTableProperty
    \value ExtendedGroupObjectReference
*/

/*!
    \enum QKnxInterfaceObjectProperty::PollingMaster
    This enum describes properties specific to polling master interface object.

    \value PollingState
    \value PollingSlaveAddress
    \value PollingCycle
*/

/*!
    \enum QKnxInterfaceObjectProperty::KnxNetIpParameter
    This enum describes properties specific to KNXnet/IP parameter interface object.

    \value ProjectInstallationId
    \value KnxIndividualAddress
    \value AdditionalIndividualAddresses
    \value CurrentIpAssignmentMethod
    \value IpAssignmentMethod
    \value IpCapabilities
    \value CurrentIpAddress
    \value CurrentSubnetMask
    \value CurrentDefaultGateway
    \value IpAddress
    \value SubnetMask
    \value DefaultGateway
    \value DhcpBootpServer
    \value MacAddress
    \value SystemSetupMulticastAddress
    \value RoutingMulticastAddress
    \value Ttl
    \value KnxNetIpDeviceCapabilities
    \value KnxNetIpDeviceState
    \value KnxNetIpRoutingCapabilities
    \value PriorityFifoEnabled
    \value QueueOverflowToIp
    \value QueueOverflowToKnx
    \value MsgTransmitToIp
    \value MsgTransmitToKnx
    \value FriendlyName
    \value RoutingBusyWaitTime
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
    This enum describes properties specific to security interface object.

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
    This enum describes properties specific to RF medium interface object.

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
    Returns true if the \a property listed in this class.
*/
bool QKnxInterfaceObjectProperty::isProperty(QKnxInterfaceObjectProperty property)
{
    return property < QKnxInterfaceObjectProperty::Invalid;
}

/*!
    Returns true if the \a property is an Independent property; false otherwise.
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
    Returns true if the \a property is an dedicated Device property; false
    otherwise.

    \note Independent properties are considered to be part of Device properties
    and therefore the function will return \c true for these properties as well.
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
    Returns true if the \a property is an dedicated Group address table property;
    false otherwise.

    \note Independent properties are considered to be part of Group address table
    properties and therefore the function will return \c true for these properties
    as well.
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
    Returns true if the \a property is an dedicated Association table property;
    false otherwise.

    \note Independent properties are considered to be part of Association table
    properties and therefore the function will return \c true for these properties
    as well.
*/
bool QKnxInterfaceObjectProperty::isAssociationTableProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Application program property;
    false otherwise.

    \note Independent properties are considered to be part of Application program
    properties and therefore the function will return \c true for these properties
    as well.
*/
bool QKnxInterfaceObjectProperty::isApplicationProgramProperty(QKnxInterfaceObjectProperty property)
{
    if (property == QKnxInterfaceObjectProperty::ApplicationProgram::ParameterReference)
        return true;
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Interface program property;
    false otherwise.

    \note Independent properties are considered to be part of Interface program
    properties and therefore the function will return \c true for these properties
    as well.
*/
bool QKnxInterfaceObjectProperty::isInterfaceProgramProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Router property; false
    otherwise.

    \note Independent properties are considered to be part of Router properties
    and therefore the function will return \c true for these properties as well.
*/
bool QKnxInterfaceObjectProperty::isKnxObjectAssociationTableProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Router property; false
    otherwise.

    \note Independent properties are considered to be part of Router properties
    and therefore the function will return \c true for these properties as well.
*/
bool QKnxInterfaceObjectProperty::isRouterProperty(QKnxInterfaceObjectProperty property)
{
    switch (property) {
    case QKnxInterfaceObjectProperty::Router::LineStatus:
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
    Returns true if the \a property is an dedicated LTE address routing table
    property; false otherwise.

    \note Independent properties are considered to be part of LTE address
    routing table properties and therefore the function will return \c true for
    these properties as well.
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
    Returns true if the \a property is an dedicated cEMI server property;
    false otherwise.

    \note Independent properties are considered to be part of cEMI server
    properties and therefore the function will return \c true for these properties
    as well.
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
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Group object table property;
    false otherwise.

    \note Independent properties are considered to be part of Group object table
    properties and therefore the function will return \c true for these properties
    as well.
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
    Returns true if the \a property is an dedicated Polling master property;
    false otherwise.

    \note Independent properties are considered to be part of Polling master
    properties and therefore the function will return \c true for these properties
    as well.
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
    Returns true if the \a property is an dedicated KNXnet/IP property; false
    otherwise.

    \note Independent properties are considered to be part of KNXnet/IP properties
    and therefore the function will return \c true for these properties as well.
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
    Returns true if the \a property is an dedicated File server property; false
    otherwise.

    \note Independent properties are considered to be part of File server
    properties and therefore the function will return \c true for these properties
    as well.
*/
bool QKnxInterfaceObjectProperty::isFileServerProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Security property; false
    otherwise.

    \note Independent properties are considered to be part of Security properties
    and therefore the function will return \c true for these properties as well.
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
    Returns true if the \a property is an dedicated RF Medium property; false
    otherwise.

    \note Independent properties are considered to be part of RF Medium properties
    and therefore the function will return \c true for these properties as well.
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
