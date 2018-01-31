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

#ifndef QKNXTPDU_H
#define QKNXTPDU_H

#include <QtCore/qbytearray.h>
#include <QtCore/qvector.h>
#include <QtCore/qshareddata.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxbytestore.h>

QT_BEGIN_NAMESPACE

class QKnxTpduFactory;
class QKnxTpduPrivate;

class Q_KNX_EXPORT QKnxTpdu final
{
    Q_GADGET

public:
    enum class ErrorCode : quint8
    {
        NoError = 0x00,
        Error = 0x01
    };
    Q_ENUM(ErrorCode)

    enum class ResetType : quint8
    {
        BasicRestart = 0x00,
        MasterRestart = 0x01
    };
    Q_ENUM(ResetType)

    enum class EraseCode : quint8
    {
        Reserved,
        ConfirmedRestart,
        FactoryReset,
        ResetIa,
        ResetAp,
        ResetParam,
        ResetLinks,
        ResetWithoutIa,
        Invalid
    };
    Q_ENUM(EraseCode)

    enum class LinkWriteFlags : quint8
    {
        AddGroupAddress = 0x00,
        AddSendingGroupAddress = 0x01,
        AddNotSendingGroupAddress = 0x00,
        DeleteGroupAddress = 0x02,
    };
    Q_ENUM(LinkWriteFlags)

    enum class TransportControlField : quint8
    {
        DataGroup = 0x00,
        DataBroadcast = 0x00,
        DataSystemBroadcast = 0x00,
        DataTagGroup = 0x04,
        DataIndividual = 0x00,
        DataConnected = 0x40,
        Connect = 0x80,
        Disconnect = 0x81,
        Acknowledge = 0xc2,
        NoAcknowledge = 0xc3,
        Invalid = 0xfc
    };
    Q_ENUM(TransportControlField)

    TransportControlField transportControlField() const;
    void setTransportControlField(TransportControlField tpci);

    enum class ApplicationControlField : quint16
    {
        GroupValueRead = 0x0000,
        GroupValueResponse = 0x0040,
        GroupValueWrite = 0x0080,
        IndividualAddressWrite = 0x00c0,
        IndividualAddressRead = 0x0100,
        IndividualAddressResponse = 0x0140,
        AdcRead = 0x0180,
        AdcResponse = 0x01c0,
        SystemNetworkParameterRead = 0x01c8,
        SystemNetworkParameterResponse = 0x01c9,
        SystemNetworkParameterWrite = 0x01ca,
        MemoryRead = 0x0200,
        MemoryResponse = 0x0240,
        MemoryWrite = 0x0280,
        UserMemoryRead = 0x02c0,
        UserMemoryResponse = 0x02c1,
        UserMemoryWrite = 0x02c2,
        UserManufacturerInfoRead = 0x02c5,
        UserManufacturerInfoResponse = 0x02c6,
        FunctionPropertyCommand = 0x02c7,
        FunctionPropertyStateRead = 0x02c8,
        FunctionPropertyStateResponse = 0x02c9,
        DeviceDescriptorRead = 0x0300,
        DeviceDescriptorResponse = 0x0340,
        Restart = 0x0380,
        AuthorizeRequest = 0x03d1,
        AuthorizeResponse = 0x03d2,
        KeyWrite = 0x03d3,
        KeyResponse = 0x03d4,
        PropertyValueRead = 0x03d5,
        PropertyValueResponse = 0x03d6,
        PropertyValueWrite = 0x03d7,
        PropertyDescriptionRead = 0x03d8,
        PropertyDescriptionResponse = 0x03d9,
        NetworkParameterRead = 0x03da,
        NetworkParameterResponse = 0x03db,
        IndividualAddressSerialNumberRead = 0x03dc,
        IndividualAddressSerialNumberResponse = 0x03dd,
        IndividualAddressSerialNumberWrite = 0x03de,
        DomainAddressWrite = 0x03e0,
        DomainAddressRead = 0x03e1,
        DomainAddressResponse = 0x03e2,
        DomainAddressSelectiveRead = 0x03e3,
        NetworkParameterWrite = 0x03e4,
        NetworkParameterInfoReport = 0x03db,
        LinkRead = 0x03e5,
        LinkResponse = 0x03e6,
        LinkWrite = 0x03e7,
        GroupPropValueRead = 0x03e8,
        GroupPropValueResponse = 0x03e9,
        GroupPropValueWrite = 0x03ea,
        GroupPropValueInfoReport = 0x03eb,
        DomainAddressSerialNumberRead = 0x03ec,
        DomainAddressSerialNumberResponse = 0x03ed,
        DomainAddressSerialNumberWrite = 0x03ee,
        FileStreamInfoReport = 0x03f0,

        Invalid = 0x00ff
    };
    Q_ENUM(ApplicationControlField)

        ApplicationControlField applicationControlField() const;
    void setApplicationControlField(ApplicationControlField apci);

    ~QKnxTpdu();

    explicit QKnxTpdu(TransportControlField tpci);
    QKnxTpdu(TransportControlField tpci, ApplicationControlField apci);
    QKnxTpdu(TransportControlField tpci, ApplicationControlField apci, const QVector<quint8> &data);
    QKnxTpdu(TransportControlField tpci, ApplicationControlField apci, quint8 seqNumber,
        const QVector<quint8> &data = {});

    QKnxTpdu(const QKnxTpdu &o);
    QKnxTpdu &operator=(const QKnxTpdu &o);

    bool isValid() const;
    quint8 dataSize() const;

    quint8 sequenceNumber() const;
    void setSequenceNumber(quint8 seqNumber);

    QVector<quint8> data() const;
    void setData(const QVector<quint8> &data);

    static QKnxTpdu fromBytes(const QVector<quint8> &data, quint16 index, quint8 size)
    {
        auto tpdu = QKnxTpdu { TransportControlField::Invalid, ApplicationControlField::Invalid };

        // data is not big enough according to the given size to be read
        const qint32 availableSize = (data.size() - index) - size;
        if (availableSize < 0)
            return tpdu;

        auto begin = std::next(std::begin(data), index);
        tpdu.setBytes(begin, std::next(begin, size));
        return tpdu;
    }

    quint16 size() const;
    quint8 byte(quint16 index) const;
    QVector<quint8> bytes() const;
    QVector<quint8> bytes(quint16 start, quint16 count) const;
    void setBytes(QVector<quint8>::const_iterator begin, QVector<quint8>::const_iterator end);
    QString toString() const;

private:
    QSharedDataPointer<QKnxTpduPrivate> d_ptr;
};
Q_DECLARE_TYPEINFO(QKnxTpdu::ErrorCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxTpdu::ResetType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxTpdu::EraseCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxTpdu::LinkWriteFlags, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxTpdu::TransportControlField, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxTpdu::ApplicationControlField, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
