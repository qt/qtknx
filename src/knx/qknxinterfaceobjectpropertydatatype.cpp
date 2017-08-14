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

#include "qhash.h"
#include "qknxinterfaceobjectpropertydatatype.h"

QT_BEGIN_NAMESPACE

struct QKnxInterfaceObjectPropertyDataTypePrivate final : public QSharedData
{
    QKnxInterfaceObjectPropertyDataTypePrivate() = default;
    ~QKnxInterfaceObjectPropertyDataTypePrivate() = default;

    quint8 m_size = 0;
    static const constexpr quint8 m_readSize = 1;
    QKnxInterfaceObjectPropertyDataType::Id m_id = QKnxInterfaceObjectPropertyDataType::Id::Invalid;
    QKnxInterfaceObjectPropertyDataType::Unit m_unit = QKnxInterfaceObjectPropertyDataType::Unit::Single;
};

QKnxInterfaceObjectPropertyDataType::QKnxInterfaceObjectPropertyDataType()
    : d_ptr(new QKnxInterfaceObjectPropertyDataTypePrivate)
{}

QKnxInterfaceObjectPropertyDataType::QKnxInterfaceObjectPropertyDataType(Id id, quint8 size, Unit unit)
    : d_ptr(new QKnxInterfaceObjectPropertyDataTypePrivate)
{
    d_ptr->m_id = id;
    d_ptr->m_size = size;
    d_ptr->m_unit =unit;
}

QKnxInterfaceObjectPropertyDataType::~QKnxInterfaceObjectPropertyDataType()
{}

bool QKnxInterfaceObjectPropertyDataType::isValid() const
{
    return d_ptr->m_size > 0 && d_ptr->m_id != Id::Invalid;
}

quint8 QKnxInterfaceObjectPropertyDataType::size(bool read) const
{
    if (read && d_ptr->m_id == QKnxInterfaceObjectPropertyDataType::Id::Control)
        return d_ptr->m_readSize;
    return d_ptr->m_size;
}

QKnxInterfaceObjectPropertyDataType::Id QKnxInterfaceObjectPropertyDataType::id() const
{
    return d_ptr->m_id;
}

QKnxInterfaceObjectPropertyDataType::Unit QKnxInterfaceObjectPropertyDataType::unit() const
{
    return d_ptr->m_unit;
}

quint8 QKnxInterfaceObjectPropertyDataType::size(Id id, bool read)
{
    switch (id) {
    case QKnxInterfaceObjectPropertyDataType::Id::Control:
        if (read)
            return 1;
        return 10;
    case QKnxInterfaceObjectPropertyDataType::Id::Char:
    case QKnxInterfaceObjectPropertyDataType::Id::UnsignedChar:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic01:
    case QKnxInterfaceObjectPropertyDataType::Id::BinaryInformation:
    case QKnxInterfaceObjectPropertyDataType::Id::Bitset8:
    case QKnxInterfaceObjectPropertyDataType::Id::Enum8:
    case QKnxInterfaceObjectPropertyDataType::Id::Scaling:
        return 1;
    case QKnxInterfaceObjectPropertyDataType::Id::Int:
    case QKnxInterfaceObjectPropertyDataType::Id::UnsignedInt:
    case QKnxInterfaceObjectPropertyDataType::Id::KnxFloat:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic02:
    case QKnxInterfaceObjectPropertyDataType::Id::Version:
    case QKnxInterfaceObjectPropertyDataType::Id::Bitset16:
        return 2;
    case QKnxInterfaceObjectPropertyDataType::Id::Date:
    case QKnxInterfaceObjectPropertyDataType::Id::Time:
    case QKnxInterfaceObjectPropertyDataType::Id::PollGroupSettings:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic03:
        return 3;
    case QKnxInterfaceObjectPropertyDataType::Id::Long:
    case QKnxInterfaceObjectPropertyDataType::Id::UnsignedLong:
    case QKnxInterfaceObjectPropertyDataType::Id::Float:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic04:
        return 4;
    case QKnxInterfaceObjectPropertyDataType::Id::Double:
    case QKnxInterfaceObjectPropertyDataType::Id::DateTime:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic08:
        return 8;
    case QKnxInterfaceObjectPropertyDataType::Id::CharBlock:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic10:
        return 10;
    case QKnxInterfaceObjectPropertyDataType::Id::ShortCharBlock:
    case QKnxInterfaceObjectPropertyDataType::Id::Generic05:
        return 5;
    case QKnxInterfaceObjectPropertyDataType::Id::VariableLength:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved01:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved02:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved03:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved04:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved05:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved06:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved07:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved08:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved09:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved10:
    case QKnxInterfaceObjectPropertyDataType::Id::Utf8:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved11:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved12:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved13:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved14:
    case QKnxInterfaceObjectPropertyDataType::Id::Reserved15:
        return 0;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic06:
    case QKnxInterfaceObjectPropertyDataType::Id::AlarmInfo:
        return 6;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic07:
        return 7;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic09:
        return 9;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic11:
        return 11;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic12:
        return 12;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic13:
        return 13;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic14:
        return 14;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic15:
        return 15;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic16:
        return 16;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic17:
        return 17;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic18:
        return 18;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic19:
        return 19;
    case QKnxInterfaceObjectPropertyDataType::Id::Generic20:
        return 20;
    case QKnxInterfaceObjectPropertyDataType::Id::NeVl:
    case QKnxInterfaceObjectPropertyDataType::Id::NeFl:
    case QKnxInterfaceObjectPropertyDataType::Id::Function:
    case QKnxInterfaceObjectPropertyDataType::Id::Escape:
    case QKnxInterfaceObjectPropertyDataType::Id::Invalid:
        break;
    }
    return 0;
}

