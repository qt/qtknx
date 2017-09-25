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

#include "qknxnpdufactory.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNpduFactory

    \inmodule QtKnx
    \brief The QKnxNpduFactory class is used to create a valid Network protocol
    data unit to be used in an \l QKnxTunnelFrame.
*/

/*!
    Returns the Transport control field of the given \a npdu.
*/
QKnxCemi::TransportLayerControlField QKnxNpduFactory::transportLayerControlFied(const QKnxNpdu &npdu)
{
    if (npdu.size() < 2)
        return QKnxCemi::TransportLayerControlField::Error;
    return QKnxCemi::TransportLayerControlField(npdu.byte(1) & 0xfc); // mask out the first two bits
}

/*!
    Returns the Application layer control field of the given \a npdu.
*/
QKnxCemi::ApplicationLayerControlField
    QKnxNpduFactory::applicationLayerControlField(const QKnxNpdu &npdu)
{
    if (npdu.size() < 3)
        return QKnxCemi::ApplicationLayerControlField::Error;

    std::bitset<8> apciHigh = npdu.byte(1) & 0x03; // mask out all bits except the first two
    std::bitset<8> apciLow = npdu.byte(2) & 0xc0;  // mask out all bits except the last two

    const auto fourBitsApci = [&apciHigh, &apciLow]() {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), quint8(apciLow.to_ulong()) } };
        return QKnxCemi::ApplicationLayerControlField(QKnxUtils::QUint16::fromBytes(apciBytes));
    };
    const auto tenBitsApci = [apciHigh](quint8 octet7) {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), octet7 } };
        return QKnxCemi::ApplicationLayerControlField(QKnxUtils::QUint16::fromBytes(apciBytes));
    };

    if ((apciHigh[0] == 0 && apciHigh[1] == 0) || (apciHigh[0] == 1 && apciHigh[1] == 1)) {
        std::bitset<8> octet7 = npdu.byte(2);
        if (octet7[7] == 1 && octet7[6] == 1)
            return tenBitsApci(npdu.byte(2));
        return fourBitsApci();
    }

    if (apciHigh[1] == 0 && apciHigh[0] == 1) {
        // connection oriented, it's one of the A_ADC service
        if (quint8(transportLayerControlFied(npdu)) > 0)
            return fourBitsApci();
        return tenBitsApci(npdu.byte(2));
    }

    // it's one of the A_Memory Service (only the 2 last bits of octet 6 are needed for the apci)
    if (apciLow[7] == 0 || apciLow[6] == 0)
        return fourBitsApci();
    return tenBitsApci(npdu.byte(2));
}

/*!
    \table
    \header
        \li Multicast Application Services
        \li Currently available in QtKnx
    \row
        \li GroupValueRead
        \li YES
    \row
        \li GroupValueWrite
        \li YES
    \row
        \li GroupValueResponse
        \li NO
    \row
        \li GroupPropValueRead
        \li YES
    \row
        \li GroupPropValueWrite
        \li YES
    \row
        \li GroupPropValueResponse
        \li NO
    \row
        \li GroupPropValueInfoReport
        \li NO
    \endtable
*/

/*!
    Returns a NPDU for a Group Value Read Application Service.
*/
QKnxNpdu QKnxNpduFactory::createGroupValueReadNpdu()
{
    return createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataGroup,
        QKnxCemi::ApplicationLayerControlField::GroupValueRead);
}

