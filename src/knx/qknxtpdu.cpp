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

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTpdu

    \inmodule QtKnx
    \brief This class represents the part of the \l QKnxCemiFrame to be read by
    the network, transport and application layers.

    To build a valid TPDU it is recommended to use the \l QKnxTpduFactory.
    Reading the bytes from left to right, a TPDU is composed of the following
    information:

    \list
        \li The transport layer code \l TransportControlField,
        \li The application layer service code \l ApplicationControlField
    \endlist

    If applicable, the T_CONNECT TPDU holds no application layer service for
    example, and the data and other information (if applicable, depending on
    the chosen service).
*/

/*!
    \enum QKnxTpdu::ErrorCode

    This enum describes the possible error codes needing in the building
    of TPDU with service \l QKnxTpdu::FunctionPropertyStateResponse or
    \l QKnxTpdu::Restart

    \value NoError
    \value Error
*/

/*!
    \enum QKnxTpdu::ResetType
    This enum describes the possible reset types needing in the building of
    TPDU with service \l QKnxTpdu::Restart.

    \value BasicRestart
    \value MasterRestart
*/

/*!
    \enum QKnxTpdu::EraseCode
    This enum describes the possible erase codes needing in the building of
    TPDU with service \l QKnxTpdu::Restart.

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
    This enum describes the possible link write flags needing in the building
    of TPDU with service \l QKnxTpdu::LinkWrite.

    \value AddGroupAddress
    \value AddSendingGroupAddress
    \value AddNotSendingGroupAddress
    \value DeleteGroupAddress
*/