QVector<QKnxInterfaceObjectPropertyDataType>
    QKnxInterfaceObjectPropertyDataType::fromProperty(QKnxInterfaceObjectProperty property)
{
    if (!QKnxInterfaceObjectProperty::isProperty(property))
        return {};

    using PropertyDataTypeHash = QHash<int, QVector<QKnxInterfaceObjectPropertyDataType>>;

    static const PropertyDataTypeHash general = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::General::ObjectType, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::General::ObjectName, { { Id::UnsignedChar, 1, Unit::Array } } },
            { QKnxInterfaceObjectProperty::General::Semaphor, {} },
            { QKnxInterfaceObjectProperty::General::GroupObjectReference, {} },
            { QKnxInterfaceObjectProperty::General::LoadStateControl, { { Id::Control, 10 } } },
            { QKnxInterfaceObjectProperty::General::RunStateControl, { { Id::Control, 10 } } },
            { QKnxInterfaceObjectProperty::General::TableReference, { { Id::UnsignedInt, 2 }, { Id::Long, 4 } } },
            { QKnxInterfaceObjectProperty::General::ServiceControl, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::General::FirmwareRevision, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::General::ServicesSupported, {} },
            { QKnxInterfaceObjectProperty::General::SerialNumber, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::General::ManufacturerId, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::General::ProgramVersion, { { Id::Generic05, 5 } } },
            { QKnxInterfaceObjectProperty::General::DeviceControl, { { Id::Bitset8, 1 } } },
            { QKnxInterfaceObjectProperty::General::OrderInfo, { { Id::Generic10, 10 } } },
            { QKnxInterfaceObjectProperty::General::PeiType, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::General::PortConfiguration, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::General::PollGroupSettings, { { Id::PollGroupSettings, 3 } } },
            { QKnxInterfaceObjectProperty::General::ManufacturerData, { { Id::Generic04, 4 } } },
            { QKnxInterfaceObjectProperty::General::Enable, {} },
            { QKnxInterfaceObjectProperty::General::Description, { { Id::UnsignedChar, 1, Unit::Array } } },
            { QKnxInterfaceObjectProperty::General::File, {} },
            { QKnxInterfaceObjectProperty::General::Table, { { Id::Int, 2, Unit::Array }, { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::General::Enrol, {} },
            { QKnxInterfaceObjectProperty::General::Version, { { Id::Version, 2 } } },
            { QKnxInterfaceObjectProperty::General::GroupObjectLink, {} },
            { QKnxInterfaceObjectProperty::General::McbTable, { { Id::Generic08, 8, Unit::Array } } },
            { QKnxInterfaceObjectProperty::General::ErrorCode, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::General::ObjectIndex, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::General::DownloadCounter, { { Id::UnsignedChar, 1 } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isGeneralProperty(property))
        return general.value(property);

    static const PropertyDataTypeHash device = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::Device::RoutingCount, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::MaxRetryCount, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::ErrorFlags, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::ProgMode, { { Id::Bitset8, 1 } } },
            { QKnxInterfaceObjectProperty::Device::ProductId, { { Id::Generic10, 10 } } },
            { QKnxInterfaceObjectProperty::Device::MaxApduLengthDevice, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Device::SubnetAddress, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::DeviceAddress, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::PbConfig, { { Id::Generic04, 4 } } },
            { QKnxInterfaceObjectProperty::Device::AddressReport, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::Device::AddressCheck, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::ObjectValue, {} },
            { QKnxInterfaceObjectProperty::Device::ObjectLink, {} },
            { QKnxInterfaceObjectProperty::Device::Application, {} },
            { QKnxInterfaceObjectProperty::Device::Parameter, {} },
            { QKnxInterfaceObjectProperty::Device::ObjectAddress, {} },
            { QKnxInterfaceObjectProperty::Device::PsuType, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Device::PsuStatus, { { Id::BinaryInformation, 1 } } },
            { QKnxInterfaceObjectProperty::Device::PsuEnables, { { Id::Enum8, 1 } } },
            { QKnxInterfaceObjectProperty::Device::DomainAddress, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Device::IoList, { { Id::UnsignedInt, 2, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Device::MgtDescriptor01, { { Id::Generic10, 10 } } },
            { QKnxInterfaceObjectProperty::Device::PL110Parameter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::RfRepeatCounter, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::ReceiveBlockTable, { { Id::UnsignedChar, 1, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Device::RandomPauseTable, { { Id::UnsignedChar, 1, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Device::ReceiveBlockNumber, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::HardwareType, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::Device::RetransmitterNumber, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::Device::SerialNumberTable, { { Id::Generic06, 6, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Device::BibatMasterAddress, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Device::RfDomainAddressDevice, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::Device::DeviceDescriptor, { { Id::Generic02, 2 } } },
            { QKnxInterfaceObjectProperty::Device::MeteringFilterTable, { { Id::Generic08, 8, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Device::GroupTelegramRateLimitTimeBase, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Device::GroupTelegramRateLimitNumberOfTelegrams, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Device::Channel01Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel02Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel03Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel04Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel05Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel06Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel07Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel08Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel09Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel10Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel11Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel12Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel13Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel14Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel15Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel16Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel17Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel18Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel19Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel20Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel21Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel22Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel23Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel24Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel25Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel26Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel27Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel28Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel29Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel30Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel31Paramter, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Device::Channel32Paramter, { { Id::Generic01, 1 } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isDeviceProperty(property))
        return device.value(property);

    static const PropertyDataTypeHash groupObjectTable = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::GroupObjectTable::GroupObjectTableProperty,
            { { Id::Generic06, 6, Unit::Array } } },
            { QKnxInterfaceObjectProperty::GroupObjectTable::ExtendedGroupObjectReference,
            { { Id::Generic08, 8, Unit::Array } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isGroupObjectTableProperty(property))
        return groupObjectTable.value(property);

    static const PropertyDataTypeHash router = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::Router::LineStatus, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Router::MainLcConfig, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Router::SubLcConfig, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Router::MainLcGroupConfig, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Router::SubLcGroupConfig, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Router::RouteTableControl, {} },
            { QKnxInterfaceObjectProperty::Router::CouplerServerControl, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::Router::MaxRouterApduLength, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::Router::Medium, { { Id::Enum8, 1 } } },
            { QKnxInterfaceObjectProperty::Router::FilterTableUse, { { Id::BinaryInformation, 1 } } },
            { QKnxInterfaceObjectProperty::Router::RfEnableSbc, {} }
        };
    }();
    if (QKnxInterfaceObjectProperty::isRouterProperty(property))
        return router.value(property);

    static const PropertyDataTypeHash netIp = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::ProjectInstallationId, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxIndividualAddress, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::AdditionalIndividualAddresses, { { Id::UnsignedInt, 2, Unit::Array } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAssignmentMethod, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::IpAssignmentMethod, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::IpCapabilities, { { Id::Bitset8, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAddress, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentSubnetMask, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentDefaultGateway, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::IpAddress, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SubnetMask, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::DefaultGateway, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::DhcpBootpServer, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MacAddress, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SystemSetupMulticastAddress, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::RoutingMulticastAddress, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::Ttl, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpDeviceCapabilities, { { Id::Bitset16, 2 }, { Id::Generic02, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpDeviceState, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpRoutingCapabilities, { { Id::UnsignedChar, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::PriorityFifoEnabled, { { Id::BinaryInformation, 1 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::QueueOverflowToIp, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::QueueOverflowToKnx, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MsgTransmitToIp, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MsgTransmitToKnx, { { Id::UnsignedLong, 4 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::FriendlyName, { { Id::UnsignedChar, 1, Unit::Array } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::RoutingBusyWaitTime, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::BackboneKey, { { Id::Generic16, 16 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::DeviceAuthenticationCode, { { Id::Generic16, 16 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::PasswordHashes, { { Id::Generic16, 16, Unit::Array } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SecuredServiceFamilies, {} },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MulticastLatencyTolerance, { { Id::UnsignedInt, 2 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SyncLatencyFraction, {} },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::TunnellingUsers, { { Id::Generic02, 2, Unit::Array } } },
        };
    }();
    if (QKnxInterfaceObjectProperty::isKnxNetIpParameterProperty(property))
        return netIp.value(property);

    static const PropertyDataTypeHash security = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::Security::SecurityMode, {} },
            { QKnxInterfaceObjectProperty::Security::P2pKeyTable, { { Id::Generic18, 18, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Security::GroupKeyTable, { { Id::Generic18, 18, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Security::SecurityIndividualAddressTable, { { Id::Generic08, 8, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Security::SecurityFailuresLog, {} },
            { QKnxInterfaceObjectProperty::Security::SkiTool, { { Id::Generic16, 16 } } },
            { QKnxInterfaceObjectProperty::Security::SecurityReport, { { Id::Bitset8, 1 } } },
            { QKnxInterfaceObjectProperty::Security::SecurityReportControl, { { Id::BinaryInformation, 1 } } },
            { QKnxInterfaceObjectProperty::Security::SequenceNumberSending, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::Security::ZoneKeysTable, { { Id::Generic19, 19, Unit::Array } } },
            { QKnxInterfaceObjectProperty::Security::GoSecurityFlags, { { Id::Generic01, 1, Unit::Array } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isSecurityProperty(property))
        return security.value(property);

    static const PropertyDataTypeHash rfMedium = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiType, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiPhysicalFeatures, { { Id::Bitset8, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiCallChannel, { { Id::Generic01, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiObjectLink, {} },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiExtendedGroupAddressRepeated, {} },
            { QKnxInterfaceObjectProperty::RfMedium::RfDomainAddressRfMedium, { { Id::Generic06, 6 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfRetransmitter, { { Id::BinaryInformation, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfSecurityReportControl, { { Id::BinaryInformation, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfFilteringModeSelectRfMedium, { { Id::Bitset8, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfBidirTimeout, {} },
            { QKnxInterfaceObjectProperty::RfMedium::RfDiagSaFilterTable, { { Id::Generic03, 3, Unit::Array } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfDiagQualityTable, { { Id::Generic04, 4, Unit::Array } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfDiagProbe, {} },
            { QKnxInterfaceObjectProperty::RfMedium::TransmissionMode, { { Id::Enum8, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::ReceptionMode, { { Id::Enum8, 1 } } },
            { QKnxInterfaceObjectProperty::RfMedium::TestSignal, { { Id::Generic02, 2 } } },
            { QKnxInterfaceObjectProperty::RfMedium::FastAcknowledge, { { Id::Generic02, 2, Unit::Array } } },
            { QKnxInterfaceObjectProperty::RfMedium::FastAcknowledgeActivate, { { Id::BinaryInformation, 1 } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isRfMediumProperty(property))
        return rfMedium.value(property);

    return {};
}

QKnxInterfaceObjectPropertyDataType::QKnxInterfaceObjectPropertyDataType(const QKnxInterfaceObjectPropertyDataType &o)
    : d_ptr(o.d_ptr)
{}

QKnxInterfaceObjectPropertyDataType &
QKnxInterfaceObjectPropertyDataType::operator=(const QKnxInterfaceObjectPropertyDataType &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

void QKnxInterfaceObjectPropertyDataType::swap(QKnxInterfaceObjectPropertyDataType &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

#ifdef Q_COMPILER_RVALUE_REFS
QKnxInterfaceObjectPropertyDataType &
QKnxInterfaceObjectPropertyDataType::operator=(QKnxInterfaceObjectPropertyDataType &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}
#endif

QT_END_NAMESPACE