/*!
    Returns a NPDU for a Group Value Write Application Service.
*/
QKnxNpdu QKnxNpduFactory::createGroupValueWriteNpdu(const QByteArray &data)
{
    if (data.size() < 2 && quint8(data[0]) < 64) {
        return createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataGroup,
            QKnxCemi::ApplicationLayerControlField::GroupValueWrite, quint8(data[0]));
    }

    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataGroup,
        QKnxCemi::ApplicationLayerControlField::GroupValueWrite);
    npdu.appendAndUpdate(data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createGroupPropValueReadNpdu(QKnxInterfaceObjectType objectType,
    quint8 objectInstance, QKnxInterfaceObjectProperty property)
{
    return createGroupPropValueNpdu(QKnxCemi::ApplicationLayerControlField::GroupPropValueRead,
        objectType, objectInstance, property);
}

QKnxNpdu QKnxNpduFactory::createGroupPropValueWriteNpdu(QKnxInterfaceObjectType objectType,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QByteArray &data)
{
    return createGroupPropValueNpdu(QKnxCemi::ApplicationLayerControlField::GroupPropValueWrite,
        objectType, objectInstance, property, data);
}

/*!
    \table
    \header
        \li Broadcast Application Services
        \li Currently available in QtKnx
    \row
        \li IndividualAddressRead
        \li YES
    \row
        \li IndividualAddressWrite
        \li YES
    \row
        \li IndividualAddressResponse
        \li NO
    \row
        \li IndividualAddressSerialNumberRead
        \li YES
    \row
        \li IndividualAddressSerialNumberWrite
        \li YES
    \row
        \li IndividualAddressSerialNumberResponse
        \li NO
    \row
        \li DomainAddressRead
        \li YES
    \row
        \li {1, 2} DomainAddressWrite
        \li YES (KNX-PL 110, 2 bites domain Address)
    \row
        \li NO (KNX-RF, 6 bites domain Address)
    \row
        \li DomainAddressResponse
        \li NO
    \row
        \li DomainAddressSelectiveRead
        \li NO
    \row
        \li DomainAddressSerialNumberRead
        \li YES
    \row
        \li {1, 2} DomainAddressSerialNumberWrite
        \li YES (KNX-PL 110, 2 bites domain Address)
    \row
        \li NO (KNX-RF, 6 bites domain Address)
    \row
        \li DomainAddressSerialNumberResponse
        \li NO
    \endtable
*/

/*!
    Returns a NPDU for Individual Address Read Application Service.
*/
QKnxNpdu QKnxNpduFactory::createIndividualAddressReadNpdu()
{
    return createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::IndividualAddressRead);
}

/*!
    Returns a NPDU for Individual Address Write Application Service with the
    given \l QKnxAddress \a newAddress; or an empty NPDU if the \a newAddress
    is not of type \l QKnxAddress::Type::Individual.
*/
QKnxNpdu QKnxNpduFactory::createIndividualAddressWriteNpdu(const QKnxAddress &newAddress)
{
    if (newAddress.type() != QKnxAddress::Type::Individual)
        return {};

    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::IndividualAddressWrite);
    npdu.appendAndUpdate(newAddress.bytes());
    return npdu;
}

/*!
    Returns a NPDU for Individual Address Serial Number Read Application Service
    with the given \a serialNumber set; or an empty NPDU if the \a serialNumber
    has a different size then six octets.
*/
QKnxNpdu QKnxNpduFactory::createIndividualAddressSerialNumberReadNpdu(const QByteArray &serialNumber)
{
    if (serialNumber.size() != 6)
        return {};

    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::IndividualAddressSerialNumberRead);
    npdu.appendAndUpdate(serialNumber);
    return npdu;
}

/*!
    Returns a NPDU for Individual Address Serial Number Write Application Service
    with the given \a serialNumber and \l QKnxAddress \a newAddress set; or
    otherwise an empty NPDU if the \a serialNumber has a different size then
    six octets or the type of \a newAddress is not \l QKnxAddress::Type::Individual.
*/
QKnxNpdu QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(const QByteArray &serialNumber,
    const QKnxAddress &newAddress)
{
    if ((serialNumber.size() != 6) || (newAddress.type() != QKnxAddress::Type::Individual))
        return {};

    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::IndividualAddressSerialNumberWrite);
    npdu.appendAndUpdate(serialNumber + newAddress.bytes());
    return npdu;
}

/*!
    Returns a NPDU for Domain Address Read Application Service.
*/
QKnxNpdu QKnxNpduFactory::createDomainAddressReadNpdu()
{
    return createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::DomainAddressRead);
}

/*!
    Returns a NPDU for Domain Address Write Application Service with the given
    \l QKnxAddress \a domainAddress set.
*/
QKnxNpdu QKnxNpduFactory::createDomainAddressWriteNpdu(const QKnxAddress &domainAddress)
{
    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::DomainAddressWrite);
    npdu.appendAndUpdate(domainAddress.bytes());
    return npdu;
}