/*!
    \enum QKnxTpdu::TransportControlField
    This enum describes the possible message code dedicated to the transport
    layer.

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
    This enum describes the message code dedicated to the application and
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


static bool isBitSet(quint8 byteToTest, quint8 bit)
{
    return (byteToTest & (quint8(1) << bit)) != 0;
};

class QKnxTpduPrivate final : public QSharedData
{
public:
    QKnxTpduPrivate() = default;
    ~QKnxTpduPrivate() = default;

    QVector<quint8> m_tpduBytes;
    qint32 m_apci;
    qint16 m_tpci;
    void setApci();
    void setTpci();
    void setByte(quint16 index, quint8 byte);
    void appendBytes(const QByteArray &bytesToAppend);
};

void QKnxTpduPrivate::setByte(quint16 index, quint8 byte)
{
    if (m_tpduBytes.size() <= index)
        m_tpduBytes.resize(index + 1);
    m_tpduBytes[index] = byte;
}

void QKnxTpduPrivate::setApci()
{
    std::bitset<8> apciHigh = m_tpduBytes[0] & 0x03; // mask out all bits except the first two
    std::bitset<8> apciLow = m_tpduBytes[1] & 0xc0;  // mask out all bits except the last two

    const auto fourBitsApci = [&apciHigh, &apciLow]() {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), quint8(apciLow.to_ulong()) } };
        return (QKnxUtils::QUint16::fromBytes(apciBytes));
    };
    const auto tenBitsApci = [apciHigh](quint8 octet7) {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), octet7 } };
        return (QKnxUtils::QUint16::fromBytes(apciBytes));
    };
    if ((apciHigh[0] == 0 && apciHigh[1] == 0) || (apciHigh[0] == 1 && apciHigh[1] == 1)) {
        std::bitset<8> octet7 = m_tpduBytes[1];
        if (octet7[7] == 1 && octet7[6] == 1) {
            m_apci  = qint32(tenBitsApci(m_tpduBytes[1]));
            return;
        }
        m_apci = qint32(fourBitsApci());
        return;
    }
    if (apciHigh[1] == 0 && apciHigh[0] == 1) {
        // connection oriented, it's one of the A_ADC service
        if (m_tpci > 0) {
            m_apci = qint32(fourBitsApci());
            return;
        }
        m_apci = qint32(tenBitsApci(m_tpduBytes[1]));
        return;
    }
    // it's one of the A_Memory Service (only the 2 last bits of octet 6 are needed for the APCI)
    if (apciLow[7] == 0 || apciLow[6] == 0) {
        m_apci = qint32(fourBitsApci());
        return;
    }
    m_apci = qint32(m_tpduBytes[1]);
    return;
}

void QKnxTpduPrivate::setTpci()
{
    if (m_tpduBytes.size() < 1) {
        m_tpci = -1;
        return;
    }
    if (isBitSet(m_tpduBytes[0], 7) && isBitSet(m_tpduBytes[0], 6) && isBitSet(m_tpduBytes[0], 1)) { // T_ACK/ T_NACK
        m_tpci = qint16(m_tpduBytes[0] & 0xc3); // no APCI, mask out sequence number
        return;
    }
    if (isBitSet(m_tpduBytes[0], 7) && (!isBitSet(m_tpduBytes[0], 6))) {// T_CONNECT/ T_DISCONNECT
        m_tpci = qint16(m_tpduBytes[0]); // no APCI, no sequence number
        return;
    }
    if (isBitSet(m_tpduBytes[0], 6) && (!isBitSet(m_tpduBytes[0], 7))) {// T_DATA_CONNECTED, mask out the APCI
        m_tpci = qint16((m_tpduBytes[0] & 0xfc) & 0xc3); // mask out the APCI and the sequence number
        return;
    }
    m_tpci = qint16(m_tpduBytes[0] & 0xfc); // mask out the APCI
    return;
}

//TODO : replace QByteArray with QVector
void QKnxTpduPrivate::appendBytes(const QByteArray &bytesToAppend)
{
    quint16 pos = quint16(m_tpduBytes.size());
    if (bytesToAppend.size() <= 0)
        return;
    m_tpduBytes.resize(pos + quint16(bytesToAppend.size()));

    std::copy(std::begin(bytesToAppend), std::end(bytesToAppend),
        std::next(std::begin(m_tpduBytes), pos));
}

quint16 QKnxTpdu::size() const
{
    return quint16(d_ptr->m_tpduBytes.size());
}
quint8 QKnxTpdu::byte(quint16 index) const
{
    if (index < size())
        return d_ptr->m_tpduBytes[index];
    return {};
}

QVector<quint8> QKnxTpdu::bytes() const
{
    return d_ptr->m_tpduBytes;
}
QVector<quint8> QKnxTpdu::bytes(quint16 start, quint16 count) const
{
    if (size() < start + count)
        return {};
    return d_ptr->m_tpduBytes.mid(start, count);
}

void QKnxTpdu::setBytes(QVector<quint8>::const_iterator begin, QVector<quint8>::const_iterator end)
{
    d_ptr->m_tpduBytes.resize(std::distance(begin, end));
    std::copy(begin, end, std::begin(d_ptr->m_tpduBytes));
    d_ptr->setTpci();
    d_ptr->setApci();
}

QString QKnxTpdu::toString() const
{
    QString bytesString;
    for (quint8 byte : d_ptr->m_tpduBytes)
        bytesString += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    bytesString.chop(2);
    return QStringLiteral("Bytes { %1 }").arg(bytesString);
}

/*!
    Returns the Transport layer control field of the \c QKnxTpdu.
*/
QKnxTpdu::TransportControlField QKnxTpdu::transportControlField() const
{
    if (d_ptr->m_tpci < 0)
        return TransportControlField::Invalid;
    return TransportControlField(d_ptr->m_tpci);
}

/*!
    Sets the Transport layer control field to \a tpci.
*/
void QKnxTpdu::setTransportControlField(TransportControlField tpci)
{
    d_ptr->m_tpci = qint16(tpci);

    if (size() < 1)
        d_ptr->m_tpduBytes.resize(1);

    switch (tpci ) {
    case TransportControlField::DataBroadcast:
    //case TransportControlField::DataGroup:
    case TransportControlField::DataTagGroup:
    //case TransportControlField::DataIndividual:
        d_ptr->setByte(0, (byte(0) & 0x03) | quint8(tpci)); // keep the APCI
        break;
    case TransportControlField::Acknowledge:
    case TransportControlField::NoAcknowledge:
        d_ptr->setByte(0, (byte(0) & 0x3c) | quint8(tpci)); // keep the sequence number
        break;
    case TransportControlField::DataConnected:
        d_ptr->setByte(0, (byte(0) & 0x3F) | quint8(tpci)); // keep the APCI and the sequence number
        break;
    case TransportControlField::Connect:
    case TransportControlField::Disconnect:
    case TransportControlField::Invalid:
        d_ptr->m_tpci = -1;
    default:
        d_ptr->setByte(0, quint8(tpci)); // replace everything
    }
}

