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
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNpduFactory

    \inmodule QtKnx
    \brief The QKnxNpduFactory class is used to create a valid Network protocol
    data unit to be used in an \l QKnxTunnelFrame.
*/

static QKnxNpdu::TransportControlField tpci(QKnxNpduFactory::PointToPoint::Mode mode, quint8 seq)
{
    if (seq > 15)
        return QKnxNpdu::TransportControlField::Invalid;

    if (mode == QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented)
        return QKnxNpdu::TransportControlField::DataConnected;
    return QKnxNpdu::TransportControlField::DataIndividual;
}


// -- PointToPoint

/*!
    Returns a NPDU for Memory Read Application Service with the given \a number,
    \a address and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::PointToPoint::createMemoryReadNpdu(Mode mode, quint8 number,
    quint16 address, quint8 seqNumber)
{
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::MemoryRead, seqNumber,
        QKnxUtils::QUint8::bytes(number) + QKnxUtils::QUint16::bytes(address) };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createMemoryResponseNpdu(Mode mode, quint8 number,
    quint16 address, const QByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::MemoryResponse, seqNumber,
        QKnxUtils::QUint8::bytes(number) + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    Returns a NPDU for Memory Write Application Service with the given \a number,
    \a address, \a data and sequence number \a seqNumber set.
*/
QKnxNpdu QKnxNpduFactory::PointToPoint::createMemoryWriteNpdu(Mode mode, quint8 number,
    quint16 address, const QByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::MemoryWrite, seqNumber,
        QKnxUtils::QUint8::bytes(number) + QKnxUtils::QUint16::bytes(address) + data };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createFunctionPropertyCommandNpdu(Mode mode,
    quint8 objIndex, QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::FunctionPropertyCommand,
        seqNumber, QKnxUtils::QUint8::bytes(objIndex) + QKnxUtils::QUint8::bytes(property) + data };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createFunctionPropertyStateReadNpdu(Mode mode,
    quint8 objIndex, QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::FunctionPropertyStateRead,
        seqNumber, QKnxUtils::QUint8::bytes(objIndex) + QKnxUtils::QUint8::bytes(property) + data };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createFunctionPropertyStateResponseNpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, QKnxNpdu::ErrorCode code,
    const QByteArray &data, quint8 seqNumber)
{
    if (data.size() > 250)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 5 Bytes already taken

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::FunctionPropertyStateResponse,
        seqNumber, QKnxUtils::QUint8::bytes(objectIndex) + QKnxUtils::QUint8::bytes(property)
        + QKnxUtils::QUint8::bytes(quint8(code)) + data };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createDeviceDescriptorReadNpdu(Mode mode,
    quint8 descriptorType, quint8 seqNumber)
{
    if (descriptorType >= 64)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::DeviceDescriptorRead,
        seqNumber, QKnxUtils::QUint8::bytes(descriptorType) };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createDeviceDescriptorResponseNpdu(Mode mode,
    quint8 descriptorType, const QByteArray &deviceDescriptor, quint8 seqNumber)
{
    if (descriptorType >= 64 || deviceDescriptor.size() > 254)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::DeviceDescriptorResponse,
        seqNumber, QKnxUtils::QUint8::bytes(descriptorType)  + deviceDescriptor };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createRestartNpdu(Mode mode, QKnxNpdu::ResetType type,
    QKnxNpdu::EraseCode eraseCode, quint8 channelNumber, quint8 seqNumber)
{
    if (type == QKnxNpdu::ResetType::BasicRestart)
        return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::Restart, seqNumber };

    if (eraseCode == QKnxNpdu::EraseCode::Reserved || eraseCode >= QKnxNpdu::EraseCode::Invalid)
        return {};

    if ((eraseCode == QKnxNpdu::EraseCode::ConfirmedRestart
        || eraseCode == QKnxNpdu::EraseCode::ResetIa
        || eraseCode == QKnxNpdu::EraseCode::ResetAp) && channelNumber != 0x00) {
        return {};
    }

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::Restart, seqNumber,
        QKnxUtils::QUint8::bytes(0x01) + QKnxUtils::QUint8::bytes(quint8(eraseCode))
        + QKnxUtils::QUint8::bytes(channelNumber) };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createRestartResponseNpdu(Mode mode, QKnxNpdu::ResetType type,
    QKnxNpdu::ErrorCode code, quint16 processTime, quint8 seqNumber)
{
    if (type == QKnxNpdu::ResetType::BasicRestart)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::Restart, seqNumber,
        QKnxUtils::QUint8::bytes(0x21) + QKnxUtils::QUint8::bytes(quint8(code))
        + QKnxUtils::QUint16::bytes(processTime) };
}

