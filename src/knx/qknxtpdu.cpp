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

#include "qknxtpdu.h"
#include "qknxutils.h"

#include <bitset>

QT_BEGIN_NAMESPACE

/*!
    \enum QKnxTpdu::ErrorCode

    This enum describes the error codes needed when building a TPDU with the
    \l FunctionPropertyStateResponse or \l Restart service.

    \value NoError
    \value Error
*/

/*!
    \enum QKnxTpdu::ResetType

    This enum describes the reset types needed when building a TPDU with the
    \l Restart service.

    \value BasicRestart
    \value MasterRestart
*/

/*!
    \enum QKnxTpdu::EraseCode

    This enum describes the erase codes needed when building a TPDU with the
    \l Restart service.

    \value Reserved
    \value ConfirmedRestart
    \value FactoryReset
    \value ResetIa
    \value ResetAp
    \value ResetParam
    \value ResetLinks
    \value ResetWithoutIa
    \value Invalid
*/

/*!
    \enum QKnxTpdu::LinkWriteFlags

    This enum describes the link write flags needed when building a TPDU with
    the \l LinkWrite service.

    \value AddGroupAddress
    \value AddSendingGroupAddress
    \value AddNotSendingGroupAddress
    \value DeleteGroupAddress
*/

/*!
    \enum QKnxTpdu::TransportControlField

    This enum describes the message codes dedicated to the transport layer.

    \value DataGroup
    \value DataBroadcast
    \value DataSystemBroadcast
    \value DataTagGroup
    \value DataIndividual
    \value DataConnected
    \value Connect
    \value Disconnect
    \value Acknowledge
    \value NoAcknowledge
    \value Invalid
*/

/*!
    \enum QKnxTpdu::ApplicationControlField

    This enum describes the message codes dedicated to the application and
    representing an application service.

    \value GroupValueRead
    \value GroupValueResponse
    \value GroupValueWrite
    \value IndividualAddressWrite
    \value IndividualAddressRead
    \value IndividualAddressResponse
    \value AdcRead
    \value AdcResponse
    \value SystemNetworkParameterRead
    \value SystemNetworkParameterResponse
    \value SystemNetworkParameterWrite
    \value MemoryRead
    \value MemoryResponse
    \value MemoryWrite
    \value UserMemoryRead
    \value UserMemoryResponse
    \value UserMemoryWrite
    \value UserManufacturerInfoRead
    \value UserManufacturerInfoResponse
    \value FunctionPropertyCommand
    \value FunctionPropertyStateRead
    \value FunctionPropertyStateResponse
    \value DeviceDescriptorRead
    \value DeviceDescriptorResponse
    \value Restart
    \value AuthorizeRequest
    \value AuthorizeResponse
    \value KeyWrite
    \value KeyResponse
    \value PropertyValueRead
    \value PropertyValueResponse
    \value PropertyValueWrite
    \value PropertyDescriptionRead
    \value PropertyDescriptionResponse
    \value NetworkParameterRead
    \value NetworkParameterResponse
    \value IndividualAddressSerialNumberRead
    \value IndividualAddressSerialNumberResponse
    \value IndividualAddressSerialNumberWrite
    \value DomainAddressWrite
    \value DomainAddressRead
    \value DomainAddressResponse
    \value DomainAddressSelectiveRead
    \value NetworkParameterWrite
    \value NetworkParameterInfoReport
    \value LinkRead
    \value LinkResponse
    \value LinkWrite
    \value GroupPropValueRead
    \value GroupPropValueResponse
    \value GroupPropValueWrite
    \value GroupPropValueInfoReport
    \value DomainAddressSerialNumberRead
    \value DomainAddressSerialNumberResponse
    \value DomainAddressSerialNumberWrite
    \value FileStreamInfoReport
    \value Invalid
*/

class QKnxTpduPrivate final : public QSharedData
{
public:
    QKnxTpduPrivate() = default;
    ~QKnxTpduPrivate() = default;

