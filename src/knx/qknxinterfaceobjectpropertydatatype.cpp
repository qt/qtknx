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

#include "qhash.h"
#include "qknxdatapointtype_p.h"
#include "qknxinterfaceobjectpropertydatatype.h"

QT_BEGIN_NAMESPACE

struct QKnxInterfaceObjectPropertyDataTypePrivate final : public QSharedData
{
    QKnxInterfaceObjectPropertyDataTypePrivate() = default;
    ~QKnxInterfaceObjectPropertyDataTypePrivate() = default;

    QKnxInterfaceObjectPropertyDataType::Id m_id = QKnxInterfaceObjectPropertyDataType::Id::Invalid;
    QKnxDatapointType::Type m_type = QKnxDatapointType::Type::Unknown;
    QKnxInterfaceObjectPropertyDataType::Unit m_unit = QKnxInterfaceObjectPropertyDataType::Unit::Single;
};

QKnxInterfaceObjectPropertyDataType::QKnxInterfaceObjectPropertyDataType()
    : d_ptr(new QKnxInterfaceObjectPropertyDataTypePrivate)
{}

QKnxInterfaceObjectPropertyDataType::~QKnxInterfaceObjectPropertyDataType()
{}

QKnxInterfaceObjectPropertyDataType::QKnxInterfaceObjectPropertyDataType(Id id,
        QKnxDatapointType::Type type, QKnxInterfaceObjectPropertyDataType::Unit unit)
    : d_ptr(new QKnxInterfaceObjectPropertyDataTypePrivate)
{
    d_ptr->m_id = id;
    d_ptr->m_unit = unit;
    d_ptr->m_type = type;
}

bool QKnxInterfaceObjectPropertyDataType::isValid() const
{
    return d_ptr->m_id != Id::Invalid;
}

quint8 QKnxInterfaceObjectPropertyDataType::size(bool read) const
{
    return QKnxInterfaceObjectPropertyDataType::size(d_ptr->m_id, read);
}

QKnxInterfaceObjectPropertyDataType::Id QKnxInterfaceObjectPropertyDataType::id() const
{
    return d_ptr->m_id;
}

