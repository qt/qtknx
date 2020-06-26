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

#include "qknxutils.h"
#include "qknxtpdufactory_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTpduFactory
    \internal

    \inmodule QtKnx
    \brief The QKnxTpduFactory class is used to create a valid Transport protocol
    data unit (\l QKnxTpdu) to be used in an \l QKnxLinkLayerFrame.

    One builds a TPDU to trigger a given application service. The factory is
    organized according to the general category the service we want to trigger
    belongs to. The category refers to the connection type needed to use the
    services. The possible service categories are: \l Multicast, \l Broadcast,
    \l PointToPoint, and \l PointToPointConnectionOriented.

    Within those categories, there is a function to create a TPDU for each
    possible application service belonging to the given category (all the
    services are listed here \l QKnxTpdu::ApplicationControlField).
*/

static QKnxTpdu::TransportControlField tpci(QKnxTpduFactory::PointToPoint::Mode mode, quint8 seq)
{
    if (seq > 15)
        return QKnxTpdu::TransportControlField::Invalid;

    if (mode == QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented)
        return QKnxTpdu::TransportControlField::DataConnected;
    return QKnxTpdu::TransportControlField::DataIndividual;
}


// -- PointToPoint

/*!
    \class QKnxTpduFactory::PointToPoint
    \internal

    \inmodule QtKnx
    \brief The QKnxTpduFactory::PointToPoint class is used to create a valid
    Transport protocol data unit (\l QKnxTpdu) for application services requesting
    point to point connection.

    Those services are accessed using the individual address of the device
    (\l QKnxAddress::Individual) in the source address part of the
    \l QKnxLinkLayerFrame.
    They can be used in \l QKnxTpduFactory::PointToPoint::ConnectionOriented or
    \l QKnxTpduFactory::PointToPoint::Connectionless, that is with or without a
    transport layer connection, respectively.
*/

/*!
    \internal

    \enum QKnxTpduFactory::PointToPoint::Mode

    \value Connectionless,
    \value ConnectionOriented
*/

QKnxTpdu QKnxTpduFactory::PointToPoint::createFunctionPropertyCommandTpdu(Mode mode,
    quint8 objIndex, QKnxInterfaceObjectProperty property, const QKnxByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::FunctionPropertyCommand,
         QKnxUtils::QUint8::bytes(objIndex) + QKnxUtils::QUint8::bytes(property) + data };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createFunctionPropertyStateReadTpdu(Mode mode,
    quint8 objIndex, QKnxInterfaceObjectProperty property, const QKnxByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::FunctionPropertyStateRead,
        QKnxUtils::QUint8::bytes(objIndex) + QKnxUtils::QUint8::bytes(property) + data };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createFunctionPropertyStateResponseTpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, QKnxTpdu::ErrorCode code,
    const QKnxByteArray &data, quint8 seqNumber)
{
    if (data.size() > 250)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 5 Bytes already taken

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::FunctionPropertyStateResponse,
        QKnxUtils::QUint8::bytes(objectIndex) + QKnxUtils::QUint8::bytes(property)
        + QKnxUtils::QUint8::bytes(quint8(code)) + data };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(Mode mode,
    quint8 descriptorType, quint8 seqNumber)
{
    if (descriptorType >= 64) {
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};
    }

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::DeviceDescriptorRead,
        QKnxUtils::QUint8::bytes(descriptorType) };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(Mode mode,
    quint8 descriptorType, const QKnxByteArray &deviceDescriptor, quint8 seqNumber)
{
    if (descriptorType >= 64 || deviceDescriptor.size() > 254)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::DeviceDescriptorResponse,
        QKnxUtils::QUint8::bytes(descriptorType)  + deviceDescriptor };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createRestartTpdu(Mode mode, QKnxTpdu::ResetType type,
    QKnxTpdu::EraseCode eraseCode, quint8 channelNumber, quint8 seqNumber)
{
    if (type == QKnxTpdu::ResetType::BasicRestart)
        return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::Restart };

    if (eraseCode == QKnxTpdu::EraseCode::Reserved || eraseCode >= QKnxTpdu::EraseCode::Invalid)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    if ((eraseCode == QKnxTpdu::EraseCode::ConfirmedRestart
        || eraseCode == QKnxTpdu::EraseCode::ResetIa
        || eraseCode == QKnxTpdu::EraseCode::ResetAp) && channelNumber != 0x00) {
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};
    }

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::Restart,
        QKnxUtils::QUint8::bytes(0x01) + QKnxUtils::QUint8::bytes(quint8(eraseCode))
        + QKnxUtils::QUint8::bytes(channelNumber) };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createRestartResponseTpdu(Mode mode, QKnxTpdu::ResetType type,
    QKnxTpdu::ErrorCode code, quint16 processTime, quint8 seqNumber)
{
    if (type == QKnxTpdu::ResetType::BasicRestart)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::Restart,
        QKnxUtils::QUint8::bytes(0x21) + QKnxUtils::QUint8::bytes(quint8(code))
        + QKnxUtils::QUint16::bytes(processTime) };
}

