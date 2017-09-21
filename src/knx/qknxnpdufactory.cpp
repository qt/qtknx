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
    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataGroup,
        QKnxNpdu::ApplicationLayerControlField::GroupValueRead);
}

/*!
    Returns a NPDU for a Group Value Write Application Service.
*/
QKnxNpdu QKnxNpduFactory::createGroupValueWriteNpdu(const QByteArray &data)
{
    if (data.size() < 2 && quint8(data[0]) < 64) {
        return setupNpdu(QKnxNpdu::TransportLayerControlField::DataGroup,
            QKnxNpdu::ApplicationLayerControlField::GroupValueWrite, { 1, data[0] });
    }

    auto npdu = setupNpdu(QKnxNpdu::TransportLayerControlField::DataGroup,
        QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
    npdu.setData(data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createGroupPropValueReadNpdu(QKnxInterfaceObjectType objectType,
    quint8 objectInstance, QKnxInterfaceObjectProperty property)
{
    return createGroupPropValueNpdu(QKnxNpdu::ApplicationLayerControlField::GroupPropValueRead,
        objectType, objectInstance, property);
}

QKnxNpdu QKnxNpduFactory::createGroupPropValueWriteNpdu(QKnxInterfaceObjectType objectType,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QByteArray &data)
{
    return createGroupPropValueNpdu(QKnxNpdu::ApplicationLayerControlField::GroupPropValueWrite,
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
    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressRead);
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

    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressWrite, newAddress.bytes());
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

    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressSerialNumberRead, serialNumber);
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

    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressSerialNumberWrite, serialNumber
        + newAddress.bytes());
}

/*!
    Returns a NPDU for Domain Address Read Application Service.
*/
QKnxNpdu QKnxNpduFactory::createDomainAddressReadNpdu()
{
    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::DomainAddressRead);
}

/*!
    Returns a NPDU for Domain Address Write Application Service with the given
    \l QKnxAddress \a domainAddress set.
*/
QKnxNpdu QKnxNpduFactory::createDomainAddressWriteNpdu(const QKnxAddress &domainAddress)
{
    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::DomainAddressWrite, domainAddress.bytes());
}

