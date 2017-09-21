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

/*!
    Returns the Transport layer control field of the \c QKnxNpdu.
*/
QKnxNpdu::TransportLayerControlField QKnxNpdu::transportLayerControlField() const
{
    if (size() < 2)
        return TransportLayerControlField::Invalid;
    return TransportLayerControlField(byte(1) & 0xfc); // mask out the first two bits
}

void QKnxNpdu::setTransportLayerControlField(TransportLayerControlField tpci)
{
    if (size() < 2)
        resize(2);
    setByte(1, (byte(1) & 0x03) | quint8(tpci));
    setByte(0, quint8(qMax<quint16>(0u, size() - 2)));
}

/*!
    Returns the Application layer control field of the \c QKnxNpdu.
*/
QKnxNpdu::ApplicationLayerControlField QKnxNpdu::applicationLayerControlField() const
{
    if (size() < 3)
        return ApplicationLayerControlField::Invalid;

    std::bitset<8> apciHigh = byte(1) & 0x03; // mask out all bits except the first two
    std::bitset<8> apciLow = byte(2) & 0xc0;  // mask out all bits except the last two

    const auto fourBitsApci = [&apciHigh, &apciLow]() {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), quint8(apciLow.to_ulong()) } };
        return ApplicationLayerControlField(QKnxUtils::QUint16::fromBytes(apciBytes));
    };
    const auto tenBitsApci = [apciHigh](quint8 octet7) {
        QVector<quint8> apciBytes = { { quint8(apciHigh.to_ulong()), octet7 } };
        return ApplicationLayerControlField(QKnxUtils::QUint16::fromBytes(apciBytes));
    };

    if ((apciHigh[0] == 0 && apciHigh[1] == 0) || (apciHigh[0] == 1 && apciHigh[1] == 1)) {
        std::bitset<8> octet7 = byte(2);
        if (octet7[7] == 1 && octet7[6] == 1)
            return tenBitsApci(byte(2));
        return fourBitsApci();
    }

    if (apciHigh[1] == 0 && apciHigh[0] == 1) {
        // connection oriented, it's one of the A_ADC service
        if (quint8(transportLayerControlField()) > 0)
            return fourBitsApci();
        return tenBitsApci(byte(2));
    }

    // it's one of the A_Memory Service (only the 2 last bits of octet 6 are needed for the apci)
    if (apciLow[7] == 0 || apciLow[6] == 0)
        return fourBitsApci();
    return tenBitsApci(byte(2));
}

void QKnxNpdu::setApplicationLayerControlField(ApplicationLayerControlField apci)
{
    if (size() < 3)
        resize(3);

    auto tmp = QKnxUtils::QUint16::bytes(quint16(apci));
    setByte(0, qMax<quint16>(1u, size() - 2));
    setByte(1, (byte(1) & 0xfc) | tmp[0]);
    setByte(2, (byte(2) & 0x3f) | tmp[1]);
}

QKnxNpdu::QKnxNpdu(TransportLayerControlField tpci)
{
    setTransportLayerControlField(tpci);
}

QKnxNpdu::QKnxNpdu(TransportLayerControlField tpci, ApplicationLayerControlField apci)
{
    setTransportLayerControlField(tpci);
    setApplicationLayerControlField(apci);
}

bool QKnxNpdu::isValid() const
{
    return size() >= 3; // TODO: Extend.
}

QByteArray QKnxNpdu::data() const
{
    if (!isValid())
        return {};

    if (size() > 3) {
        const auto &tmp = ref(3);
        return tmp.bytes<QByteArray>(0, tmp.size());
    }
    return QByteArray(1, byte(2) & 0x3f); // 6 bits from an optimized NPDU
}

