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
    and therefore the function will return \c truefor these properties as well.
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
    case QKnxInterfaceObjectProperty::Device::Channel01Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel02Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel03Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel04Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel05Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel06Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel07Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel08Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel09Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel10Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel11Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel12Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel13Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel14Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel15Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel16Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel17Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel18Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel19Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel20Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel21Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel22Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel23Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel24Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel25Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel26Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel27Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel28Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel29Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel30Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel31Paramter:
    case QKnxInterfaceObjectProperty::Device::Channel32Paramter:
        return true;
    };
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Group address table property;
    false otherwise.

    \note Independent properties are considered to be part of Group address table
    properties and therefore the function will return \c truefor these properties
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
    properties and therefore the function will return \c truefor these properties
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
    properties and therefore the function will return \c truefor these properties
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
    properties and therefore the function will return \c truefor these properties
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
    and therefore the function will return \c truefor these properties as well.
*/
bool QKnxInterfaceObjectProperty::isKnxObjectAssociationTableProperty(QKnxInterfaceObjectProperty property)
{
    return QKnxInterfaceObjectProperty::isGeneralProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Router property; false
    otherwise.

    \note Independent properties are considered to be part of Router properties
    and therefore the function will return \c truefor these properties as well.
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
    routing table properties and therefore the function will return \c truefor
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
    properties and therefore the function will return \c truefor these properties
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
    properties and therefore the function will return \c truefor these properties
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
    properties and therefore the function will return \c truefor these properties
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
    and therefore the function will return \c truefor these properties as well.
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
    properties and therefore the function will return \c truefor these properties
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
    and therefore the function will return \c truefor these properties as well.
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
    and therefore the function will return \c truefor these properties as well.
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