static QKnxNpdu createPropertyValueNpdu(QKnxNpduFactory::PointToPoint::Mode mode, quint8 seqNumber,
                                        QKnxNpdu::ApplicationControlField apci,
                                        quint8 objectIndex, quint8 property, quint8 nbElement,
                                        quint16 startIndex, const QByteArray &data = {})
{
    if (data.size() > 249) // L_Data_Extended -> max 254 bytes payload
        return {}; // 6 bytes already used for APCI, object index, PID etc.

    if ((nbElement > 0x0f) || (startIndex > 0x0fff))
        return {};

    auto tmp = QKnxUtils::QUint16::bytes(startIndex);
    tmp[0] = quint8(quint8(nbElement << 4) | quint8(tmp[0]));
    return { tpci(mode, seqNumber), apci, seqNumber, QKnxUtils::QUint8::bytes(objectIndex)
        + QKnxUtils::QUint8::bytes(property) + tmp + data };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createPropertyValueReadNpdu(Mode mode, quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex, quint8 seqNumber)
{
    return createPropertyValueNpdu(mode, seqNumber,
        QKnxNpdu::ApplicationControlField::PropertyValueRead, objectIndex, quint8(property),
            nbElement, startIndex);
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createPropertyValueResponseNpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    const QByteArray &data, quint8 seqNumber)
{
    return createPropertyValueNpdu(mode, seqNumber,
        QKnxNpdu::ApplicationControlField::PropertyValueResponse, objectIndex, quint8(property),
            nbElement, startIndex, data);
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createPropertyValueWriteNpdu(Mode mode, quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    const QByteArray &data, quint8 seqNumber)
{
    return createPropertyValueNpdu(mode, seqNumber,
        QKnxNpdu::ApplicationControlField::PropertyValueWrite, objectIndex, quint8(property),
            nbElement, startIndex, data);
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createPropertyDescriptionReadNpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, quint8 propertyIndex,
    quint8 seqNumber)
{
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::PropertyDescriptionRead,
        seqNumber, QKnxUtils::QUint8::bytes(objectIndex)
        + QKnxUtils::QUint8::bytes(quint8(property)) + QKnxUtils::QUint8::bytes(propertyIndex) };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createPropertyDescriptionResponseNpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, quint8 propertyIndex, bool writeable,
    QKnxInterfaceObjectPropertyDataType::Id type, quint16 maxSize, quint8 read, quint8 write,
    quint8 seqNumber)
{
    if (type >= QKnxInterfaceObjectPropertyDataType::Id::Invalid)
        return {};

    if (maxSize > 0x0fff || read > 15 || write > 15)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::PropertyDescriptionResponse,
          seqNumber, QKnxUtils::QUint8::bytes(objectIndex)
        + QKnxUtils::QUint8::bytes(quint8(property))
        + QKnxUtils::QUint8::bytes(propertyIndex)
        + QKnxUtils::QUint8::bytes(quint8(writeable ? 0x80 : 0x00) | quint8(quint8(type) & 0x3f))
        + QKnxUtils::QUint16::bytes(maxSize)
        + QKnxUtils::QUint8::bytes(quint8(read << 4) | quint8(write & 0x0f)) };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createLinkReadNpdu(Mode mode, quint8 groupObjectNumber,
    quint8 startIndex, quint8 seqNumber)
{
    if (startIndex > 0x0f)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::LinkRead, seqNumber,
        QKnxUtils::QUint8::bytes(groupObjectNumber) + QKnxUtils::QUint8::bytes(startIndex) };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createLinkResponseNpdu(Mode mode, quint8 groupObjectNumber,
    quint8 sendingAddress, quint8 startAddress, const QVector<QKnxAddress> &addresses,
    quint8 seqNumber)
{
    if (sendingAddress > 15 || startAddress > 15)
        return {};

    if (addresses.size() > 6
        || !std::all_of(addresses.constBegin(), addresses.constEnd(), [] (const QKnxAddress &a) {
            return a.type() == QKnxAddress::Type::Group;
        })) {
        return {};
    }

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::LinkResponse, seqNumber,
          QKnxUtils::QUint8::bytes(groupObjectNumber)
        + QKnxUtils::QUint8::bytes(quint8(sendingAddress << 4) | quint8(startAddress & 0x0f))
        + [&]() -> QByteArray {
            QByteArray ba;
            for (auto address : qAsConst(addresses))
                ba += address.bytes();
            return ba;
        }() };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createLinkWriteNpdu(Mode mode, quint8 groupObjectNumber,
    QKnxNpdu::LinkWriteFlags flags, const QKnxAddress &groupAddress, quint8 seqNumber)
{
    if (groupAddress.type() != QKnxAddress::Type::Group)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::LinkWrite, seqNumber,
        QKnxUtils::QUint8::bytes(groupObjectNumber) + QKnxUtils::QUint8::bytes(quint8(flags))
        + groupAddress.bytes() };
}