/*!
    Returns the Application layer control field of the \c QKnxTpdu.
*/
QKnxTpdu::ApplicationControlField QKnxTpdu::applicationControlField() const
{
    if (size() < 2 || d_ptr->m_apci < 0)
        return ApplicationControlField::Invalid;
    else
        return ApplicationControlField(d_ptr->m_apci);
}

/*!
    Sets the Application layer control field to \a apci.
*/
void QKnxTpdu::setApplicationControlField(ApplicationControlField apci)
{
    if (apci == ApplicationControlField::Invalid)
        d_ptr->m_apci = -1;
    else
        d_ptr->m_apci = qint32(apci);

    if (size() < 2)
        d_ptr->m_tpduBytes.resize(2);
    auto tmp = QKnxUtils::QUint16::bytes(quint16(apci));
    d_ptr->setByte(0, (byte(0) & 0xfc) | tmp[0]);
    d_ptr->setByte(1, (byte(1) & 0x3f) | tmp[1]);
}

QKnxTpdu::~QKnxTpdu()
{
}

QKnxTpdu::QKnxTpdu(TransportControlField tpci)
    : d_ptr(new QKnxTpduPrivate)
{
    setTransportControlField(tpci);
}

QKnxTpdu::QKnxTpdu(TransportControlField tpci, ApplicationControlField apci)
    : d_ptr(new QKnxTpduPrivate)
{
    setTransportControlField(tpci);
    setApplicationControlField(apci);
}

QKnxTpdu::QKnxTpdu(TransportControlField tpci, ApplicationControlField apci, const QByteArray &data)
    : QKnxTpdu(tpci, apci, 0, data)
{}

QKnxTpdu::QKnxTpdu(TransportControlField tpci, ApplicationControlField apci, quint8 seqNumber,
        const QByteArray &data)
    : QKnxTpdu(tpci, apci)
{
    setSequenceNumber(seqNumber);
    setData(data);
}

QKnxTpdu::QKnxTpdu(const QKnxTpdu &o)
    : d_ptr(o.d_ptr)
{}

QKnxTpdu &QKnxTpdu::operator=(const QKnxTpdu &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Returns true if the current TPDU is valid.

    \note This function is not implemented for every services.
    To make sure your TPDU is correct, use the \l QKnxTpduFactory.
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

#define HEADER_SIZE 2 // [TCPI|APCI][APCI] 2 bytes
#define L_DATA_PAYLOAD 14 // 3_02_02 Communication Medium TP1, Paragraph 2.2.4.1
#define L_DATA_EXTENDED_PAYLOAD 253 // 3_02_02 Communication Medium TP1, Paragraph 2.2.5.1

    switch (applicationControlField()) {
    case ApplicationControlField::GroupValueRead:
    case ApplicationControlField::GroupPropValueRead:
    case ApplicationControlField::IndividualAddressRead:
    case ApplicationControlField::IndividualAddressResponse:
    case ApplicationControlField::DomainAddressRead:
        return size() == HEADER_SIZE;

    case ApplicationControlField::IndividualAddressWrite:
        return size() == HEADER_SIZE + 2; // 2 bytes individual address

    case ApplicationControlField::GroupValueResponse:
    case ApplicationControlField::GroupValueWrite:
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_PAYLOAD);

    case ApplicationControlField::GroupPropValueResponse:
    case ApplicationControlField::GroupPropValueWrite:
    case ApplicationControlField::GroupPropValueInfoReport:
    case ApplicationControlField::NetworkParameterRead:
    case ApplicationControlField::NetworkParameterWrite:
    case ApplicationControlField::SystemNetworkParameterRead:
    case ApplicationControlField::SystemNetworkParameterResponse:
    case ApplicationControlField::SystemNetworkParameterWrite:
        // 3_02_02 Paragraph 2.2.5.1: L_Data_Extended -> max 254 bytes
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_EXTENDED_PAYLOAD);

     // To properly jude that next two one needs to know the type of cEMI frame.
    case ApplicationControlField::NetworkParameterResponse: // L_Data
        //return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_PAYLOAD);
    // case ApplicationLayerControlField::NetworkParameterInfoReport: // L_Data_Extended
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + L_DATA_EXTENDED_PAYLOAD);

    case ApplicationControlField::IndividualAddressSerialNumberRead:
    case ApplicationControlField::DomainAddressSerialNumberRead:
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + 6); // 6 bytes serial number
    case ApplicationControlField::IndividualAddressSerialNumberResponse:
         // 6 bytes serial number, 2 bytes new address, 2 reserved bytes
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + 10);
    case ApplicationControlField::IndividualAddressSerialNumberWrite:
         // 6 bytes serial number, 2 bytes new address, 4 reserved bytes
        return (size() >= HEADER_SIZE) && (size() <= HEADER_SIZE + 12);

    case ApplicationControlField::DomainAddressWrite:
    case ApplicationControlField::DomainAddressResponse:
       return (size() == HEADER_SIZE + 2) || (size() == HEADER_SIZE + 6); // 2 or 6 byteToTest domain address
    case ApplicationControlField::DomainAddressSelectiveRead:
        if (size() >= HEADER_SIZE) // 03_05_02 Management Procedures
            return size() == (byte(2) == 0x00 ? 8 : 14); // Paragraph: 2.12.1.1/2.12.1.2
        return false;

    case ApplicationControlField::DomainAddressSerialNumberResponse:
    case ApplicationControlField::DomainAddressSerialNumberWrite: // 6 byteToTest serial number
        return (size() == HEADER_SIZE + 8) || (size() == HEADER_SIZE + 12); // 2 or 6 byteToTest domain address

    case ApplicationControlField::AdcRead:
    case ApplicationControlField::AdcResponse:
    case ApplicationControlField::MemoryRead:
    case ApplicationControlField::MemoryResponse:
    case ApplicationControlField::MemoryWrite:
    case ApplicationControlField::UserMemoryRead:
    case ApplicationControlField::UserMemoryResponse:
    case ApplicationControlField::UserMemoryWrite:
    case ApplicationControlField::UserManufacturerInfoRead:
    case ApplicationControlField::UserManufacturerInfoResponse:
    case ApplicationControlField::FunctionPropertyCommand:
    case ApplicationControlField::FunctionPropertyStateRead:
    case ApplicationControlField::FunctionPropertyStateResponse:
    case ApplicationControlField::DeviceDescriptorRead:
    case ApplicationControlField::DeviceDescriptorResponse:
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

