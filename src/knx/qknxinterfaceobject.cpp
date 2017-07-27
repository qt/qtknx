/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxinterfaceobject.h"

QT_BEGIN_NAMESPACE

/*!
    Returns true if the \a property is an Independent property; false otherwise.
*/
bool QKnxInterfaceObject::Property::isIndependentProperty(Property property)
{
    switch (property) {
    case Property::Independent::ObjectType:
    case Property::Independent::ObjectName:
    case Property::Independent::LoadStateControl:
    case Property::Independent::RunStateControl:
    case Property::Independent::TableReference:
    case Property::Independent::ServiceControl:
    case Property::Independent::FirmwareRevision:
    case Property::Independent::ServicesSupported:
    case Property::Independent::SerialNumber:
    case Property::Independent::ManufacturerId:
    case Property::Independent::ProgramVersion:
    case Property::Independent::DeviceControl:
    case Property::Independent::OrderInfo:
    case Property::Independent::PeiType:
    case Property::Independent::PortConfiguration:
    case Property::Independent::PollGroupSettings:
    case Property::Independent::ManufacturerData:
    case Property::Independent::Description:
    case Property::Independent::Table:
    case Property::Independent::Version:
    case Property::Independent::McbTable:
    case Property::Independent::ErrorCode:
    case Property::Independent::ObjectIndex:
    case Property::Independent::DownloadCounter:
        return true;
    default:
        break;
    };
    return false;
}