    qint32 m_apci { -1 };
    qint16 m_tpci { -1 };
    QKnxByteArray m_tpduBytes;
    QKnx::MediumType m_mediumType { QKnx::MediumType::NetIP };

    quint8 byte(quint16 index) const
    {
        return m_tpduBytes.value(index, 0x00);
    }
    void setByte(quint16 index, quint8 byte)
    {
        if (m_tpduBytes.size() <= index)
            m_tpduBytes.resize(index + 1);
        m_tpduBytes.setValue(index, byte);
    }
    static bool isBitSet(quint8 byteToTest, quint8 bit)
    {
        return (byteToTest & (quint8(1) << bit)) != 0;
    };
};


/*!
    \class QKnxTpdu

    \inmodule QtKnx
    \brief The QKnxTpdu class represents a TPDU, which is the part of a link
    layer frame to be read by the network, transport, and application layers.

    Reading the bytes from left to right, a transport protocol data unit (TPDU)
    contains the following information:

    \list
        \li The transport layer code indication (TPCI), \l TransportControlField
        \li The application layer code indication (APCI),
            \l ApplicationControlField
    \endlist

    The other fields contained by the TPDU depend on the selected service.
    For example, the \c T_CONNECT TPDU holds no application layer service.

    The sequence number of the TPDU is used when transmitting frames between
    KNXNet/IP clients and servers to make sure the all packages are received
    and that they arrive in the correct order.

    \sa QKnxLinkLayerFrame
*/

/*!
    Creates a TPDU.
*/
QKnxTpdu::QKnxTpdu()
    : d_ptr(new QKnxTpduPrivate)
{}

/*!
    Deletes a TPDU.
*/
QKnxTpdu::~QKnxTpdu()
{}

/*!
    Creates a TPDU with the transport layer code indication set to \a tpci.
*/
QKnxTpdu::QKnxTpdu(TransportControlField tpci)
    : d_ptr(new QKnxTpduPrivate)
{
    setTransportControlField(tpci);
}

/*!
    Creates a TPDU with the transport layer code indication set to \a tpci,
    the application layer code indication set to \a apci, and the data set
    to \a data.
*/
QKnxTpdu::QKnxTpdu(TransportControlField tpci, ApplicationControlField apci,
        const QKnxByteArray &data)
    : QKnxTpdu(tpci)
{
    setApplicationControlField(apci);
    setData(data);
}

/*!
    Creates a TPDU with the transport layer code indication set to \a tpci and
    the sequence number set to \a seqNumber.
*/
QKnxTpdu::QKnxTpdu(TransportControlField tpci, quint8 seqNumber)
    : QKnxTpdu(tpci)
{
    setSequenceNumber(seqNumber);
}

/*!
    Creates a TPDU with the transport layer code indication set to \a tpci,
    sequence number set to \a seqNumber, application layer code indication
    set to \a apci, and the data set to \a data.
*/
QKnxTpdu::QKnxTpdu(TransportControlField tpci, quint8 seqNumber, ApplicationControlField apci,
        const QKnxByteArray &data)
    : QKnxTpdu(tpci)
{
    setApplicationControlField(apci);
    setSequenceNumber(seqNumber);
    setData(data);
}