QKnxDatapointType::Type QKnxInterfaceObjectPropertyDataType::datapointType() const
{
    return d_ptr->m_type;
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
            { QKnxInterfaceObjectProperty::General::ObjectType,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::DptPropertyDataType }
                }
            },
            { QKnxInterfaceObjectProperty::General::ObjectName,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::General::Semaphor, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::General::GroupObjectReference, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::General::LoadStateControl, { { Id::Control } } },
            { QKnxInterfaceObjectProperty::General::RunStateControl, { { Id::Control } } },
            { QKnxInterfaceObjectProperty::General::TableReference,
                {
                    { Id::Long, QKnxDatapointType::Type::Dpt13_4ByteSigned }
                }
            },
            { QKnxInterfaceObjectProperty::General::ServiceControl,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::General::FirmwareRevision,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::General::ServicesSupported, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::General::SerialNumber,
                {
                    { Id::Generic06, QKnxDatapointType::Type::DptSerialNumber }
                }
            },
            { QKnxInterfaceObjectProperty::General::ManufacturerId,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::General::ProgramVersion, { { Id::Generic05 } } },
            { QKnxInterfaceObjectProperty::General::DeviceControl,
                {
                    { Id::Bitset8, QKnxDatapointType::Type::DptDeviceControl },
                    { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::General::OrderInfo, { { Id::Generic10 } } },
            { QKnxInterfaceObjectProperty::General::PeiType,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::General::PortConfiguration,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::General::PollGroupSettings,
                {
                    { Id::PollGroupSettings, QKnxDatapointType::Type::Dpt225_ScalingSpeed }
                }
            },
            { QKnxInterfaceObjectProperty::General::ManufacturerData, { { Id::Generic04 } } },
            { QKnxInterfaceObjectProperty::General::Enable, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::General::Description,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::General::File, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::General::Table,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned, Unit::Array },
                    { Id::Generic02, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::General::Enrol,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::General::Version,
                {
                    { Id::Version , QKnxDatapointType::Type::DptVersion },
                    { Id::Generic02 }
                }
            },
            { QKnxInterfaceObjectProperty::General::GroupObjectLink,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::General::McbTable,
                {
                    { Id::Generic08, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::General::ErrorCode,
                {
                    { Id::Enum8, QKnxDatapointType::Type::DptErrorClassSystem },
                    { Id::UnsignedChar }
                }
            },
            { QKnxInterfaceObjectProperty::General::ObjectIndex,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::DptValue1Ucount }
                }
            },
            { QKnxInterfaceObjectProperty::General::DownloadCounter,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::DptValue2UCount }
                }
            }
        };
    }();
    if (QKnxInterfaceObjectProperty::isGeneralProperty(property))
        return general.value(property);

    static const PropertyDataTypeHash device = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::Device::RoutingCount,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::MaxRetryCount, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::ErrorFlags,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::ProgMode,
                {
                    { Id::Bitset8, QKnxDatapointType::Type::Dpt21_8BitSet },
                    { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::Device::ProductId, { { Id::Generic10 } } },
            { QKnxInterfaceObjectProperty::Device::MaxApduLengthDevice,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::SubnetAddress,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::DeviceAddress,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::PbConfig, { { Id::Generic04 } } },
            { QKnxInterfaceObjectProperty::Device::AddressReport, { { Id::Generic06 } } },
            { QKnxInterfaceObjectProperty::Device::AddressCheck, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::ObjectValue,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Device::ObjectLink,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Device::Application,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Device::Parameter,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Device::ObjectAddress,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Device::PsuType,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::DptUEICurrentMilliA }
                }
            },
            { QKnxInterfaceObjectProperty::Device::PsuStatus,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::DptSwitch },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::PsuEnable,
                {
                    { Id::Enum8, QKnxDatapointType::Type::DptPsuMode },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::DomainAddress,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::DptValue2UCount }
                }
            },
            { QKnxInterfaceObjectProperty::Device::IoList,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Device::MgtDescriptor01, { { Id::Generic10 } } },
            { QKnxInterfaceObjectProperty::Device::PL110Parameter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::RfRepeatCounter,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::ReceiveBlockTable,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Device::RandomPauseTable,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Device::ReceiveBlockNumber,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::HardwareType, { { Id::Generic06 } } },
            { QKnxInterfaceObjectProperty::Device::RetransmitterNumber,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::SerialNumberTable,
                {
                    { Id::Generic06, QKnxDatapointType::Type::DptSerialNumber, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Device::BibatMasterAddress,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::RfDomainAddressDevice, { { Id::Generic06 } } },
            { QKnxInterfaceObjectProperty::Device::DeviceDescriptor, { { Id::Generic02 } } },
            { QKnxInterfaceObjectProperty::Device::MeteringFilterTable,
                {
                    { Id::Generic08, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Device::GroupTelegramRateLimitTimeBase,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::GroupTelegramRateLimitNumberOfTelegrams,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Device::Channel01Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel02Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel03Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel04Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel05Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel06Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel07Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel08Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel09Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel10Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel11Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel12Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel13Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel14Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel15Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel16Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel17Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel18Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel19Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel20Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel21Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel22Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel23Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel24Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel25Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel26Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel27Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel28Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel29Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel30Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel31Paramter, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Device::Channel32Paramter, { { Id::Generic01 } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isDeviceProperty(property))
        return device.value(property);

    static const PropertyDataTypeHash groupObjectTable = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::GroupObjectTable::GroupObjectTableProperty,
            { { Id::Generic06, QKnxDatapointType::Type::Unknown, Unit::Array } } },
            { QKnxInterfaceObjectProperty::GroupObjectTable::ExtendedGroupObjectReference,
            { { Id::Generic08, QKnxDatapointType::Type::Unknown, Unit::Array } } }
        };
    }();
    if (QKnxInterfaceObjectProperty::isGroupObjectTableProperty(property))
        return groupObjectTable.value(property);

    static const PropertyDataTypeHash router = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::Router::LineStatus, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Router::MainLcConfig, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Router::SubLcConfig, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Router::MainLcGroupConfig, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Router::SubLcGroupConfig, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Router::RouteTableControl,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Router::CouplerServerControl, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::Router::MaxRouterApduLength,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Router::Medium,
                {
                    { Id::Enum8, QKnxDatapointType::Type::Dpt20_1Byte },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Router::FilterTableUse,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::Dpt1_1Bit },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Router::RfEnableSbc,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            }
        };
    }();
    if (QKnxInterfaceObjectProperty::isRouterProperty(property))
        return router.value(property);

    static const PropertyDataTypeHash netIp = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::ProjectInstallationId,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxIndividualAddress,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::AdditionalIndividualAddresses,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAssignmentMethod,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::IpAssignmentMethod,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::IpCapabilities,
                {
                    { Id::Bitset8, QKnxDatapointType::Type::Dpt21_8BitSet },
                    { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAddress,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentSubnetMask,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentDefaultGateway,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::IpAddress,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SubnetMask,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::DefaultGateway,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::DhcpBootpServer,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MacAddress, { { Id::Generic06 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SystemSetupMulticastAddress,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::RoutingMulticastAddress,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::Ttl,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpDeviceCapabilities,
                {
                    { Id::Bitset16, QKnxDatapointType::Type::Dpt22_16BitSet },
                    { Id::Generic02 }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpDeviceState,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::KnxNetIpRoutingCapabilities,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::PriorityFifoEnabled,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::Dpt1_1Bit },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::QueueOverflowToIp,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::QueueOverflowToKnx,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MsgTransmitToIp,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MsgTransmitToKnx,
                {
                    { Id::UnsignedLong, QKnxDatapointType::Type::Dpt12_4ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::FriendlyName,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::RoutingBusyWaitTime,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::BackboneKey, { { Id::Generic16 } } },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::DeviceAuthenticationCode,
                {
                    { Id::Generic16 }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::PasswordHashes,
                {
                    { Id::Generic16, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SecuredServiceFamilies,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::MulticastLatencyTolerance,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::Dpt7_2ByteUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::SyncLatencyFraction,
                {
                    { Id::Scaling, QKnxDatapointType::Type::DptScaling },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::KnxNetIpParameter::TunnellingUsers,
                {
                    { Id::Generic02, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            }
        };
    }();
    if (QKnxInterfaceObjectProperty::isKnxNetIpParameterProperty(property))
        return netIp.value(property);

    static const PropertyDataTypeHash cemiServer = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::CemiServer::MediumType,
                {
                    { Id::Bitset16, QKnxDatapointType::Type::DptMedia }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::CommonMode,
                {
                    { Id::Enum8, QKnxDatapointType::Type::DptCommandMode }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::MediumAvailability,
                {
                    { Id::Bitset16, QKnxDatapointType::Type::DptMedia }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::AdditionalInfoTypes,
                {
                    { Id::Enum8, QKnxDatapointType::Type::DptAdditionalInfoTypes, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::TimeBase,
                {
                    { Id::UnsignedInt, QKnxDatapointType::Type::DptValue2UCount }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::TransparencyModeEnabled,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::DptEnable }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::BiBatNextBlock,
                {
                    { Id::UnsignedChar, QKnxDatapointType::Type::DptValue1Ucount }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::RfModeSelect,
                {
                    { Id::Enum8 }, { Id::UnsignedChar }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::RfModeSupport,
                {
                    { Id::Bitset8 }, { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::RfFilteringModeSelectCemiServer,
                {
                    { Id::Enum8 }, { Id::UnsignedChar }
                }
            },
            { QKnxInterfaceObjectProperty::CemiServer::RfFilteringModeSupport,
                {
                    { Id::Bitset8 }, { Id::Generic01 }
                }
            }
        };
    }();
    if (QKnxInterfaceObjectProperty::isCemiServerProperty(property))
        return cemiServer.value(property);

    static const PropertyDataTypeHash security = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::Security::SecurityMode,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Security::P2pKeyTable,
                {
                    { Id::Generic18, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Security::GroupKeyTable,
                {
                    { Id::Generic18, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Security::SecurityIndividualAddressTable,
                {
                    { Id::Generic08, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Security::SecurityFailuresLog,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::Security::SkiTool, { { Id::Generic16 } } },
            { QKnxInterfaceObjectProperty::Security::SecurityReport,
                {
                    { Id::Bitset8, QKnxDatapointType::Type::Dpt21_8BitSet },
                    { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::Security::SecurityReportControl,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::Dpt1_1Bit },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::Security::SequenceNumberSending, { { Id::Generic06 } } },
            { QKnxInterfaceObjectProperty::Security::ZoneKeysTable,
                {
                    { Id::Generic19, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::Security::GoSecurityFlags,
                {
                    { Id::Generic01, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            }
        };
    }();
    if (QKnxInterfaceObjectProperty::isSecurityProperty(property))
        return security.value(property);

    static const PropertyDataTypeHash rfMedium = []() -> PropertyDataTypeHash {
        return {
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiType, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiPhysicalFeatures,
                {
                    { Id::Bitset8, QKnxDatapointType::Type::Dpt21_8BitSet },
                    { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiCallChannel, { { Id::Generic01 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiObjectLink, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::RfMedium::RfMultiExtendedGroupAddressRepeated, { /* TODO */ } },
            { QKnxInterfaceObjectProperty::RfMedium::RfDomainAddressRfMedium, { { Id::Generic06 } } },
            { QKnxInterfaceObjectProperty::RfMedium::RfRetransmitter,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::Dpt1_1Bit },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfSecurityReportControl,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::Dpt1_1Bit },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfFilteringModeSelectRfMedium,
                {
                    { Id::Bitset8, QKnxDatapointType::Type::Dpt21_8BitSet },
                    { Id::Generic01 }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfBidirTimeout,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfDiagSaFilterTable,
                {
                    { Id::Generic03, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfDiagQualityTable,
                {
                    { Id::Generic04, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::RfDiagProbe,
                {
                    { Id::Function },
                    { Id::VariableLength, QKnxDatapointType::Type::DptVariableString88591 }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::TransmissionMode,
                {
                    { Id::Enum8, QKnxDatapointType::Type::Dpt20_1Byte },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::ReceptionMode,
                {
                    { Id::Enum8, QKnxDatapointType::Type::Dpt20_1Byte },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::TestSignal, { { Id::Generic02 } } },
            { QKnxInterfaceObjectProperty::RfMedium::FastAcknowledge,
                {
                    { Id::Generic02, QKnxDatapointType::Type::Unknown, Unit::Array }
                }
            },
            { QKnxInterfaceObjectProperty::RfMedium::FastAcknowledgeActivate,
                {
                    { Id::BinaryInformation, QKnxDatapointType::Type::Dpt1_1Bit },
                    { Id::UnsignedChar, QKnxDatapointType::Type::Dpt5_8bitUnsigned }
                }
            }
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

#include "moc_qknxinterfaceobjectpropertydatatype.cpp"

QT_END_NAMESPACE