bool QKnxNpdu::setData(const QByteArray &data)
{
    if (data.isEmpty())
        return true;
    if (data.size() > 254)
        return false;

    bool optimizedAppend = false;
    auto apci = applicationLayerControlField();
    switch (apci) {
    case ApplicationLayerControlField::GroupValueRead:
    case ApplicationLayerControlField::IndividualAddressRead:
    case ApplicationLayerControlField::IndividualAddressResponse:
    case ApplicationLayerControlField::UserManufacturerInfoRead:
        return false;

    case ApplicationLayerControlField::GroupValueResponse:
    case ApplicationLayerControlField::GroupValueWrite:
        if (data.size() > 1 || data[0] > 0x3f)
            break;
        resize(3);
        setByte(2, (byte(2) & 0xc0) | data[0]);
        setByte(0, quint8(size() - 2));
        return true; // build an optimized NPDU and return early

    case ApplicationLayerControlField::AdcRead:
    case ApplicationLayerControlField::AdcResponse:
    case ApplicationLayerControlField::MemoryRead:
    case ApplicationLayerControlField::MemoryResponse:
    case ApplicationLayerControlField::MemoryWrite:
    case ApplicationLayerControlField::DeviceDescriptorRead:
    case ApplicationLayerControlField::DeviceDescriptorResponse:
    case ApplicationLayerControlField::Restart:
        optimizedAppend = true;
        break;

    case ApplicationLayerControlField::IndividualAddressWrite:
    case ApplicationLayerControlField::SystemNetworkParameterRead:
    case ApplicationLayerControlField::SystemNetworkParameterResponse:
    case ApplicationLayerControlField::SystemNetworkParameterWrite:
    case ApplicationLayerControlField::UserMemoryRead:
    case ApplicationLayerControlField::UserMemoryResponse:
    case ApplicationLayerControlField::UserMemoryWrite:
    case ApplicationLayerControlField::UserManufacturerInfoResponse:
    case ApplicationLayerControlField::FunctionPropertyCommand:
    case ApplicationLayerControlField::FunctionPropertyStateRead:
    case ApplicationLayerControlField::FunctionPropertyStateResponse:

    // data will never fit into the first 4 bits only
    case ApplicationLayerControlField::AuthorizeRequest:
    case ApplicationLayerControlField::AuthorizeResponse:
    case ApplicationLayerControlField::KeyWrite:
    case ApplicationLayerControlField::KeyResponse:
    case ApplicationLayerControlField::PropertyValueRead:
    case ApplicationLayerControlField::PropertyValueResponse:
    case ApplicationLayerControlField::PropertyValueWrite:
    case ApplicationLayerControlField::PropertyDescriptionRead:
    case ApplicationLayerControlField::PropertyDescriptionResponse:
    case ApplicationLayerControlField::NetworkParameterRead:
    case ApplicationLayerControlField::NetworkParameterResponse:
    case ApplicationLayerControlField::IndividualAddressSerialNumberRead:
    case ApplicationLayerControlField::IndividualAddressSerialNumberResponse:
    case ApplicationLayerControlField::IndividualAddressSerialNumberWrite:
    case ApplicationLayerControlField::DomainAddressWrite:
    case ApplicationLayerControlField::DomainAddressRead:
    case ApplicationLayerControlField::DomainAddressResponse:
    case ApplicationLayerControlField::DomainAddressSelectiveRead:
    case ApplicationLayerControlField::NetworkParameterWrite:
    case ApplicationLayerControlField::LinkRead:
    case ApplicationLayerControlField::LinkResponse:
    case ApplicationLayerControlField::LinkWrite:
    case ApplicationLayerControlField::GroupPropValueRead:
    case ApplicationLayerControlField::GroupPropValueResponse:
    case ApplicationLayerControlField::GroupPropValueWrite:
    case ApplicationLayerControlField::GroupPropValueInfoReport:
    case ApplicationLayerControlField::DomainAddressSerialNumberRead:
    case ApplicationLayerControlField::DomainAddressSerialNumberResponse:
    case ApplicationLayerControlField::DomainAddressSerialNumberWrite:
    case ApplicationLayerControlField::FileStreamInfoReport:
        break;
    case ApplicationLayerControlField::Invalid:
        return false;
    }

    resize(3);
    auto tmp = QKnxUtils::QUint16::bytes(quint16(apci));
    setByte(1, (byte(1) & 0xfc) | tmp[0]); // 2 bit APCI part

    if (optimizedAppend) {
        setByte(2, tmp[1] | data[0]);
        appendBytes(data.mid(1));
    } else {
        setByte(2, tmp[1]);
        appendBytes(data);
    }
    setByte(0, quint8(size() - 2));
    return true;
}

QT_END_NAMESPACE