/*!
    Returns \c true if the TPDU is valid.

    \note This function is not implemented for all services and medium types.
    At the time of this writing, only KNXnet/IP is supported.
*/
bool QKnxTpdu::isValid() const
{
    switch (transportControlField()) {
    case TransportControlField::Invalid:
        return false;
    case TransportControlField::Connect:
    case TransportControlField::Disconnect:
    case TransportControlField::Acknowledge:
    case TransportControlField::NoAcknowledge:
        return size() == 1;
    default:
        break;
    }

// TODO: adjust the constants to depending on the medium type
#define HEADER_SIZE 2 // [TCPI|APCI][APCI] 2 bytes
#define L_DATA_PAYLOAD 14 // 3_02_02 Communication Medium TP1, Paragraph 2.2.4.1
#define L_DATA_MEMORY_PAYLOAD 66 // Max APDU + 2 bites for the tpdu AN177 Table 1
#define L_DATA_EXTENDED_PAYLOAD 253 // 3_02_02 Communication Medium TP1, Paragraph 2.2.5.1

    switch (applicationControlField()) {
    case ApplicationControlField::GroupValueRead:
    case ApplicationControlField::GroupPropValueRead:
    case ApplicationControlField::DeviceDescriptorRead:
    case ApplicationControlField::IndividualAddressRead:
    case ApplicationControlField::IndividualAddressResponse:
    case ApplicationControlField::DomainAddressRead:
        return size() == HEADER_SIZE;

    case ApplicationControlField::MemoryRead:
    case ApplicationControlField::IndividualAddressWrite:
        return size() == HEADER_SIZE + 2; // 2 bytes individual tpdu

    case ApplicationControlField::GroupValueResponse:
    case ApplicationControlField::GroupValueWrite:
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_PAYLOAD);
    case ApplicationControlField::MemoryResponse:
    case ApplicationControlField::MemoryWrite:
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_MEMORY_PAYLOAD);

    case ApplicationControlField::GroupPropValueResponse:
    case ApplicationControlField::GroupPropValueWrite:
    case ApplicationControlField::GroupPropValueInfoReport:
    case ApplicationControlField::NetworkParameterRead:
    case ApplicationControlField::NetworkParameterWrite:
    case ApplicationControlField::SystemNetworkParameterRead:
    case ApplicationControlField::SystemNetworkParameterResponse:
    case ApplicationControlField::SystemNetworkParameterWrite:
    case ApplicationControlField::DeviceDescriptorResponse:
        // 3_02_02 Paragraph 2.2.5.1: L_Data_Extended -> max 254 bytes
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_EXTENDED_PAYLOAD);

     // To properly judge that next two one needs to know the type of cEMI frame.
    case ApplicationControlField::NetworkParameterResponse: // L_Data
        //return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_PAYLOAD);
    // case ApplicationLayerControlField::NetworkParameterInfoReport: // L_Data_Extended
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_EXTENDED_PAYLOAD);

    case ApplicationControlField::IndividualAddressSerialNumberRead:
    case ApplicationControlField::DomainAddressSerialNumberRead:
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + 6); // 6 bytes serial number
    case ApplicationControlField::IndividualAddressSerialNumberResponse:
         // 6 bytes serial number, 2 bytes new tpdu, 2 reserved bytes
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + 10);
    case ApplicationControlField::IndividualAddressSerialNumberWrite:
         // 6 bytes serial number, 2 bytes new tpdu, 4 reserved bytes
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + 12);

    case ApplicationControlField::DomainAddressWrite:
    case ApplicationControlField::DomainAddressResponse:
       return (size() == HEADER_SIZE + 2) || (size() == HEADER_SIZE + 6); // 2 or 6 byteToTest domain tpdu
    case ApplicationControlField::DomainAddressSelectiveRead:
        if (size() >= HEADER_SIZE) // 03_05_02 Management Procedures
            return size() == (d_ptr->byte(2) == 0x00 ? 8 : 14); // Paragraph: 2.12.1.1/2.12.1.2
        return false;

    case ApplicationControlField::DomainAddressSerialNumberResponse:
    case ApplicationControlField::DomainAddressSerialNumberWrite: // 6 byteToTest serial number

        return (size() == HEADER_SIZE + 8) || (size() == HEADER_SIZE + 12); // 2 or 6 byteToTest domain tpdu
    case ApplicationControlField::AdcRead:
    case ApplicationControlField::AdcResponse:
    case ApplicationControlField::UserMemoryRead:
    case ApplicationControlField::UserMemoryResponse:
    case ApplicationControlField::UserMemoryWrite:
    case ApplicationControlField::UserManufacturerInfoRead:
    case ApplicationControlField::UserManufacturerInfoResponse:
    case ApplicationControlField::FunctionPropertyCommand:
    case ApplicationControlField::FunctionPropertyStateRead:
    case ApplicationControlField::FunctionPropertyStateResponse:
    case ApplicationControlField::Restart:
    case ApplicationControlField::AuthorizeRequest:
    case ApplicationControlField::AuthorizeResponse:
    case ApplicationControlField::KeyWrite:
    case ApplicationControlField::KeyResponse:
    case ApplicationControlField::PropertyValueRead:
    case ApplicationControlField::PropertyValueResponse:
    case ApplicationControlField::PropertyValueWrite:
    case ApplicationControlField::PropertyDescriptionRead:
    case ApplicationControlField::PropertyDescriptionResponse:
    case ApplicationControlField::LinkRead:
    case ApplicationControlField::LinkResponse:
    case ApplicationControlField::LinkWrite:
    case ApplicationControlField::FileStreamInfoReport:
    case ApplicationControlField::Invalid:
        break;  // TODO: implement
    }
    return false;

