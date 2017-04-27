/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

QKnxNetIpConnectionHeader::QKnxNetIpConnectionHeader(quint8 communicationChannelId,
        quint8 sequenceCounter, quint8 serviceTypeSpecificValue)
    : m_isValid(0x07)
{
    setByte(0, 4);
    setByte(1, communicationChannelId);
    setByte(2, sequenceCounter);
    setByte(3, serviceTypeSpecificValue);
}

bool QKnxNetIpConnectionHeader::isValid() const
{
    return (size() >= 4) && (m_isValid >= 0x07) && (byte(0) == size());
}

quint8 QKnxNetIpConnectionHeader::communicationChannelId() const
{
    return byte(1);
}

void QKnxNetIpConnectionHeader::setCommunicationChannelId(quint8 id)
{
    setByte(1, id);
    m_isValid |= 1 << 0;
    setByte(0, byte(0) == 0 ? 4 : byte(0));
}

quint8 QKnxNetIpConnectionHeader::sequenceCounter() const
{
    return byte(2);
}

void QKnxNetIpConnectionHeader::setSequenceCounter(quint8 counter)
{
    setByte(2, counter);
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
        .arg(communicationChannelId(), 2, 16, QLatin1Char('0'))
        .arg(sequenceCounter(), 2, 16, QLatin1Char('0'))
        .arg(serviceTypeSpecificValue(), 2, 16, QLatin1Char('0')).arg(tmp);
}

QT_END_NAMESPACE