QKnxNpdu QKnxNpduFactory::PointToPoint::createFileStreamInfoReportNpdu(Mode mode, quint8 fileHandle,
    quint8 fileBlockSeqNumber, const QByteArray &data, quint8 seqNumber)
{
    if (fileHandle > 15 || fileBlockSeqNumber > 15)
        return {};

    if (data.size() > 254)
        return {};

    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::FileStreamInfoReport,
        seqNumber, QKnxUtils::QUint8::bytes(quint8(fileHandle << 4) | quint8(fileBlockSeqNumber & 0x0f))
        + data };
}


// -- PointToPointConnectionless

static QKnxNpdu createNetworkParameterNpduP2P(QKnxNpdu::ApplicationControlField apci,
                                              QKnxInterfaceObjectType object,
                                              QKnxInterfaceObjectProperty property,
                                              const QByteArray &data, // aka. testInfo
                                              const QByteArray &testResult = {})
{
    if (!QKnxInterfaceObjectType::isMatch(object, property))
        return {};

    return { QKnxNpdu::TransportControlField::DataIndividual, apci,
        QKnxUtils::QUint16::bytes(quint16(object)).append(quint8(property)) + data + testResult };
}

QKnxNpdu
QKnxNpduFactory::PointToPointConnectionless::createNetworkParameterResponseNpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QByteArray &testInfo, const QByteArray &testResult)
{
    if ((testInfo.size() + testResult.size()) > 11) // L_Data -> max 14 bytes payload
        return {}; // 5 bytes already used for APCI, object, instance

    return createNetworkParameterNpduP2P(QKnxNpdu::ApplicationControlField::NetworkParameterResponse,
        object, property, testInfo, testResult);
}

QKnxNpdu
QKnxNpduFactory::PointToPointConnectionless::createNetworkParameterWriteNpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QByteArray &value)
{
    if (value.size() > 250) // L_Data_Extended -> max 254 bytes payload
        return {}; // 5 bytes already used for APCI, object, instance

    return createNetworkParameterNpduP2P(QKnxNpdu::ApplicationControlField::NetworkParameterWrite,
        object, property, value);
}


// -- PointToPointConnectionOriented

/*!
    Returns a NPDU for ADC Read Application Service with the given sequence
    number \a seqNumber, \a channelNumber and \a readCount set.
*/
QKnxNpdu QKnxNpduFactory::PointToPointConnectionOriented::createAdcReadNpdu(quint8 channel,
    quint8 readCount, quint8 seqNumber)
{
    if (channel > 0x3f)
        return {};

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::AdcRead, seqNumber,
        QKnxUtils::QUint8::bytes(channel) + QKnxUtils::QUint8::bytes(readCount) };
}