#undef HEADER_SIZE
#undef L_DATA_PAYLOAD
#undef L_DATA_EXTENDED_PAYLOAD
}

/*!
    Returns the medium type of the TPDU.
*/
QKnx::MediumType QKnxTpdu::mediumType() const
{
    return d_ptr->m_mediumType;
}

/*!
    Sets the medium type of the TPDU to \a mediumType.
*/
void QKnxTpdu::setMediumType(QKnx::MediumType mediumType)
{
     d_ptr->m_mediumType = mediumType;
}

/*!
    Returns the transport layer code indication field of the TPDU.
*/
QKnxTpdu::TransportControlField QKnxTpdu::transportControlField() const
{
    if (size() < 1 || d_ptr->m_tpci < 0)
        return TransportControlField::Invalid;
    return TransportControlField(d_ptr->m_tpci);
}

/*!
    Sets the transport layer control indication field to \a tpci.
*/
void QKnxTpdu::setTransportControlField(TransportControlField tpci)
{
    d_ptr->m_tpci = qint16(tpci);

    if (size() < 1)
        d_ptr->m_tpduBytes.resize(1);

    switch (tpci ) {
    case TransportControlField::DataBroadcast:
    case TransportControlField::DataTagGroup:
         // keep the APCI
        d_ptr->setByte(0, (d_ptr->byte(0) & 0x03) | quint8(tpci));
        break;
    case TransportControlField::Acknowledge:
    case TransportControlField::NoAcknowledge:
         // keep the sequence number
        d_ptr->setByte(0, (d_ptr->byte(0) & 0x3c) | quint8(tpci));
        break;
    case TransportControlField::DataConnected:
         // keep the APCI and the sequence number
        d_ptr->setByte(0, (d_ptr->byte(0) & 0x3F) | quint8(tpci));
        break;
    case TransportControlField::Connect:
    case TransportControlField::Disconnect:
        // replace everything
        d_ptr->setByte(0, quint8(tpci));
        break;
    default:
        d_ptr->m_tpci = -1;
    }
}

/*!
    Returns the application layer control indication field of the TPDU.
*/
QKnxTpdu::ApplicationControlField QKnxTpdu::applicationControlField() const
{
    if (size() < 2 || d_ptr->m_apci < 0)
        return ApplicationControlField::Invalid;
    return ApplicationControlField(d_ptr->m_apci);
}

/*!
    Sets the application layer control indication field to \a apci.
*/
void QKnxTpdu::setApplicationControlField(ApplicationControlField apci)
{
    d_ptr->m_apci = qint32(apci);
    if (size() < 2)
        d_ptr->m_tpduBytes.resize(2);

    if (apci != ApplicationControlField::Invalid) {
        auto tmp = QKnxUtils::QUint16::bytes(quint16(apci));
        d_ptr->setByte(0, (d_ptr->byte(0) & 0xfc) | tmp.at(0));
        d_ptr->setByte(1, (d_ptr->byte(1) & 0x3f) | tmp.at(1));
    } else {
        d_ptr->m_apci = -1;
    }
}