static QKnxTpdu createPropertyValueTpdu(QKnxTpduFactory::PointToPoint::Mode mode, quint8 seqNumber,
                                        QKnxTpdu::ApplicationControlField apci,
                                        quint8 objectIndex, quint8 property, quint8 nbElement,
                                        quint16 startIndex, const QKnxByteArray &data = {})
{
    if (data.size() > 249) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 6 bytes already used for APCI, object index, PID etc.

    if ((nbElement > 0x0f) || (startIndex > 0x0fff))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    auto tmp = QKnxUtils::QUint16::bytes(startIndex);
    tmp.set(0, quint8(quint8(nbElement << 4) | quint8(tmp.at(0))));
    return { tpci(mode, seqNumber), seqNumber, apci, QKnxUtils::QUint8::bytes(objectIndex)
        + QKnxUtils::QUint8::bytes(property) + tmp + data };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createPropertyValueReadTpdu(Mode mode, quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex, quint8 seqNumber)
{
    return createPropertyValueTpdu(mode, seqNumber,
        QKnxTpdu::ApplicationControlField::PropertyValueRead, objectIndex, quint8(property),
            nbElement, startIndex);
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createPropertyValueResponseTpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    const QKnxByteArray &data, quint8 seqNumber)
{
    return createPropertyValueTpdu(mode, seqNumber,
        QKnxTpdu::ApplicationControlField::PropertyValueResponse, objectIndex, quint8(property),
            nbElement, startIndex, data);
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createPropertyValueWriteTpdu(Mode mode, quint8 objectIndex,
    QKnxInterfaceObjectProperty property, quint8 nbElement, quint16 startIndex,
    const QKnxByteArray &data, quint8 seqNumber)
{
    return createPropertyValueTpdu(mode, seqNumber,
        QKnxTpdu::ApplicationControlField::PropertyValueWrite, objectIndex, quint8(property),
            nbElement, startIndex, data);
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createPropertyDescriptionReadTpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, quint8 propertyIndex,
    quint8 seqNumber)
{
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::PropertyDescriptionRead,
        QKnxUtils::QUint8::bytes(objectIndex)
        + QKnxUtils::QUint8::bytes(quint8(property)) + QKnxUtils::QUint8::bytes(propertyIndex) };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createPropertyDescriptionResponseTpdu(Mode mode,
    quint8 objectIndex, QKnxInterfaceObjectProperty property, quint8 propertyIndex, bool writeable,
    QKnxInterfaceObjectPropertyDataType::Id type, quint16 maxSize, quint8 read, quint8 write,
    quint8 seqNumber)
{
    if (type >= QKnxInterfaceObjectPropertyDataType::Id::Invalid)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    if (maxSize > 0x0fff || read > 15 || write > 15)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::PropertyDescriptionResponse,
          QKnxUtils::QUint8::bytes(objectIndex)
        + QKnxUtils::QUint8::bytes(quint8(property))
        + QKnxUtils::QUint8::bytes(propertyIndex)
        + QKnxUtils::QUint8::bytes(quint8(writeable ? 0x80 : 0x00) | quint8(quint8(type) & 0x3f))
        + QKnxUtils::QUint16::bytes(maxSize)
        + QKnxUtils::QUint8::bytes(quint8(read << 4) | quint8(write & 0x0f)) };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createLinkReadTpdu(Mode mode, quint8 groupObjectNumber,
    quint8 startIndex, quint8 seqNumber)
{
    if (startIndex > 0x0f)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::LinkRead,
        QKnxUtils::QUint8::bytes(groupObjectNumber) + QKnxUtils::QUint8::bytes(startIndex) };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createLinkResponseTpdu(Mode mode, quint8 groupObjectNumber,
    quint8 sendingAddress, quint8 startAddress, const QList<QKnxAddress> &addresses,
    quint8 seqNumber)
{
    if (sendingAddress > 15 || startAddress > 15)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    if (addresses.size() > 6
        || !std::all_of(addresses.constBegin(), addresses.constEnd(), [] (const QKnxAddress &a) {
            return a.type() == QKnxAddress::Type::Group;
        })) {
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};
    }

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::LinkResponse,
          QKnxUtils::QUint8::bytes(groupObjectNumber)
        + QKnxUtils::QUint8::bytes(quint8(sendingAddress << 4) | quint8(startAddress & 0x0f))
        + [&]() -> QKnxByteArray {
            QKnxByteArray ba;
            for (auto address : qAsConst(addresses))
                ba += address.bytes();
            return ba;
        }() };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createLinkWriteTpdu(Mode mode, quint8 groupObjectNumber,
    QKnxTpdu::LinkWriteFlags flags, const QKnxAddress &groupAddress, quint8 seqNumber)
{
    if (groupAddress.type() != QKnxAddress::Type::Group) {
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};
    }
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::LinkWrite,
        QKnxUtils::QUint8::bytes(groupObjectNumber) + QKnxUtils::QUint8::bytes(quint8(flags))
        + groupAddress.bytes() };
}