/*
QKnxNpdu QKnxNpduFactory::createDomainAddressSelectiveReadNpdu()
{
    // TODO: this one is more complicated than that
    auto npdu = createNpduOctets6And7(QKnxCemi::TransportControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::DomainAddressRead);
    // append the ASDU (?)
    return npdu;
}
*/

/*!
    Returns a NPDU for Domain Address Serial Number Read Application Service
    with the given \a serialNumber set; or an empty NPDU if the \a serialNumber
    has a different size then six octets.
*/
QKnxNpdu QKnxNpduFactory::createDomainAddressSerialNumberReadNpdu(const QByteArray &serialNumber)
{
    if (serialNumber.size() != 6)
        return {};

    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::DomainAddressSerialNumberRead);
    npdu.appendAndUpdate(serialNumber);
    return npdu;
}

/*!
    Returns a NPDU for Domain Address Serial Number Write Application Service
    with the given \a serialNumber and \l QKnxAddress \a domainAddress set;
    or otherwise an empty NPDU if the \a serialNumber has a different size then
    six octets.
*/
QKnxNpdu QKnxNpduFactory::createDomainAddressSerialNumberWriteNpdu(const QByteArray &serialNumber,
    const QKnxAddress &domainAddress)
{
    if (serialNumber.size() != 6)
        return {};

    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataBroadcast,
        QKnxCemi::ApplicationLayerControlField::DomainAddressSerialNumberWrite);
    npdu.appendAndUpdate(serialNumber + domainAddress.bytes());
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createNetworkParameterWriteNpdu(Network type,
    QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property,
    const QByteArray &value)
{
    auto field = (type == Network::System
        ? QKnxCemi::ApplicationLayerControlField::SystemNetworkParameterWrite
        : QKnxCemi::ApplicationLayerControlField::NetworkParameterWrite);
    return createNetworkParameterNpdu(field, objectType, property, value, type);
}

QKnxNpdu QKnxNpduFactory::createPropertyValueReadNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex, quint8 seqNumber)
{
    return createPropertyValueReadWriteNpdu(objectIndex, property, nbElement, startIndex,
        seqNumber, QKnxCemi::ApplicationLayerControlField::PropertyValueRead);
}

QKnxNpdu QKnxNpduFactory::createPropertyValueWriteNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    const QByteArray &data, quint8 seqNumber)
{
    return createPropertyValueReadWriteNpdu(objectIndex, property, nbElement, startIndex,
        seqNumber, QKnxCemi::ApplicationLayerControlField::PropertyValueWrite, data);
}

QKnxNpdu QKnxNpduFactory::createPropertyDescriptionReadNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 propertyIndex, quint8 seqNumber)
{
    // TODO: Check if the property belongs to an interface object.

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci,
        QKnxCemi::ApplicationLayerControlField::PropertyDescriptionRead);
    npdu.setByte(npdu.size(), objectIndex);
    npdu.setByte(npdu.size(), quint8(property));
    npdu.setByte(npdu.size(), propertyIndex);
    npdu.setByte(0, quint8(npdu.size() - 2));
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createFunctionPropertyStateReadNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 seqNumber)
{
    return createFunctionPropertyNpdu(
        QKnxCemi::ApplicationLayerControlField::FunctionPropertyStateRead,
        objectIndex, property, data, seqNumber);
}

QKnxNpdu QKnxNpduFactory::createFunctionPropertyCommandNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 seqNumber)
{
    return createFunctionPropertyNpdu(
        QKnxCemi::ApplicationLayerControlField::FunctionPropertyCommand, objectIndex,
        property, data, seqNumber);
}

QKnxNpdu QKnxNpduFactory::createLinkReadNpdu(quint8 groupObjectNumber, quint8 startIndex,
    quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::LinkRead);
    npdu.setByte(npdu.size(), groupObjectNumber);
    npdu.setByte(npdu.size(), startIndex);
    npdu.setByte(0, quint8(npdu.size() - 2));
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createLinkWriteNpdu(quint8 groupObjectNumber, quint8 flags,
    const QKnxAddress &groupAddress, quint8 seqNumber)
{
    if (groupAddress.type() != QKnxAddress::Type::Group)
        return {};

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::LinkWrite);
    npdu.setByte(npdu.size(), groupObjectNumber);
    npdu.setByte(npdu.size(), flags);
    npdu.appendAndUpdate(groupAddress.bytes());
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createDeviceDescriptorReadNpdu(quint8 descriptorType, quint8 seqNumber)
{
    if (descriptorType >= 64)
        return {};

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    // Device Descriptor can be of type 0 or 2 (cf Glossary)
    // but if not supported just receive an answer with error message
    return createNpduOctets6And7(tpci,
        QKnxCemi::ApplicationLayerControlField::DeviceDescriptorRead, descriptorType);
}