/*!
    Returns the number of bytes of the TPDU.
*/
quint16 QKnxTpdu::size() const
{
    return quint16(d_ptr->m_tpduBytes.size());
}

/*!
    Returns the number of bytes of the TPDU data.

    \note The data part of a TPDU may contain the low byte of the application
    layer control indication (APCI), but excludes the byte for the transport
    layer control indication (TPCI) field.
*/
quint16 QKnxTpdu::dataSize() const
{
    return (size() - 1); // data size start after the TPCI/APCI byte.
}

/*!
    Returns the sequence number if the frame is connection oriented; otherwise
    returns \c 0.
*/
quint8 QKnxTpdu::sequenceNumber() const
{
    if (QKnxTpduPrivate::isBitSet(d_ptr->byte(0), 6))
        return quint8((d_ptr->byte(0) & 0x3c) >> 2);
    return 0;
}

/*!
    Sets the sequence number to \a seqNumber if the frame is connection oriented;
    otherwise does nothing.
*/
void QKnxTpdu::setSequenceNumber(quint8 seqNumber)
{
    if ((seqNumber > 15) || (!QKnxTpduPrivate::isBitSet(d_ptr->byte(0), 6)))
        return;
    d_ptr->setByte(0, (d_ptr->byte(0) & 0xc3) | quint8(seqNumber << 2));
}

/*!
    Returns the data part of the TPDU as an array of bytes.

    \note TPCI and APCI are not part of the returned byte array.
*/
QKnxByteArray QKnxTpdu::data() const
{
    if (size() < 2)
        return {};

    QKnxByteArray dataApci;
    switch (applicationControlField()) {
    case ApplicationControlField::GroupValueResponse:
    case ApplicationControlField::GroupValueWrite:
        if (size() > 2)
            break;

    case ApplicationControlField::AdcRead:
    case ApplicationControlField::AdcResponse:
    case ApplicationControlField::MemoryRead:
    case ApplicationControlField::MemoryResponse:
    case ApplicationControlField::MemoryWrite:
    case ApplicationControlField::DeviceDescriptorRead:
    case ApplicationControlField::DeviceDescriptorResponse:
    case ApplicationControlField::Restart: // 6 bits from an optimized TPDU
        dataApci = QKnxUtils::QUint8::bytes(quint8(d_ptr->byte(1) & 0x3f));
    default:
        break;
    }

    return dataApci + d_ptr->m_tpduBytes.mid(2, dataSize() - 1);
}

/*!
    Sets the data part of the TPDU to \a data.

    \note The TPCI and APCI may not be a part of the passed argument.
*/
void QKnxTpdu::setData(const QKnxByteArray &data)
{
    // In those cases there should be no data.
    switch (transportControlField()) {
    case TransportControlField::Invalid:
        return;
    case TransportControlField::Connect:
    case TransportControlField::Disconnect:
    case TransportControlField::Acknowledge:
    case TransportControlField::NoAcknowledge:
        return;
    default:
        break;
    }

    auto apci = applicationControlField();
    auto apciBytes = QKnxUtils::QUint16::bytes(quint16(apci));

    d_ptr->m_tpduBytes.resize(2); // always resize to minimum size
    if (apci != ApplicationControlField::Invalid)
        d_ptr->setByte(1, apciBytes.at(1)); // and clear the possible 6 bits of the upper APCI byteToTest

    if (data.isEmpty())
        return; // no data, bytes got cleared before

    auto remainingData = data;
    switch (apci) {
    case ApplicationControlField::GroupValueResponse:
    case ApplicationControlField::GroupValueWrite:
        if (data.size() > 1 || quint8(data.at(0)) > 0x3f)
            break;

    case ApplicationControlField::AdcRead:
    case ApplicationControlField::AdcResponse:
    case ApplicationControlField::MemoryRead:
    case ApplicationControlField::MemoryResponse:
    case ApplicationControlField::MemoryWrite:
    case ApplicationControlField::DeviceDescriptorRead:
    case ApplicationControlField::DeviceDescriptorResponse:
    case ApplicationControlField::Restart:
        d_ptr->setByte(1, apciBytes.at(1) | quint8(data.at(0)));
        remainingData = data.mid(1); Q_FALLTHROUGH();

    default:
        break;
    }
    d_ptr->m_tpduBytes += remainingData;
}

