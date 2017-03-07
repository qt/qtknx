/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipframeheader.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpFrameHeader::QKnxNetIpFrameHeader(quint16 code)
    : QKnxNetIpFrameHeader(code, 0)
{}

QKnxNetIpFrameHeader::QKnxNetIpFrameHeader(quint16 code, quint16 payloadSize)
{
    setCode(code);
    setPayloadSize(payloadSize);
}

bool QKnxNetIpFrameHeader::isValid() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return false;
    return code() != 0;
}

quint16 QKnxNetIpFrameHeader::totalSize() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return 0;
    return QKnxUtils::QUint16::fromBytes(bytes<std::vector<quint8>>(4, 2));
}

quint16 QKnxNetIpFrameHeader::payloadSize() const
{
    return totalSize() - size();
}

void QKnxNetIpFrameHeader::setPayloadSize(quint16 payloadSize)
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10) {
        resize(6, 0);
        setByte(0, QKnxNetIpFrameHeader::HeaderSize10);
        setByte(1, QKnxNetIpFrameHeader::KnxNetIpVersion);
    }
    appendBytes(QKnxUtils::QUint16::bytes<std::vector<quint8>>(payloadSize
        + QKnxNetIpFrameHeader::HeaderSize10));
}

quint16 QKnxNetIpFrameHeader::code() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return 0;
    return QKnxUtils::QUint16::fromBytes(bytes<std::vector<quint8>>(2, 2));
}

void QKnxNetIpFrameHeader::setCode(quint16 code)
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10) {
        resize(6);
        setByte(0, QKnxNetIpFrameHeader::HeaderSize10);
        setByte(1, QKnxNetIpFrameHeader::KnxNetIpVersion);
    }
    appendBytes(QKnxUtils::QUint16::bytes<std::vector<quint8>>(code));
}

QString QKnxNetIpFrameHeader::toString() const
{
    return QStringLiteral("Header size { 0x%1 }, Version { 0x%2 }, Code { 0x%3 }, "
        "Total size { 0x%4 }")
        .arg(QKnxNetIpFrameHeader::HeaderSize10, 2, 16, QLatin1Char('0'))
        .arg(QKnxNetIpFrameHeader::KnxNetIpVersion, 2, 16, QLatin1Char('0'))
        .arg(code(), 2, 16, QLatin1Char('0'))
        .arg(totalSize(), 2, 16, QLatin1Char('0'));
}

QT_END_NAMESPACE