QKnxNpdu QKnxNpduFactory::createNetworkParameterReadNpdu(Network type,
    QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property,
    const QByteArray &testInfo)
{
    auto field = (type == Network::System
        ? QKnxCemi::ApplicationLayerControlField::SystemNetworkParameterRead
        : QKnxCemi::ApplicationLayerControlField::NetworkParameterRead);
    return createNetworkParameterNpdu(field, objectType, property, testInfo, type);
}

/*!
  \table
  \header
    \li System Broadcast Application Services
    \li Currently available in QtKnx
  \row
    \li DeviceDescriptorInfoReport
    \li NO
  \row
    \li SystemNetworkParameterRead
    \li YES
  \row
    \li SystemNetworkParameterWrite
    \li YES
  \row
    \li SystemNetworkParameterResponse
    \li NO
  \endtable
*/

/*!
  \table
  \header
    \li Point to point connection less Application Services
    \li Currently available in QtKnx
  \row
    \li NetworkParameterRead
    \li YES
  \row
    \li NetworkParameterWrite
    \li YES
  \row
    \li NetworkParameterResponse
    \li NO
  \endtable
*/

/*!
  \table
  \header
    \li Point to point connection less or connection oriented Application Services
    \li Currently available in QtKnx
  \row
    \li PropertyValueRead
    \li YES
  \row
    \li PropertyValueWrite
    \li YES
  \row
    \li PropertyValueResponse
    \li NO
  \row
    \li PropertyDescriptionRead
    \li YES
  \row
    \li PropertyDescriptionResponse
    \li NO
  \row
    \li FunctionPropertyCommand
    \li YES
  \row
    \li FunctionPropertyStateRead
    \li YES
  \row
    \li FunctionPropertyStateResponse
    \li NO
  \row
    \li LinkRead
    \li YES
  \row
    \li LinkWrite
    \li YES
  \row
    \li LinkResponse
    \li NO
  \row
    \li DeviceDescriptorRead
    \li YES
  \row
    \li DeviceDescriptorResponse
    \li NO
  \row
    \li Restart
    \li NO
  \endtable
*/

/*!
  \table
  \header
    \li Point to point connection connection oriented Application Services
    \li Currently available in QtKnx
  \row
    \li AdcRead
    \li YES
  \row
    \li AdcResponse
    \li NO
  \row
    \li MemoryRead
    \li YES
  \row
    \li MemoryResponse
    \li NO
  \row
    \li MemoryWrite
    \li YES
  \row
    \li MemoryBitWrite
    \li NO
  \row
    \li UserMemoryRead
    \li YES
  \row
    \li UserMemoryResponse
    \li NO
  \row
    \li UserMemoryResponse
    \li NO
  \row
    \li UserMemoryWrite
    \li YES
  \row
    \li UserMemoryBitWrite
    \li NO
  \row
    \li UserManufacturerInfoRead
    \li YES
  \row
    \li UserManufacturerInfoResponse
    \li NO
  \row
    \li AuthorizeRequest
    \li NO
  \row
    \li AuthorizeResponse
    \li NO
  \row
    \li KeyWrite
    \li NO
  \row
    \li KeyResponse
    \li NO
  \endtable
*/

