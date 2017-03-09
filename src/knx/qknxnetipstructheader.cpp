/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipstructheader.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*
    2.1.3 Structures:

    All KNXnet/IP structures follow a common rule: the first octet shall always
    be the length of the complete structure (as some structures may have fields
    of variable length e.g. strings) and the second octet shall always be an
    identifier that shall specify the type of the structure. From the third
    octet on the structure data shall follow.
*/

QKnxNetIpStructHeader::QKnxNetIpStructHeader(quint8 code)
    : QKnxNetIpStructHeader(code, 0)
{}

QKnxNetIpStructHeader::QKnxNetIpStructHeader(quint8 code, quint16 payloadSize)
{
    setCode(code);
    setPayloadSize(payloadSize);
}

bool QKnxNetIpStructHeader::isValid() const
{
    if (size() != 2 && size() != 4)
        return false;
    return code() != 0;
}

quint16 QKnxNetIpStructHeader::totalSize() const
{
    if (size() == 2)
        return byte(0);

    if (size() == 4)
        return QKnxUtils::QUint16::fromBytes(bytes(1, 2));
     return 0;
}

quint16 QKnxNetIpStructHeader::payloadSize() const
{
    return totalSize() - size();
}

void QKnxNetIpStructHeader::setPayloadSize(quint16 payloadSize)
{
    // 2.1.3 Structures:

    // If the amount of data exceeds 252 octets, the length octet shall be FFh
    // and the next two octets shall contain the length as a 16 bit value. Then
    // the structure data shall start at the fifth octet.

    resize(payloadSize > 0xfc ? 4 : 2, code());
    if (payloadSize > 0xfc) {
        setByte(0, 0xff);
        setByte(1, quint8((payloadSize + 4) >> 8));
        setByte(2, quint8(payloadSize + 4));
    } else {
        setByte(0, quint8(payloadSize + 2));
    }
}

quint8 QKnxNetIpStructHeader::code() const
{
    if (size() == 0)
        return 0;
    return byte(size() - 1);
}

void QKnxNetIpStructHeader::setCode(quint8 code)
{
    if (size() == 0)
        resize(2, 2);
    setByte(size() - 1, code);
}

QString QKnxNetIpStructHeader::toString() const
{
    return QStringLiteral("Total size { 0x%1 }, Code { 0x%2 }")
        .arg(totalSize(), 2, 16, QLatin1Char('0')).arg(code(), 2, 16, QLatin1Char('0'));
}

QT_END_NAMESPACE