/*!
    Returns the TPDU as an array of bytes.
*/
QKnxByteArray QKnxTpdu::bytes() const
{
    return d_ptr->m_tpduBytes;
}

/*!
    Creates a TPDU with the medium type \a mediumType from the byte array
    \a data starting at the position \a index inside the array with the size
    \a size.
*/
QKnxTpdu QKnxTpdu::fromBytes(const QKnxByteArray &data, quint16 index, quint16 size,
    QKnx::MediumType mediumType)
{
    // data is not big enough according to the given size to be read
    const qint32 availableSize = (data.size() - index) - size;
    if (availableSize < 0) // the TPDU consists at least out of a single byte (TPCI)
        return { TransportControlField::Invalid, ApplicationControlField::Invalid };

    QKnxTpdu tpdu(data.mid(index, size));
    tpdu.setMediumType(mediumType);
    tpdu.setTransportControlField(QKnxTpdu::tpci(data, index));
    tpdu.setApplicationControlField(QKnxTpdu::apci(data, index));
    return tpdu;
}

/*!
    Returns the sequence number extracted from the \a data byte array if the
    byte at position \a index can be verified as a valid TPCI field.

    If an error occurs, \a ok is set to \c false and this function returns a
    negative value.

    \note The given byte array is not further validated, so you need to be sure
    to pass data that is a TPDU.
*/
quint8 QKnxTpdu::sequenceNumber(const QKnxByteArray &data, quint8 index, bool *ok)
{
    if (ok)
        *ok = false;

    if (data.size() - index < 1)
        return 0;

    auto tpci = QKnxTpdu::tpci(data, index);
    if (!QKnxTpduPrivate::isBitSet(quint8(tpci), 6))
        return 0;

    if (tpci == QKnxTpdu::TransportControlField::Invalid)
        return 0;

    auto byte = data.at(index);
    if (QKnxTpduPrivate::isBitSet(byte, 6)) {
        if (ok) *ok = true;
        return quint8((byte & 0x3c) >> 2);
    }
    return -1;
}

/*!
    Returns the TPCI field extracted from the \a data byte array at the position
    \a index; otherwise returns \l Invalid.

    \note If the TPCI field carries a sequence number, the value
    is removed from the return value.

    \note The given byte array is not further validated, so you need to be sure
    to pass data that is a TPDU.
*/
QKnxTpdu::TransportControlField QKnxTpdu::tpci(const QKnxByteArray &data, quint8 index)
{
    if (data.size() - index < 1)
        return QKnxTpdu::TransportControlField::Invalid;

    auto byte = data.at(index);
    if (QKnxTpduPrivate::isBitSet(byte, 7) && QKnxTpduPrivate::isBitSet(byte, 6)
        && QKnxTpduPrivate::isBitSet(byte, 1)) {
            // T_ACK/ T_NACK, mask out sequence number (no APCI)
            return QKnxTpdu::TransportControlField(byte & 0xc3);
    }

    if (QKnxTpduPrivate::isBitSet(byte, 7) && (!QKnxTpduPrivate::isBitSet(byte, 6))) {
        // T_CONNECT/ T_DISCONNECT, no sequence number (no APCI)
        return QKnxTpdu::TransportControlField(byte);
    }

    if (QKnxTpduPrivate::isBitSet(byte, 6) && (!QKnxTpduPrivate::isBitSet(byte, 7))) {
        // T_DATA_CONNECTED, mask out the APCI and sequence number
        return QKnxTpdu::TransportControlField((byte & 0xfc) & 0xc3);
    }

    return QKnxTpdu::TransportControlField(byte & 0xfc); // mask out the APCI
}

