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

#include "qknxnetipconnectionheader.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*
    5.3.1 Connection Header:

    The body of every KNXnet/IP frame sent over an established communication
    channel shall start with data fields that shall contain additional general
    information about the data connection. The amount of this data and what
    type of information is included there in particular shall be determined by
    several options during the connection phase of a communication channel.
    The total of these data fields is called connection header and its
    appearance varies greatly depending on the already mentioned connection
    options. Only the order in which the different data fields are stored in
    the connection header is fixed.
*/

QKnxNetIpConnectionHeader::QKnxNetIpConnectionHeader(quint8 channelId, quint8 sequenceCount,
        quint8 serviceTypeSpecificValue)
    : m_isValid(0x07)
{
    setByte(0, 4);
    setByte(1, channelId);
    setByte(2, sequenceCount);
    setByte(3, serviceTypeSpecificValue);
}

bool QKnxNetIpConnectionHeader::isValid() const
{
    return (size() >= 4) && (m_isValid >= 0x07) && (byte(0) == size());
}

quint8 QKnxNetIpConnectionHeader::channelId() const
{
    return byte(1);
}

void QKnxNetIpConnectionHeader::setChannelId(quint8 id)
{
    setByte(1, id);
    m_isValid |= 1 << 0;
    setByte(0, byte(0) == 0 ? 4 : byte(0));
}

quint8 QKnxNetIpConnectionHeader::sequenceCount() const
{
    return byte(2);
}

void QKnxNetIpConnectionHeader::setSequenceCount(quint8 count)
{
    setByte(2, count);
    m_isValid |= 1 << 1;
    setByte(0, byte(0) == 0 ? 4 : byte(0));
}

quint8 QKnxNetIpConnectionHeader::serviceTypeSpecificValue() const
{
    return byte(3);
}

void QKnxNetIpConnectionHeader::setServiceTypeSpecificValue(quint8 value)
{
    setByte(3, value);
    m_isValid |= 1 << 2;
    setByte(0, byte(0) == 0 ? 4 : byte(0));
}

QString QKnxNetIpConnectionHeader::toString() const
{
    const auto items = connectionTypeSpecificHeaderItems();

    QString tmp;
    for (quint8 byte : items)
        tmp += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    tmp.chop(2);

    return QStringLiteral("Size { 0x%1 }, Communication channel ID { 0x%2 }"
        ", Sequence counter { 0x%3 }, Service type specific value { 0x%4 }"
        ", Connection type specific header items { %5 }")
        .arg(size(), 2, 16, QLatin1Char('0'))
        .arg(channelId(), 2, 16, QLatin1Char('0'))
        .arg(sequenceCount(), 2, 16, QLatin1Char('0'))
        .arg(serviceTypeSpecificValue(), 2, 16, QLatin1Char('0')).arg(tmp);
}

QKnxNetIpQKnxNetIpConnectionHeaderRef QKnxNetIpConnectionHeader::ref() const
{
    return QKnxByteStore::ref(0);
}

QKnxNetIpConnectionHeader QKnxNetIpConnectionHeader::fromBytes(const QKnxByteArray &bytes, quint16 index)
{
    const qint32 availableSize = bytes.size() - index;
    if (availableSize < 1)
        return {}; // total size missing

    const quint8 totalSize = QKnxUtils::QUint8::fromBytes(bytes, index);
    if (availableSize < totalSize)
        return {}; // header might be corrupted

    const quint8 channelId = QKnxUtils::QUint8::fromBytes(bytes, index + 1);
    const quint8 sequenceCount = QKnxUtils::QUint8::fromBytes(bytes, index + 2);
    const quint8 serviceTypeSpecificValue = QKnxUtils::QUint8::fromBytes(bytes, index + 3);
    QKnxNetIpConnectionHeader header(channelId, sequenceCount, serviceTypeSpecificValue);
    if (totalSize > 4)
        header.setConnectionTypeSpecificHeaderItems(bytes.mid(index + 4, totalSize - 4));
    return header;
}

QT_END_NAMESPACE