/*!
    Returns a NPDU for User Manufacturer Info Read Application Service with the
    given sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::createUserManufacturerInfoReadNpdu(quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    return createNpduOctets6And7(tpci,
        QKnxCemi::ApplicationLayerControlField::UserManufacturerInfoRead);
}

/*!
    Returns a NPDU for Memory Read Application Service with the given \a number,
    \a address and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::createMemoryReadNpdu(quint8 number, const QKnxAddress &address,
    quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::MemoryRead,
        number);
    if (npdu.size() > 0)
        npdu.appendAndUpdate(address.bytes());
    return npdu;
}

/*!
    Returns a NPDU for Memory Write Application Service with the given \a number,
    \a address, \a data and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::createMemoryWriteNpdu(quint8 number, const QKnxAddress &address,
    const QByteArray &data, quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::MemoryWrite,
        number);
    if (npdu.size() > 0) {
        npdu.appendAndUpdate(address.bytes());
        npdu.appendAndUpdate(data);
    }
    return npdu;
}

/*!
    Returns a NPDU for User Memory Read Application Service with \a number,
    \a address, \a data and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::createUserMemoryReadNpdu(quint8 addressExtention, quint8 number,
    const QKnxAddress &address, quint8 seqNumber)
{
    return createUserMemoryReadWriteNpdu(addressExtention, number, address, seqNumber);
}

/*!
    Returns a NPDU for User Memory Write Application Service with
    \a addressExtention, \a number, \a address, \a data and \a seqNumber
    set.
*/
QKnxNpdu QKnxNpduFactory::createUserMemoryWriteNpdu(quint8 addressExtention, quint8 number,
    const QKnxAddress &address, const QByteArray &data, quint8 seqNumber)
{
    return createUserMemoryReadWriteNpdu(addressExtention, number, address, seqNumber, data);
}

/*!
    Returns a NPDU for ADC Read Application Service with the given sequence
    number \a seqNumber, \a channelNumber and \a readCount set.
*/
QKnxNpdu QKnxNpduFactory::createAdcReadNpdu(quint8 seqNumber, quint8 channelNumber,
    quint8 readCount)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    // Device Descriptor can be of type 0 or 2 (cf Glossary)
    // but if not supported just receive an answer with error message
    auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::AdcRead,
        channelNumber);
    if (npdu.size() > 0) {
        npdu.setByte(npdu.size(), readCount);
        npdu.setByte(0, quint8(npdu.size() - 2));
    }
    return npdu;
}

// -- private


QByteArray QKnxNpduFactory::parameterType(QKnxInterfaceObjectType objectType,
    QKnxInterfaceObjectProperty property)
{
    if (!QKnxInterfaceObjectType::isMatch(objectType, property))
        return {};
    return QKnxUtils::QUint16::bytes<QByteArray>(quint16(objectType)).append(quint8(property));
}

/*!
    Returns the Transport control field with the sequence number \a seqNumber set.
*/
QKnxCemi::TransportLayerControlField QKnxNpduFactory::buildTransportControlField(quint8 seqNumber)
{
    if (seqNumber > 15)
        return QKnxCemi::TransportLayerControlField::Error;

    if (seqNumber == 0)
        return QKnxCemi::TransportLayerControlField::DataIndividual;

    auto connected = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    return QKnxCemi::TransportLayerControlField(connected | (seqNumber << 2));
}