/*!
    Returns the APCI field extracted out of the \a data byte array at the
    position \a index; otherwise returns \l Invalid.

    \note The given byte array is not further validated, so you need to be sure
    to pass data that is a TPDU.
*/
QKnxTpdu::ApplicationControlField QKnxTpdu::apci(const QKnxByteArray &data, quint8 index)
{
    if (data.size() - index < 2)
        return QKnxTpdu::ApplicationControlField::Invalid;

    std::bitset<8> apciHigh = data.at(index) & 0x03; // mask out all bits except the first two
    std::bitset<8> apciLow = data.at(index + 1) & 0xc0;  // mask out all bits except the last two

    const auto fourBitsApci = [&apciHigh, &apciLow]() {
        return (QKnxUtils::QUint16::fromBytes({ quint8(apciHigh.to_ulong()),
            quint8(apciLow.to_ulong()) }));
    };
    const auto tenBitsApci = [apciHigh](quint8 octet7) {
        return (QKnxUtils::QUint16::fromBytes({ quint8(apciHigh.to_ulong()), octet7 }));
    };

    if ((apciHigh[0] == 0 && apciHigh[1] == 0) || (apciHigh[0] == 1 && apciHigh[1] == 1)) {
        std::bitset<8> octet7 = data.at(index + 1);
        if (octet7[7] == 1 && octet7[6] == 1)
            return QKnxTpdu::ApplicationControlField(tenBitsApci(data.at(index + 1)));
        return QKnxTpdu::ApplicationControlField(fourBitsApci());
    }
    if (apciHigh[1] == 0 && apciHigh[0] == 1) {
        // connection oriented, it's one of the A_ADC service
        if (QKnxTpdu::tpci(data, index) >= QKnxTpdu::TransportControlField::DataTagGroup)
            return QKnxTpdu::ApplicationControlField(fourBitsApci());
        return QKnxTpdu::ApplicationControlField(tenBitsApci(data.at(index + 1)));
    }
    // it's one of the A_Memory Service (only the 2 last bits of octet 6 are needed for the APCI)
    if (apciLow[7] == 0 || apciLow[6] == 0)
        return QKnxTpdu::ApplicationControlField(fourBitsApci());
    return QKnxTpdu::ApplicationControlField(data.at(index + 1));
}

/*!
    Constructs a copy of \a other.
*/
QKnxTpdu::QKnxTpdu(const QKnxTpdu &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxTpdu &QKnxTpdu::operator=(const QKnxTpdu &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxTpdu::swap(QKnxTpdu &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxTpdu::QKnxTpdu(QKnxTpdu &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxTpdu &QKnxTpdu::operator=(QKnxTpdu &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxTpdu::operator==(const QKnxTpdu &other) const
{
    return d_ptr == other.d_ptr
        || (d_ptr->m_tpci == other.d_ptr->m_tpci
            && d_ptr->m_apci == other.d_ptr->m_apci
            && d_ptr->m_tpduBytes == other.d_ptr->m_tpduBytes
            && d_ptr->m_mediumType == other.d_ptr->m_mediumType);
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxTpdu::operator!=(const QKnxTpdu &other) const
{
    return !operator==(other);
}

/*!
    \internal
*/
QKnxTpdu::QKnxTpdu(const QKnxByteArray &data)
    : QKnxTpdu()
{
    d_ptr->m_tpduBytes = data;
}

/*!
    \relates QKnxTpdu

    Writes the KNX TPDU \a tpdu to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxTpdu &tpdu)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << tpdu.bytes().toHex();
}

#include "moc_qknxtpdu.cpp"

QT_END_NAMESPACE