QKnxTpdu QKnxTpduFactory::PointToPoint::createFileStreamInfoReportTpdu(Mode mode, quint8 fileHandle,
    quint8 fileBlockSeqNumber, const QKnxByteArray &data, quint8 seqNumber)
{
    if (fileHandle > 15 || fileBlockSeqNumber > 15)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    if (data.size() > 254)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::FileStreamInfoReport,
        QKnxUtils::QUint8::bytes(quint8(fileHandle << 4) | quint8(fileBlockSeqNumber & 0x0f))
        + data };
}


// -- PointToPointConnectionless

/*!
    \class QKnxTpduFactory::PointToPointConnectionless
    \internal

    \inmodule QtKnx
    \brief The QKnxTpduFactory::PointToPointConnectionless class is used to
    create a valid Transport protocol data unit (\l QKnxTpdu) for application
    services requesting point to point connection without transport layer
    connection.

    Those services are accessed using the individual address of the device
    (\l QKnxAddress::Individual) in the source address part of the
    \l QKnxLinkLayerFrame.
*/

static QKnxTpdu createNetworkParameterTpduP2P(QKnxTpdu::ApplicationControlField apci,
                                              QKnxInterfaceObjectType object,
                                              QKnxInterfaceObjectProperty property,
                                              const QKnxByteArray &data, // aka. testInfo
                                              const QKnxByteArray &testResult = {})
{
    if (!QKnxInterfaceObjectType::isMatch(object, property))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    QKnxByteArray objectVector = QKnxUtils::QUint16::bytes(quint16(object));
    objectVector.append(quint8(property));
    return { QKnxTpdu::TransportControlField::DataIndividual, apci, objectVector + data + testResult };
}

QKnxTpdu
QKnxTpduFactory::PointToPointConnectionless::createNetworkParameterResponseTpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QKnxByteArray &testInfo, const QKnxByteArray &testResult)
{
    if (testResult.size() > 21) //3.7.7 paragraph 3.2.6 Figure 16
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};
    if ((testInfo.size() + testResult.size()) > 250) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 4 bytes already used for APCI, object, instance

    return createNetworkParameterTpduP2P(QKnxTpdu::ApplicationControlField::NetworkParameterResponse,
        object, property, testInfo, testResult);
}

QKnxTpdu
QKnxTpduFactory::PointToPointConnectionless::createNetworkParameterWriteTpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QKnxByteArray &value)
{
    if (value.size() > 250) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 4 bytes already used for APCI, object, instance

    return createNetworkParameterTpduP2P(QKnxTpdu::ApplicationControlField::NetworkParameterWrite,
        object, property, value);
}


// -- PointToPointConnectionOriented

/*!
    \class QKnxTpduFactory::PointToPointConnectionOriented
    \internal

    \inmodule QtKnx
    \brief The QKnxTpduFactory::PointToPointConnectionOriented class is used to
    create a valid Transport protocol data unit (\l QKnxTpdu) for application
    services requesting point to point connection with a mandatory transport
    layer connection.

    Those services are accessed using the individual address of the device
    (\l QKnxAddress::Individual) in the source address part of the
    \l QKnxLinkLayerFrame. To be successful, the CEMI frame containing those
    TPDU need to be send within the frame work of a transport layer connection.
*/

/*!
    \internal

    Returns a TPDU for Memory Read Application Service with the given \a number,
    \a address and sequence number \a seqNumber set.
*/
QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createMemoryReadTpdu(quint8 number,
    quint16 address, quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::MemoryRead,
        QKnxUtils::QUint8::bytes(number) + QKnxUtils::QUint16::bytes(address) };
}