quint8 QKnxTpdu::dataSize() const
{
    return (size() - 1); // data size start after the TPCI/APCI byte.
}

quint8 QKnxTpdu::sequenceNumber() const
{
    if (isBitSet(byte(0), 6))
        return quint8((byte(0) & 0x3c) >> 2);
    return 0;
}

void QKnxTpdu::setSequenceNumber(quint8 seqNumber)
{
    if ((seqNumber > 15) || (!isBitSet(byte(0), 6)))
        return;
    d_ptr->setByte(0, (byte(0) & 0xc3) | quint8(seqNumber << 2));
}

QVector<quint8> QKnxTpdu::data() const
{
    if (size() < 2)
        return {};

    QVector<quint8> dataApci;
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
        dataApci = QKnxUtils::QUint8::bytes<QVector<quint8>>(quint8(byte(1) & 0x3f));
    default:
        break;
    }

    return dataApci + bytes(2, dataSize() - 1);
}

void QKnxTpdu::setData(const QByteArray &data)
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
    auto apciBytes = QKnxUtils::QUint16::bytes<QVector<quint8>>(quint16(apci));

    d_ptr->m_tpduBytes.resize(2); // always resize to minimum size
    d_ptr->setByte(1, apciBytes[1]); // and clear the possible 6 bits of the upper APCI byteToTest

    if (data.isEmpty())
        return; // no data, bytes got cleared before

    auto remainingData = data;
    switch (apci) {
    case ApplicationControlField::GroupValueResponse:
    case ApplicationControlField::GroupValueWrite:
        if (data.size() > 1 || quint8(data[0]) > 0x3f)
            break;

    case ApplicationControlField::AdcRead:
    case ApplicationControlField::AdcResponse:
    case ApplicationControlField::MemoryRead:
    case ApplicationControlField::MemoryResponse:
    case ApplicationControlField::MemoryWrite:
    case ApplicationControlField::DeviceDescriptorRead:
    case ApplicationControlField::DeviceDescriptorResponse:
    case ApplicationControlField::Restart:
        d_ptr->setByte(1, apciBytes[1] | quint8(data[0]));
        remainingData = data.mid(1); Q_FALLTHROUGH();

    default:
        break;
    }
    d_ptr->appendBytes(remainingData);
}

#include "moc_qknxtpdu.cpp"

QT_END_NAMESPACE