/*!
    Returns true if the \a property is an dedicated Device property; false
    otherwise.

    \note Independent properties are considered to be part of Device properties
    and therefore the function will Returns true for these properties as well.
*/
bool QKnxInterfaceObject::Property::isDeviceProperty(Property property)
{
    switch (property) {
    case Property::Device::RoutingCount:
    case Property::Device::MaxRetryCount:
    case Property::Device::ErrorFlags:
    case Property::Device::ProgMode:
    case Property::Device::ProductId:
    case Property::Device::MaxApduLengthDevice:
    case Property::Device::SubnetAddress:
    case Property::Device::DeviceAddress:
    case Property::Device::PbConfig:
    case Property::Device::AddressReport:
    case Property::Device::AddressCheck:
    case Property::Device::ObjectValue:
    case Property::Device::ObjectLink:
    case Property::Device::Application:
    case Property::Device::Parameter:
    case Property::Device::ObjectAddress:
    case Property::Device::PsuType:
    case Property::Device::PsuStatus:
    case Property::Device::PsuEnables:
    case Property::Device::DomainAddress:
    case Property::Device::IoList:
    case Property::Device::MgtDescriptor01:
    case Property::Device::PL110Parameter:
    case Property::Device::RfRepeatCounter:
    case Property::Device::ReceiveBlockTable:
    case Property::Device::RandomPauseTable:
    case Property::Device::ReceiveBlockNumber:
    case Property::Device::HardwareType:
    case Property::Device::RetransmitterNumber:
    case Property::Device::SerialNumberTable:
    case Property::Device::BibatMasterAddress:
    case Property::Device::RfDomainAddress:
    case Property::Device::DeviceDescriptor:
    case Property::Device::GroupTelegramRateLimitTimeBase:
    case Property::Device::GroupTelegramRateLimitNumberOfTelegrams:
    case Property::Device::Channel01Paramter:
    case Property::Device::Channel02Paramter:
    case Property::Device::Channel03Paramter:
    case Property::Device::Channel04Paramter:
    case Property::Device::Channel05Paramter:
    case Property::Device::Channel06Paramter:
    case Property::Device::Channel07Paramter:
    case Property::Device::Channel08Paramter:
    case Property::Device::Channel09Paramter:
    case Property::Device::Channel10Paramter:
    case Property::Device::Channel11Paramter:
    case Property::Device::Channel12Paramter:
    case Property::Device::Channel13Paramter:
    case Property::Device::Channel14Paramter:
    case Property::Device::Channel15Paramter:
    case Property::Device::Channel16Paramter:
    case Property::Device::Channel17Paramter:
    case Property::Device::Channel18Paramter:
    case Property::Device::Channel19Paramter:
    case Property::Device::Channel20Paramter:
    case Property::Device::Channel21Paramter:
    case Property::Device::Channel22Paramter:
    case Property::Device::Channel23Paramter:
    case Property::Device::Channel24Paramter:
    case Property::Device::Channel25Paramter:
    case Property::Device::Channel26Paramter:
    case Property::Device::Channel27Paramter:
    case Property::Device::Channel28Paramter:
    case Property::Device::Channel29Paramter:
    case Property::Device::Channel30Paramter:
    case Property::Device::Channel31Paramter:
    case Property::Device::Channel32Paramter:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Group address table property;
    false otherwise.

    \note Independent properties are considered to be part of Group address table
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isGroupAddressTableProperty(Property property)
{
    switch (property) {
    case Property::GroupAddressTable::ExtendedFrameFormat:
    case Property::GroupAddressTable::AddressTable1:
    case Property::GroupAddressTable::GroupResponserTable:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Association table property;
    false otherwise.

    \note Independent properties are considered to be part of Association table
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isAssociationTableProperty(Property property)
{
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Application program property;
    false otherwise.

    \note Independent properties are considered to be part of Application program
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isApplicationProgramProperty(Property property)
{
    if (property == QKnxInterfaceObject::Property::ApplicationProgram::ParameterReference)
        return true;
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Interface program property;
    false otherwise.

    \note Independent properties are considered to be part of Interface program
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isInterfaceProgramProperty(Property property)
{
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Router property; false
    otherwise.

    \note Independent properties are considered to be part of Router properties
    and therefore the function will Returns true for these properties as well.
*/
bool QKnxInterfaceObject::Property::isKnxObjectAssociationTableProperty(Property property)
{
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Router property; false
    otherwise.

    \note Independent properties are considered to be part of Router properties
    and therefore the function will Returns true for these properties as well.
*/
bool QKnxInterfaceObject::Property::isRouterProperty(Property property)
{
    switch (property) {
    case Property::Router::LineStatus:
    case Property::Router::MainLcConfig:
    case Property::Router::SubLcConfig:
    case Property::Router::MainLcGroupConfig:
    case Property::Router::SubLcGroupConfig:
    case Property::Router::RouteTableControl:
    case Property::Router::CouplerServerControl:
    case Property::Router::MaxApduLengthRouter:
    case Property::Router::L2CouplerType:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated LTE address routing table
    property; false otherwise.

    \note Independent properties are considered to be part of LTE address
    routing table properties and therefore the function will Returns true for
    these properties as well.
*/
bool QKnxInterfaceObject::Property::isLteAddressRoutingTableProperty(Property property)
{
    switch (property) {
    case Property::LteAddressRoutingTable::LteRouteSelect:
    case Property::LteAddressRoutingTable::LteRouteTable:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated cEMI server property;
    false otherwise.

    \note Independent properties are considered to be part of cEMI server
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isCemiServerProperty(Property property)
{
    switch (property) {
    case Property::CemiServer::MediumType:
    case Property::CemiServer::CommonMode:
    case Property::CemiServer::MediumAvailability:
    case Property::CemiServer::AdditionalInfoTypes:
    case Property::CemiServer::TimeBase:
    case Property::CemiServer::TransparencyModeEnabled:
    case Property::CemiServer::BiBatNextBlock:
    case Property::CemiServer::RfModeSelect:
    case Property::CemiServer::RfModeSupport:
    case Property::CemiServer::RfFilteringModeSelect:
    case Property::CemiServer::RfFilteringModeSupport:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Group object table property;
    false otherwise.

    \note Independent properties are considered to be part of Group object table
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isGroupObjectTableProperty(Property property)
{
    switch (property) {
    case Property::GroupObjectTable::GroupObjectTableProperty:
    case Property::GroupObjectTable::ExtendedGroupObjectReference:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated Polling master property;
    false otherwise.

    \note Independent properties are considered to be part of Polling master
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isPollingMasterProperty(Property property)
{
    switch (property) {
    case Property::PollingMaster::PollingState:
    case Property::PollingMaster::PollingSlaveAddress:
    case Property::PollingMaster::PollingCycle:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated KNXnet/IP property; false
    otherwise.

    \note Independent properties are considered to be part of KNXnet/IP properties
    and therefore the function will Returns true for these properties as well.
*/
bool QKnxInterfaceObject::Property::isKnxNetIpParameterProperty(Property property)
{
    switch (property) {
    case Property::KnxNetIpParameter::ProjectInstallationId:
    case Property::KnxNetIpParameter::KnxIndividualAddress:
    case Property::KnxNetIpParameter::AdditionalIndividualAddresses:
    case Property::KnxNetIpParameter::CurrentIpAssignmentMethod:
    case Property::KnxNetIpParameter::IpAssignmentMethod:
    case Property::KnxNetIpParameter::IpCapabilities:
    case Property::KnxNetIpParameter::CurrentIpAddress:
    case Property::KnxNetIpParameter::CurrentSubnetMask:
    case Property::KnxNetIpParameter::CurrentDefaultGateway:
    case Property::KnxNetIpParameter::IpAddress:
    case Property::KnxNetIpParameter::SubnetMask:
    case Property::KnxNetIpParameter::DefaultGateway:
    case Property::KnxNetIpParameter::DhcpBootpServer:
    case Property::KnxNetIpParameter::MacAddress:
    case Property::KnxNetIpParameter::SystemSetupMulticastAddress:
    case Property::KnxNetIpParameter::RoutingMulticastAddress:
    case Property::KnxNetIpParameter::Ttl:
    case Property::KnxNetIpParameter::KnxNetIpDeviceCapabilities:
    case Property::KnxNetIpParameter::KnxNetIpDeviceState:
    case Property::KnxNetIpParameter::KnxNetIpRoutingCapabilities:
    case Property::KnxNetIpParameter::PriorityFifoEnabled:
    case Property::KnxNetIpParameter::QueueOverflowToIp:
    case Property::KnxNetIpParameter::QueueOverflowToKnx:
    case Property::KnxNetIpParameter::MsgTransmitToIp:
    case Property::KnxNetIpParameter::MsgTransmitToKnx:
    case Property::KnxNetIpParameter::FriendlyName:
    case Property::KnxNetIpParameter::RoutingBusyWaitTime:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated File server property; false
    otherwise.

    \note Independent properties are considered to be part of File server
    properties and therefore the function will Returns true for these properties
    as well.
*/
bool QKnxInterfaceObject::Property::isFileServerProperty(Property property)
{
    return Property::isIndependentProperty(property);
}

/*!
    Returns true if the \a property is an dedicated RF Medium property; false
    otherwise.

    \note Independent properties are considered to be part of RF Medium properties
    and therefore the function will Returns true for these properties as well.
*/
bool QKnxInterfaceObject::Property::isRfMediumProperty(Property property)
{
    switch (property) {
    case Property::RfMedium::RfMultiType:
    case Property::RfMedium::RfMultiPhysicalFeatures:
    case Property::RfMedium::RfMultiCallChannel:
    case Property::RfMedium::RfMultiObjectLink:
    case Property::RfMedium::RfMultiExtendedGroupAddressRepeated:
    case Property::RfMedium::TransmissionMode:
    case Property::RfMedium::ReceptionMode:
    case Property::RfMedium::TestSignal:
    case Property::RfMedium::FastAcknowledge:
    case Property::RfMedium::FastAcknowledgeActivate:
        return true;
    default:
        break;
    };
    return Property::isIndependentProperty(property);
}

QKnxInterfaceObject::Type::Type(Type::System type)
    : m_objectType(type)
{}

QKnxInterfaceObject::Type &QKnxInterfaceObject::Type::operator=(Type::System type)
{
    return operator=(quint16(type));
}

QKnxInterfaceObject::Type::Type(Type::Application type)
    : m_objectType(type)
{}

QKnxInterfaceObject::Type &QKnxInterfaceObject::Type::operator=(Type::Application type)
{
    return operator=(quint16(type));
}

QKnxInterfaceObject::Type::Type(Type::NonStandardized type)
    : m_objectType(type)
{}

QKnxInterfaceObject::Type &QKnxInterfaceObject::Type::operator=(Type::NonStandardized type)
{
    return operator=(quint16(type));
}

QKnxInterfaceObject::Type::Type(quint16 objectType)
    : m_objectType(objectType)
{}

QKnxInterfaceObject::Type &QKnxInterfaceObject::Type::operator=(quint16 objectType)
{
    m_objectType = objectType;
    return *this;
}

/*!
    Returns true if the \a objectType is of type \l System, \l Application or
    l\ NonStandardized; false otherwise.

    \note Reserved values are also considered as invalid object type.
*/
bool QKnxInterfaceObject::Type::isObjectType(QKnxInterfaceObject::Type type)
{
    if (type == System::Reserved)
        return false;

    if ((type >= Application::ApplicationReserved && type < Application::WhiteGoods))
        return false;

    if (type >= Application::ApplicationReserved2 && type < Application::WhiteGoods2)
        return false;

    return type < QKnxInterfaceObject::Type::Invalid;
}

/*!
    Returns true if the given \a property can be used in conjunction with the
    given \a objectType; false otherwise.

    \note The function performs a check on the object type first and if it is
    not a valid object type, it will return false.

    // TODO: Find a better wording for the next sentence.
    \note For all valid object types except \l System object types the function
    returns true always.
*/
bool QKnxInterfaceObject::Type::isMatch(QKnxInterfaceObject::Type type, Property property)
{
    if (!isObjectType(type))
        return false;

    switch (type) {
    case System::Device:
        return Property::isDeviceProperty(property);
    case System::GroupAddressTable:
        return Property::isGroupAddressTableProperty(property);
    case System::AssociationTable:
        return Property::isAssociationTableProperty(property);
    case System::ApplicationProgram:
        return Property::isApplicationProgramProperty(property);
    case System::InterfaceProgram:
        return Property::isInterfaceProgramProperty(property);
    case System::KnxObjectAssociationTable:
        return Property::isKnxObjectAssociationTableProperty(property);
    case System::Router:
        return Property::isRouterProperty(property);
    case System::LteAddressRoutingTable:
        return Property::isLteAddressRoutingTableProperty(property);
    case System::CemiServer:
        return Property::isCemiServerProperty(property);
    case System::GroupObjectTable:
        return Property::isGroupObjectTableProperty(property);
    case System::PollingMaster:
        return Property::isPollingMasterProperty(property);
    case System::KnxNetIpParameter:
        return Property::isKnxNetIpParameterProperty(property);
    case System::FileServer:
        return Property::isFileServerProperty(property);
    case System::RfMedium:
        return Property::isRfMediumProperty(property);
    default:
        break;
    }
    return false;
}

bool QKnxInterfaceObject::Type::operator==(Type::System type) const
{
    return m_objectType == int(type);
}

bool QKnxInterfaceObject::Type::operator==(Type::Application type) const
{
    return m_objectType == int(type);
}

bool QKnxInterfaceObject::Type::operator==(Type::NonStandardized type) const
{
    return m_objectType == int(type);
}

QT_END_NAMESPACE
