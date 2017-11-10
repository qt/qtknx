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

#include "qknxnpdu.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

static bool isBitSet(quint8 byteToTest, quint8 bit)
{
    return (byteToTest & (quint8(1) << bit)) != 0;
};

/*!
    Returns the Transport layer control field of the \c QKnxNpdu.
*/
QKnxNpdu::TransportControlField QKnxNpdu::transportControlField() const
{
    if (size() < 2)
        return TransportControlField::Invalid;

    if (isBitSet(byte(1), 6))                                // T_DATA_CONNECTED, mask out the APCI
        return TransportControlField((byte(1) & 0xfc) & 0xc3); // and the sequence number

    if (isBitSet(byte(1), 7) && (!isBitSet(byte(1), 6))) // T_CONNECT/ T_DISCONNECT
        return TransportControlField(byte(1)); // no APCI, no sequence number

    if (isBitSet(byte(1), 7) && isBitSet(byte(1), 6))      // T_ACK/ T_NACK
        return TransportControlField(byte(1) & 0xc3); // no APCI, mask out sequence number

    return TransportControlField(byte(1) & 0xfc); // mask out the APCI
}

/*!
    Sets the Transport layer control field to \a tpci.
*/
void QKnxNpdu::setTransportControlField(TransportControlField tpci)
{
    if (size() < 2)
        resize(2);
    setByte(1, (byte(1) & 0x03) | quint8(tpci));
    setByte(0, quint8(qMax<quint16>(0u, size() - 2)));
}

/*!
    Returns the Application layer control field of the \c QKnxNpdu.
*/
QKnxNpdu::ApplicationControlField QKnxNpdu::applicationControlField() const
{
    if (size() < 3)
        return ApplicationControlField::Invalid;

    std::bitset<8> apciHigh = byte(1) & 0x03; // mask out all bits except the first two
    std::bitset<8> apciLow = byte(2) & 0xc0;  // mask out all bits except the last two

    const auto fourBitsApci = [&apciHigh, &apciLow]() {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), quint8(apciLow.to_ulong()) } };
        return ApplicationControlField(QKnxUtils::QUint16::fromBytes(apciBytes));
    };
    const auto tenBitsApci = [apciHigh](quint8 octet7) {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), octet7 } };
        return ApplicationControlField(QKnxUtils::QUint16::fromBytes(apciBytes));
    };

    if ((apciHigh[0] == 0 && apciHigh[1] == 0) || (apciHigh[0] == 1 && apciHigh[1] == 1)) {
        std::bitset<8> octet7 = byte(2);
        if (octet7[7] == 1 && octet7[6] == 1)
            return tenBitsApci(byte(2));
        return fourBitsApci();
    }

    if (apciHigh[1] == 0 && apciHigh[0] == 1) {
        // connection oriented, it's one of the A_ADC service
        if (quint8(transportControlField()) > 0)
            return fourBitsApci();
        return tenBitsApci(byte(2));
    }

    // it's one of the A_Memory Service (only the 2 last bits of octet 6 are needed for the apci)
    if (apciLow[7] == 0 || apciLow[6] == 0)
        return fourBitsApci();
    return tenBitsApci(byte(2));
}

/*!
    Sets the Application layer control field to \a apci.
*/
void QKnxNpdu::setApplicationControlField(ApplicationControlField apci)
{
    if (size() < 3)
        resize(3);

    auto tmp = QKnxUtils::QUint16::bytes(quint16(apci));
    setByte(0, qMax<quint16>(1u, size() - 2));
    setByte(1, (byte(1) & 0xfc) | tmp[0]);
    setByte(2, (byte(2) & 0x3f) | tmp[1]);
}

QKnxNpdu::QKnxNpdu(TransportControlField tpci)
{
    setTransportControlField(tpci);
}

QKnxNpdu::QKnxNpdu(TransportControlField tpci, ApplicationControlField apci)
{
    setTransportControlField(tpci);
    setApplicationControlField(apci);
}

QKnxNpdu::QKnxNpdu(TransportControlField tpci, ApplicationControlField apci, const QByteArray &data)
    : QKnxNpdu(tpci, apci, 0, data)
{}

QKnxNpdu::QKnxNpdu(TransportControlField tpci, ApplicationControlField apci, quint8 seqNumber,
        const QByteArray &data)
    : QKnxNpdu(tpci, apci)
{
    setSequenceNumber(seqNumber);
    setData(data);
}

bool QKnxNpdu::isValid() const
{
    if (transportControlField() == TransportControlField::Invalid)
        return false;

#define HEADER_SIZE 3 // [size][TCPI|APCI][APCI] 3 bytes
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
            return size() == (byte(3) == 0x00 ? 8 : 14); // Paragraph: 2.12.1.1/2.12.1.2
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

quint8 QKnxNpdu::dataSize() const
{
    return byte(0);
}

quint8 QKnxNpdu::sequenceNumber() const
{
    if (isBitSet(byte(1), 6))
        return quint8((byte(1) & 0x3c) >> 2);
    return 0;
}

void QKnxNpdu::setSequenceNumber(quint8 seqNumber)
{
    if ((seqNumber > 15) || (!isBitSet(byte(1), 6)))
        return;
    setByte(1, (byte(1) & 0xc3) | quint8(seqNumber << 2));
}

QByteArray QKnxNpdu::data() const
{
    if (size() < 3)
        return {};

    QByteArray bytes;
    switch (applicationControlField()) {
    case ApplicationControlField::GroupValueResponse:
    case ApplicationControlField::GroupValueWrite:
        if (size() > 3)
            break;

    case ApplicationControlField::AdcRead:
    case ApplicationControlField::AdcResponse:
    case ApplicationControlField::MemoryRead:
    case ApplicationControlField::MemoryResponse:
    case ApplicationControlField::MemoryWrite:
    case ApplicationControlField::DeviceDescriptorRead:
    case ApplicationControlField::DeviceDescriptorResponse:
    case ApplicationControlField::Restart:
        bytes = QKnxUtils::QUint8::bytes(quint8(byte(2) & 0x3f)); // 6 bits from an optimized NPDU

    default:
        break;
    }

    const auto &tmp = ref(3);
    return bytes + tmp.bytes<QByteArray>(0, tmp.size());
}

void QKnxNpdu::setData(const QByteArray &data)
{
    auto apci = applicationControlField();
    auto apciBytes = QKnxUtils::QUint16::bytes<QVector<quint8>>(quint16(apci));

    resize(3); // always resize to minimum size
    setByte(2, apciBytes[1]); // and clear the possible 6 bits of the upper APCI byteToTest

    if (data.isEmpty()) {
        setByte(0, quint8(size() - 2));
        return; // no data, bytes got cleared before
    }

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
        setByte(2, apciBytes[1] | quint8(data[0]));
        remainingData = data.mid(1); Q_FALLTHROUGH();

    default:
        break;
    }

    appendBytes(remainingData);
    setByte(0, quint8(size() - 2));
}

QT_END_NAMESPACE