/*
QKnxNpdu QKnxNpduFactory::createDomainAddressSelectiveReadNpdu()
{
    // TODO: this one is more complicated than that
    auto npdu = createNpduOctets6And7(QKnxCemi::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::DomainAddressRead);
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

    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::DomainAddressSerialNumberRead, serialNumber);
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

    return setupNpdu(QKnxNpdu::TransportLayerControlField::DataBroadcast,
        QKnxNpdu::ApplicationLayerControlField::DomainAddressSerialNumberWrite, serialNumber
        + domainAddress.bytes());
}

QKnxNpdu QKnxNpduFactory::createNetworkParameterWriteNpdu(Network type,
    QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property,
    const QByteArray &value)
{
    auto field = (type == Network::System
        ? QKnxNpdu::ApplicationLayerControlField::SystemNetworkParameterWrite
        : QKnxNpdu::ApplicationLayerControlField::NetworkParameterWrite);
    return createNetworkParameterNpdu(field, objectType, property, value, type);
}

QKnxNpdu QKnxNpduFactory::createPropertyValueReadNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex, quint8 seqNumber)
{
    return createPropertyValueReadWriteNpdu(objectIndex, property, nbElement, startIndex,
        seqNumber, QKnxNpdu::ApplicationLayerControlField::PropertyValueRead);
}

QKnxNpdu QKnxNpduFactory::createPropertyValueWriteNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    const QByteArray &data, quint8 seqNumber)
{
    return createPropertyValueReadWriteNpdu(objectIndex, property, nbElement, startIndex,
        seqNumber, QKnxNpdu::ApplicationLayerControlField::PropertyValueWrite, data);
}

QKnxNpdu QKnxNpduFactory::createPropertyDescriptionReadNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 propertyIndex, quint8 seqNumber)
{
    // TODO: Check if the property belongs to an interface object.

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    auto npdu = setupNpdu(tpci,
        QKnxNpdu::ApplicationLayerControlField::PropertyDescriptionRead);
    QByteArray data(3, Qt::Uninitialized);
    data[0] = quint8(objectIndex);
    data[1] = quint8(property);
    data[2] = quint8(propertyIndex);
    npdu.setData(data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createFunctionPropertyStateReadNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 seqNumber)
{
    return createFunctionPropertyNpdu(
        QKnxNpdu::ApplicationLayerControlField::FunctionPropertyStateRead,
        objectIndex, property, data, seqNumber);
}

QKnxNpdu QKnxNpduFactory::createFunctionPropertyCommandNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 seqNumber)
{
    return createFunctionPropertyNpdu(
        QKnxNpdu::ApplicationLayerControlField::FunctionPropertyCommand, objectIndex,
        property, data, seqNumber);
}

QKnxNpdu QKnxNpduFactory::createLinkReadNpdu(quint8 groupObjectNumber, quint8 startIndex,
    quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    auto npdu = setupNpdu(tpci, QKnxNpdu::ApplicationLayerControlField::LinkRead);
    QByteArray data(2, Qt::Uninitialized);
    data[0] = quint8(groupObjectNumber);
    data[1] = quint8(startIndex);
    npdu.setData(data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createLinkWriteNpdu(quint8 groupObjectNumber, quint8 flags,
    const QKnxAddress &groupAddress, quint8 seqNumber)
{
    if (groupAddress.type() != QKnxAddress::Type::Group)
        return {};

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    auto npdu = setupNpdu(tpci, QKnxNpdu::ApplicationLayerControlField::LinkWrite);
    QByteArray data(2, Qt::Uninitialized);
    data[0] = quint8(groupObjectNumber);
    data[1] = quint8(flags);
    npdu.setData(QByteArray(data + groupAddress.bytes()));
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createDeviceDescriptorReadNpdu(quint8 descriptorType, quint8 seqNumber)
{
    if (descriptorType >= 64)
        return {};

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    // Device Descriptor can be of type 0 or 2 (cf Glossary)
    // but if not supported just receive an answer with error message
    return setupNpdu(tpci,
        QKnxNpdu::ApplicationLayerControlField::DeviceDescriptorRead, { 1, char(descriptorType) });
}

QKnxNpdu QKnxNpduFactory::createNetworkParameterReadNpdu(Network type,
    QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property,
    const QByteArray &testInfo)
{
    auto field = (type == Network::System
        ? QKnxNpdu::ApplicationLayerControlField::SystemNetworkParameterRead
        : QKnxNpdu::ApplicationLayerControlField::NetworkParameterRead);
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
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    return setupNpdu(tpci,
        QKnxNpdu::ApplicationLayerControlField::UserManufacturerInfoRead);
}

/*!
    Returns a NPDU for Memory Read Application Service with the given \a number,
    \a address and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::createMemoryReadNpdu(quint8 number, const QKnxAddress &address,
    quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    return setupNpdu(tpci, QKnxNpdu::ApplicationLayerControlField::MemoryRead,
        QByteArray(1, char(number)) + address.bytes());
}

/*!
    Returns a NPDU for Memory Write Application Service with the given \a number,
    \a address, \a data and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::createMemoryWriteNpdu(quint8 number, const QKnxAddress &address,
    const QByteArray &data, quint8 seqNumber)
{
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    return setupNpdu(tpci, QKnxNpdu::ApplicationLayerControlField::MemoryWrite,
        QByteArray(1, char(number)) + address.bytes() + data);
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
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    // Device Descriptor can be of type 0 or 2 (cf Glossary)
    // but if not supported just receive an answer with error message
    auto npdu = setupNpdu(tpci, QKnxNpdu::ApplicationLayerControlField::AdcRead);
    if (npdu.isValid())
        npdu.setData(QByteArray(1, char(channelNumber)) + QByteArray(1, char(readCount)));
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
QKnxNpdu::TransportLayerControlField QKnxNpduFactory::buildTransportControlField(quint8 seqNumber)
{
    if (seqNumber > 15)
        return QKnxNpdu::TransportLayerControlField::Invalid;

    if (seqNumber == 0)
        return QKnxNpdu::TransportLayerControlField::DataIndividual;

    auto connected = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    return QKnxNpdu::TransportLayerControlField(connected | (seqNumber << 2));
}

QKnxNpdu QKnxNpduFactory::createGroupPropValueNpdu(QKnxNpdu::ApplicationLayerControlField apci,
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
    auto npdu = setupNpdu(QKnxNpdu::TransportLayerControlField::DataTagGroup, apci,
        bytes + data);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createNetworkParameterNpdu(QKnxNpdu::ApplicationLayerControlField apci,
    QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property,
    const QByteArray &testInfoValue, Network type)
{
    // testInfo: Value against which the resource indicated by parameterType is tested
    QByteArray bytes = parameterType(objectType, property);
    if (bytes.size() == 0)
        return {};

    auto npdu = setupNpdu((type == Network::System
        ? QKnxNpdu::TransportLayerControlField::DataBroadcast
        : QKnxNpdu::TransportLayerControlField::DataIndividual), apci);
    if (!npdu.isValid())
        return npdu;

    //--- part PID part test_info reserved: Setting the whole octet 11 to zero---
    if (type == Network::System)
        bytes.append(QByteArray::fromHex("00"));
    bytes.append(testInfoValue);
    npdu.setData(bytes);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createPropertyValueReadWriteNpdu(quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    quint8 seqNumber, QKnxNpdu::ApplicationLayerControlField apci, const QByteArray &data)
{
    // This is for Interface Object (reduced one: 3/4/1 p16)
    // TODO: Check if the property belongs to an interface object.

    if (apci == QKnxNpdu::ApplicationLayerControlField::PropertyValueRead && data.size() != 0)
        return {};
    if (apci == QKnxNpdu::ApplicationLayerControlField::PropertyValueWrite && data.size() == 0)
        return {};

    // objectIndex is relative to the device. Different from the ObjectType!!!!
    // Before arriving here the property should probably be checked against the objectType of
    // the object with index objectIndex.
    // Does not need to be checked at this level:
    // ApplicationLayer p52. if it does not fit the response is empty

    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    auto npdu = setupNpdu(tpci, apci);
    if (!npdu.isValid())
        return npdu;

    auto bytes = QKnxUtils::QUint16::bytes(quint16(startIndex));
    bytes.append(char(objectIndex));
    bytes.append(char(property));

    bytes[0] = quint8(quint8(bytes[0]) | (nbElement << 4));

    if (apci == QKnxNpdu::ApplicationLayerControlField::PropertyValueWrite)
        bytes += data;
    npdu.setData(bytes);
    return npdu;
}

QKnxNpdu QKnxNpduFactory::createFunctionPropertyNpdu(QKnxNpdu::ApplicationLayerControlField apci,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, const QByteArray &data,
    quint8 seqNumber)
{
    // TODO: Figure out what is data
    auto tpci = buildTransportControlField(seqNumber);
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    auto npdu = setupNpdu(tpci, apci);
    if (!npdu.isValid())
        return npdu;

    QByteArray value = data;
    value.prepend(char(property));
    value.prepend(char(objectIndex));
    npdu.setData(value);
    return npdu;
}

/*!
    Returns the three first bytes of the NPDU.

    The first byte contains the NPDU size starting count. The second byte
    contains the Transport Control Field together with the hight part of the
    Application layer control field. The third byte contains the low part of
    the Application layer control field together with, in some cases, data.
*/
QKnxNpdu QKnxNpduFactory::setupNpdu(QKnxNpdu::TransportLayerControlField tpci,
    QKnxNpdu::ApplicationLayerControlField apci, const QByteArray &data)
{
    QKnxNpdu npdu(tpci, apci);
    if (npdu.setData(data))
        return npdu;
    return {};
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
    if (tpci == QKnxNpdu::TransportLayerControlField::Invalid)
        return {};

    auto apci = ((data.size() == 0) ? QKnxNpdu::ApplicationLayerControlField::UserMemoryRead
        : QKnxNpdu::ApplicationLayerControlField::UserMemoryWrite);

    auto npdu = setupNpdu(tpci, apci);
    if (!npdu.isValid())
        return npdu;

    QByteArray value = data;
    value.prepend(address.bytes());
    value.prepend(char(number | (addressExtention << 4)));
    npdu.setData(value);
    return npdu;
}

QT_END_NAMESPACE