QKnxNpdu QKnxNpduFactory::PointToPointConnectionOriented::createAdcResponseNpdu(quint8 channel,
    quint8 readCount, quint16 sumOfAdc, quint8 seqNumber)
{
    if (channel > 0x3f)
        return {};

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::AdcRead, seqNumber,
        QKnxUtils::QUint8::bytes(channel) + QKnxUtils::QUint8::bytes(readCount)
        + QKnxUtils::QUint16::bytes(sumOfAdc) };
}

/*!
    Returns a NPDU for User Memory Read Application Service with \a number,
    \a address, \a data and sequence number \a seqNumber set.
*/
QKnxNpdu
QKnxNpduFactory::PointToPointConnectionOriented::createUserMemoryReadNpdu(quint8 addressExtention,
    quint8 number, quint16 address, quint8 seqNumber)
{
    if (addressExtention > 15 || number > 15)
        return {};

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::UserMemoryRead, seqNumber,
        QKnxUtils::QUint8::bytes(quint8(addressExtention << 4) | quint8(number & 0x0f))
        + QKnxUtils::QUint16::bytes(address) };
}

QKnxNpdu
QKnxNpduFactory::PointToPointConnectionOriented::createUserMemoryResponseNpdu(quint8 addressExtention,
    quint8 number, quint16 address, const QByteArray &data, quint8 seqNumber)
{
    // TODO: Figure out if data is supposed to be of a given size.
    if (addressExtention > 15 || number > 15 || data.size() <= 0 || data.size() > 250)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 5 Bytes already taken

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::UserMemoryResponse, seqNumber,
        QKnxUtils::QUint8::bytes(quint8(addressExtention << 4) | quint8(number & 0x0f))
        + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    Returns a NPDU for User Memory Write Application Service with
    \a addressExtention, \a number, \a address, \a data and \a seqNumber
    set.
*/
QKnxNpdu
QKnxNpduFactory::PointToPointConnectionOriented::createUserMemoryWriteNpdu(quint8 addressExtention,
    quint8 number, quint16 address, const QByteArray &data, quint8 seqNumber)
{
    // TODO: Figure out if data is supposed to be of a given size.
    if (addressExtention > 15 || number > 15 || data.size() <= 0 || data.size() > 250)
        return {}; // L_Data_Extended -> max 254 Bytes payload, 5 Bytes already taken

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::UserMemoryWrite, seqNumber,
        QKnxUtils::QUint8::bytes(quint8(addressExtention << 4) | quint8(number & 0x0f))
        + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    Returns a NPDU for User Manufacturer Info Read Application Service with the
    given sequence number \a seqNumber set.
*/
QKnxNpdu
QKnxNpduFactory::PointToPointConnectionOriented::createUserManufacturerInfoReadNpdu(quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::UserManufacturerInfoRead,
        seqNumber };
}

QKnxNpdu
QKnxNpduFactory::PointToPointConnectionOriented::createUserManufacturerInfoResponseNpdu(quint8 id,
    quint16 manufacturerSpecific, quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::UserManufacturerInfoResponse,
        seqNumber, QKnxUtils::QUint8::bytes(id) + QKnxUtils::QUint16::bytes(manufacturerSpecific) };
}

QKnxNpdu QKnxNpduFactory::PointToPointConnectionOriented::createAuthorizeRequestNpdu(quint32 key,
    quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::AuthorizeRequest, seqNumber,
        QKnxUtils::QUint8::bytes(0u) + QKnxUtils::QUint32::bytes(key) };
}

QKnxNpdu QKnxNpduFactory::PointToPointConnectionOriented::createAuthorizeResponseNpdu(quint8 level,
    quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::AuthorizeResponse, seqNumber,
        QKnxUtils::QUint8::bytes(level) };
}

QKnxNpdu QKnxNpduFactory::PointToPointConnectionOriented::createKeyWriteNpdu(quint8 level,
    quint32 key, quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::KeyWrite, seqNumber,
        QKnxUtils::QUint8::bytes(level) + QKnxUtils::QUint32::bytes(key) };
}

QKnxNpdu QKnxNpduFactory::PointToPointConnectionOriented::createKeyResponseNpdu(quint8 level,
    quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), QKnxNpdu::ApplicationControlField::KeyResponse, seqNumber,
        QKnxUtils::QUint8::bytes(level) };
}

QT_END_NAMESPACE