/*!
    \internal

    Returns a TPDU for Memory Response Application Service with the given
    \a number, \a address, \a data and sequence number \a seqNumber set.
*/
QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createMemoryResponseTpdu(quint8 number,
    quint16 address, const QKnxByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::MemoryResponse,
        QKnxUtils::QUint8::bytes(number) + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    \internal

    Returns a TPDU for Memory Write Application Service with the given
    \a number, \a address, \a data and sequence number \a seqNumber set.
*/
QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createMemoryWriteTpdu(quint8 number,
    quint16 address, const QKnxByteArray &data, quint8 seqNumber)
{
    if (data.size() > 251)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::MemoryWrite,
        QKnxUtils::QUint8::bytes(number) + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    \internal

    Returns a \l QKnxTpdu for ADC Read Application Service with the given
    \a channel, \a readCount and \a seqNumber set.
*/
QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createAdcReadTpdu(quint8 channel,
    quint8 readCount, quint8 seqNumber)
{
    if (channel > 0x3f)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::AdcRead,
        QKnxUtils::QUint8::bytes(channel) + QKnxUtils::QUint8::bytes(readCount) };
}

QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createAdcResponseTpdu(quint8 channel,
    quint8 readCount, quint16 sumOfAdc, quint8 seqNumber)
{
    if (channel > 0x3f)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::AdcRead,
        QKnxUtils::QUint8::bytes(channel) + QKnxUtils::QUint8::bytes(readCount)
        + QKnxUtils::QUint16::bytes(sumOfAdc) };
}

/*!
    \internal

    Returns a \l QKnxTpdu for User Memory Read Application Service with
    \a addressExtention, \a number, \a address and sequence number \a seqNumber
    set.
*/
QKnxTpdu
QKnxTpduFactory::PointToPointConnectionOriented::createUserMemoryReadTpdu(quint8 addressExtention,
    quint8 number, quint16 address, quint8 seqNumber)
{
    if (addressExtention > 15 || number > 15)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::UserMemoryRead,
        QKnxUtils::QUint8::bytes(quint8(addressExtention << 4) | quint8(number & 0x0f))
        + QKnxUtils::QUint16::bytes(address) };
}

QKnxTpdu
QKnxTpduFactory::PointToPointConnectionOriented::createUserMemoryResponseTpdu(quint8 addressExtention,
    quint8 number, quint16 address, const QKnxByteArray &data, quint8 seqNumber)
{
    // TODO: Figure out if data is supposed to be of a given size.
    if (addressExtention > 15 || number > 15 || data.size() <= 0 || data.size() > 250)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 4 Bytes already taken

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::UserMemoryResponse,
        QKnxUtils::QUint8::bytes(quint8(addressExtention << 4) | quint8(number & 0x0f))
        + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    \internal

    Returns a TPDU for User Memory Write Application Service with
    \a addressExtention, \a number, \a address, \a data and \a seqNumber
    set.
*/
QKnxTpdu
QKnxTpduFactory::PointToPointConnectionOriented::createUserMemoryWriteTpdu(quint8 addressExtention,
    quint8 number, quint16 address, const QKnxByteArray &data, quint8 seqNumber)
{
    // TODO: Figure out if data is supposed to be of a given size.
    if (addressExtention > 15 || number > 15 || data.size() <= 0 || data.size() > 250)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // L_Data_Extended -> max 254 Bytes payload, 5 Bytes already taken

    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::UserMemoryWrite,
        QKnxUtils::QUint8::bytes(quint8(addressExtention << 4) | quint8(number & 0x0f))
        + QKnxUtils::QUint16::bytes(address) + data };
}

/*!
    \internal

    Returns a TPDU for User Manufacturer Info Read Application Service with the
    given sequence number \a seqNumber set.
*/
QKnxTpdu
QKnxTpduFactory::PointToPointConnectionOriented::createUserManufacturerInfoReadTpdu(quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber,
        QKnxTpdu::ApplicationControlField::UserManufacturerInfoRead };
}

QKnxTpdu
QKnxTpduFactory::PointToPointConnectionOriented::createUserManufacturerInfoResponseTpdu(quint8 id,
    quint16 manufacturerSpecific, quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::UserManufacturerInfoResponse,
        QKnxUtils::QUint8::bytes(id) + QKnxUtils::QUint16::bytes(manufacturerSpecific) };
}

QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createAuthorizeRequestTpdu(quint32 key,
    quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::AuthorizeRequest,
        QKnxUtils::QUint8::bytes(0u) + QKnxUtils::QUint32::bytes(key) };
}

QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createAuthorizeResponseTpdu(quint8 level,
    quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::AuthorizeResponse,
        QKnxUtils::QUint8::bytes(level) };
}

QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createKeyWriteTpdu(quint8 level,
    quint32 key, quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::KeyWrite,
        QKnxUtils::QUint8::bytes(level) + QKnxUtils::QUint32::bytes(key) };
}

QKnxTpdu QKnxTpduFactory::PointToPointConnectionOriented::createKeyResponseTpdu(quint8 level,
    quint8 seqNumber)
{
    const PointToPoint::Mode mode = PointToPoint::Mode::ConnectionOriented;
    return { tpci(mode, seqNumber), seqNumber, QKnxTpdu::ApplicationControlField::KeyResponse,
        QKnxUtils::QUint8::bytes(level) };
}

QT_END_NAMESPACE