QKnxNpdu QKnxNpduFactory::createGroupPropValueNpdu(QKnxCemi::ApplicationLayerControlField apci,
    QKnxInterfaceObjectType objectType, quint8 objectInstance, QKnxInterfaceObjectProperty property,
    const QByteArray &data)
{
    QByteArray bytes = parameterType(objectType, property);
    if (bytes.size() == 0)
        return {};

    // parameter type = Object Type + PID
    // For GroupPropValueRead we need Object Type + Object Instance + PID
    // parameter type is build (and checked first) then the Object Instance is inserted
    bytes.insert(2, objectInstance);
    // if objectInstance is 0, it means all instance (10/1 Logical Tag Extended p81)
    auto npdu = createNpduOctets6And7(QKnxCemi::TransportLayerControlField::DataTagGroup, apci);
    npdu.appendAndUpdate(bytes);
    if (data.size() > 0)
        npdu.appendAndUpdate(data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createNetworkParameterNpdu(QKnxCemi::ApplicationLayerControlField apci,
    QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property,
    const QByteArray &testInfoValue, Network type)
{
    // testInfo: Value against which the resource indicated by parameterType is tested
    QByteArray bytes = parameterType(objectType, property);
    if (bytes.size() == 0)
        return {};

    auto npdu = createNpduOctets6And7((type == Network::System
        ? QKnxCemi::TransportLayerControlField::DataBroadcast
        : QKnxCemi::TransportLayerControlField::DataIndividual), apci);

    //--- part PID part test_info reserved: Setting the whole octet 11 to zero---
    if (type == Network::System)
        bytes.append(QByteArray::fromHex("00"));
    bytes.append(testInfoValue);
    npdu.appendAndUpdate(bytes);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createPropertyValueReadWriteNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    quint8 seqNumber, QKnxCemi::ApplicationLayerControlField apci, const QByteArray &data)
{
    // This is for Interface Object (reduced one: 3/4/1 p16)
    // TODO: Check if the property belongs to an interface object.

    if (apci == QKnxCemi::ApplicationLayerControlField::PropertyValueRead && data.size() != 0)
        return {};
    if (apci == QKnxCemi::ApplicationLayerControlField::PropertyValueWrite && data.size() == 0)
        return {};

    // objectIndex is relative to the device. Different from the ObjectType!!!!
    // Before arriving here the property should probably be checked against the objectType of
    // the object with index objectIndex.
    // Does not need to be checked at this level:
    // ApplicationLayer p52. if it does not fit the response is empty

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci, apci);
    npdu.setByte(npdu.size(), objectIndex);
    npdu.setByte(npdu.size(), quint8(property));

    auto bytes = QKnxUtils::QUint16::bytes(quint16(startIndex));
    bytes[0] = quint8(quint8(bytes[0]) | (nbElement << 4));

    npdu.appendAndUpdate(bytes);
    if (apci == QKnxCemi::ApplicationLayerControlField::PropertyValueWrite)
        npdu.appendAndUpdate(data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createFunctionPropertyNpdu(QKnxCemi::ApplicationLayerControlField apci,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, const QByteArray &data,
    quint8 seqNumber)
{
    // TODO: Figure out what is data
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto npdu = createNpduOctets6And7(tpci, apci);
    npdu.setByte(npdu.size(), objectIndex);
    npdu.setByte(npdu.size(), quint8(property));
    npdu.appendAndUpdate(data);
    return npdu;
}

/*!
    Returns the three first bytes of the NPDU.

    The first byte contains the NPDU size starting count. The second byte
    contains the Transport Control Field together with the hight part of the
    Application layer control field. The third byte contains the low part of
    the Application layer control field together with, in some cases, data.
*/
QKnxNpdu QKnxNpduFactory::createNpduOctets6And7(QKnxCemi::TransportLayerControlField tpci,
    QKnxCemi::ApplicationLayerControlField apci, quint8 data)
{
    if (data >= 64) // data has to be less then 64
        return {};

    auto bytes = QKnxUtils::QUint16::bytes<QVector<quint8>>(quint16(apci));

    QKnxNpdu npdu;
    npdu.setByte(1, bytes[0] | quint8(tpci)); // set TPCI
    npdu.setByte(2, (data > 0 ? bytes[1] | data : bytes[1]));  // set data in APCI-DATA
    npdu.setByte(0, quint8(npdu.size() - 2)); // Length (starts counting after TPCI-APCI octet)
    return npdu;
}

/*!
    Returns a NPDU for User Memory Read or Write Application Service with
    \a addressExtention, \a number, \a address, \a data and \a seqNumber
    set.
*/
QKnxNpdu QKnxNpduFactory::createUserMemoryReadWriteNpdu(quint8 addressExtention, quint8 number,
    const QKnxAddress &address, quint8 seqNumber, const QByteArray &data)
{
    if (number > 15 || addressExtention > 15)
        return {};

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxCemi::TransportLayerControlField::Error)
        return {};

    auto apci = ((data.size() == 0) ? QKnxCemi::ApplicationLayerControlField::UserMemoryRead
        : QKnxCemi::ApplicationLayerControlField::UserMemoryWrite);

    auto npdu = createNpduOctets6And7(tpci, apci);
    npdu.setByte(npdu.size(), number | (addressExtention << 4));
    npdu.appendAndUpdate(address.bytes());
    if (data.size() > 0)
        npdu.appendAndUpdate(data);
    return npdu;
}

QT_END_NAMESPACE
