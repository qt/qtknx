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

#include "qknxtunnelframe.h"

QT_BEGIN_NAMESPACE

QKnxTunnelFrame::QKnxTunnelFrame(QKnxTunnelFrame::MessageCode messageCode)
    : QKnxCemiFrame(messageCode)
{}

bool QKnxTunnelFrame::isValid() const
{
    return QKnxCemiFrame::isValid();
    // TODO: check NPDU size, several cases need to be taken into account:
    // 1; Information-Length (max. value is 255); number of NPDU octets, TPCI octet not included!
    // 2; Check precense of Pl/RF medium information in the additional info -> size always needs
    //    to be greather then 15 bytes because both need additional information.
    //    03_06_03 EMI_IMI v01.03.03 AS.pdf page 76 Table(Use of flags in control field)
    // 3; RF frames do not inlcude a length field at all, it is supposed to be set to 0x00.
    //    03_06_03 EMI_IMI v01.03.03 AS.pdf page 75 NOTE 1
    // 4; control field frame type standard -> max. length value is 15
    //    control field frame type extended -> max. length value is 255
    //    03_03_02 Data Link Layer General v01.02.02 AS.pdf page 12 paragraph 2.2.5
}

QKnxControlField QKnxTunnelFrame::controlField() const
{
    return QKnxControlField { serviceInformationRef(additionalInfosSize() + 1).byte(0) };
}

void QKnxTunnelFrame::setControlField(const QKnxControlField &controlField)
{
    auto payload = serviceInformation();
    payload.setByte(additionalInfosSize() + 1, controlField.bytes());
    setServiceInformation(payload);
}

QKnxExtendedControlField QKnxTunnelFrame::extendedControlField() const
{
    return QKnxExtendedControlField { serviceInformationRef(additionalInfosSize() + 1).byte(1) };
}

void QKnxTunnelFrame::setExtendedControlField(const QKnxExtendedControlField &controlFieldEx)
{
    auto payload = serviceInformation();
    payload.setByte(additionalInfosSize() + 2, controlFieldEx.bytes());
    setServiceInformation(payload);
}

quint8 QKnxTunnelFrame::additionalInfosSize() const
{
    auto size = serviceInformationRef().byte(0);
    return (size < 0xff ? size : 0u); // 0xff is reserved for future use
}

void QKnxTunnelFrame::addAdditionalInfo(const QKnxAdditionalInfo &info)
{
    auto payload = serviceInformation();

    quint8 size = payload.byte(0);
    if (size + info.size() > 0xfe)
        return; // maximum size would be exceeded, 0xff is reserved for future use

    quint8 index = 1;
    if (size > 0) {
        while (index < size) {
            if (QKnxAdditionalInfo::Type(payload.byte(index)) >= info.type())
                break;
            index += payload.byte(index + 1) + 2; // type + size => 2
        }
    }

    payload.insertBytes((index > size ? size + 1 : index), info.bytes());
    payload.setByte(0, size + info.size());
    setServiceInformation(payload);
}

void QKnxTunnelFrame::removeAdditionalInfo(QKnxAdditionalInfo::Type type)
{
    quint8 oldSize = additionalInfosSize();
    if (oldSize == 0)
        return;

    QKnxCemiPayload payload(oldSize);

    auto infos = additionalInfos();
    for (auto &info : qAsConst(infos)) {
        if (info.type() == type)
            continue;
        payload.appendBytes(info.bytes());
    }

    payload.setByte(0, payload.size() - 1);
    payload.appendBytes(serviceInformationRef(oldSize + 1).bytes<QByteArray>());

    setServiceInformation(payload);
}

void QKnxTunnelFrame::removeAdditionalInfo(const QKnxAdditionalInfo &info)
{
    quint8 oldSize = additionalInfosSize();
    if (oldSize == 0)
        return;

    QKnxCemiPayload payload(oldSize);

    auto infos = additionalInfos();
    for (auto &tmp : qAsConst(infos)) {
        if (tmp.type() == info.type()) {
            if (tmp.bytes() == info.bytes())
                continue;
        }
        payload.appendBytes(tmp.bytes());
    }

    payload.setByte(0, payload.size() - 1);
    payload.appendBytes(serviceInformationRef(oldSize + 1).bytes<QByteArray>());

    setServiceInformation(payload);
}

void QKnxTunnelFrame::clearAdditionalInfos()
{
    quint8 oldSize = additionalInfosSize();
    if (oldSize == 0)
        return;

    QKnxCemiPayload payload(0x00);
    payload.appendBytes(serviceInformationRef(oldSize + 1).bytes<QByteArray>());
    setServiceInformation(payload);
}

const QKnxAddress QKnxTunnelFrame::sourceAddress() const
{
    return { QKnxAddress::Type::Individual, serviceInformationRef(additionalInfosSize() + 1)
        .bytes(2, 2) };
}

void QKnxTunnelFrame::setSourceAddress(const QKnxAddress &source)
{
    auto payload = serviceInformation();
    payload.replaceBytes(additionalInfosSize() + 3, source.bytes());
    setServiceInformation(payload);
}

const QKnxAddress QKnxTunnelFrame::destinationAddress() const
{
    return { extendedControlField().destinationAddressType(),
        serviceInformationRef(additionalInfosSize() + 1).bytes(4, 2) };
}

void QKnxTunnelFrame::setDestinationAddress(const QKnxAddress &destination)
{
    auto payload = serviceInformation();
    payload.replaceBytes(additionalInfosSize() + 5, destination.bytes());
    setServiceInformation(payload);
}

QKnxNpdu QKnxTunnelFrame::npdu() const
{
     // length field + ctrl + extCtrl + 2 * KNX address -> 7
    return QKnxNpdu::fromBytes(serviceInformationRef(), additionalInfosSize() + 7,
        size() - (additionalInfosSize() + 7));
}

void QKnxTunnelFrame::setNpdu(const QKnxNpdu &npdu)
{
    auto info = serviceInformation();
    info.resize(additionalInfosSize() + 7); // length field + ctrl + extCtrl + 2 * KNX address
    info.appendBytes(npdu.bytes());
    setServiceInformation(info);
}

QKnxTunnelFrame::QKnxTunnelFrame(const QKnxCemiFrame &other)
    : QKnxCemiFrame(other)
{}

QT_END_NAMESPACE
