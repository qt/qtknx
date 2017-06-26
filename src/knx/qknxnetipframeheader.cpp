/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetipframeheader.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

constexpr const quint8 QKnxNetIpFrameHeader::HeaderSize10;
constexpr const quint8 QKnxNetIpFrameHeader::KnxNetIpVersion10;

QKnxNetIpFrameHeader::QKnxNetIpFrameHeader(QKnxNetIp::ServiceType code)
    : QKnxNetIpFrameHeader(code, 0)
{}

QKnxNetIpFrameHeader::QKnxNetIpFrameHeader(QKnxNetIp::ServiceType code, quint16 payloadSize)
{
    setCode(code);
    setPayloadSize(payloadSize);
}

bool QKnxNetIpFrameHeader::isValid() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return false;
    if (protocolVersion() != QKnxNetIpFrameHeader::KnxNetIpVersion10)
        return false;
    return QKnxNetIp::isFrameType(code());
}

quint16 QKnxNetIpFrameHeader::totalSize() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return 0;
    return QKnxUtils::QUint16::fromBytes(bytes(4, 2));
}

quint8 QKnxNetIpFrameHeader::protocolVersion() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return 0;
    return byte(1);
}

quint16 QKnxNetIpFrameHeader::payloadSize() const
{
    return totalSize() - size();
}

void QKnxNetIpFrameHeader::setPayloadSize(quint16 payloadSize)
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10) {
        resize(QKnxNetIpFrameHeader::HeaderSize10, 0);
        setByte(0, QKnxNetIpFrameHeader::HeaderSize10);
        setByte(1, QKnxNetIpFrameHeader::KnxNetIpVersion10);
    }
    auto bytes = QKnxUtils::QUint16::bytes(payloadSize + QKnxNetIpFrameHeader::HeaderSize10);
    setByte(4, bytes[0]);
    setByte(5, bytes[1]);
}

QKnxNetIp::ServiceType QKnxNetIpFrameHeader::code() const
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10)
        return QKnxNetIp::ServiceType::Unknown;
    return QKnxNetIp::ServiceType(QKnxUtils::QUint16::fromBytes(bytes(2, 2)));
}

void QKnxNetIpFrameHeader::setCode(QKnxNetIp::ServiceType code)
{
    if (size() != QKnxNetIpFrameHeader::HeaderSize10) {
        resize(QKnxNetIpFrameHeader::HeaderSize10, 0);
        setByte(0, QKnxNetIpFrameHeader::HeaderSize10);
        setByte(1, QKnxNetIpFrameHeader::KnxNetIpVersion10);
    }
    auto bytes = QKnxUtils::QUint16::bytes(quint16(code));
    setByte(2, bytes[0]);
    setByte(3, bytes[1]);
}

/*!
    Returns the KNXnet/IP header size, KNXnet/IP version, generic code and
    the total size as string. Header size, version, code and total size are
    formatted in hexadecimal notation.
*/
QString QKnxNetIpFrameHeader::toString() const
{
    return QStringLiteral("Header size { 0x%1 }, Version { 0x%2 }, Code { 0x%3 }, "
        "Total size { 0x%4 }")
        .arg(QKnxNetIpFrameHeader::HeaderSize10, 2, 16, QLatin1Char('0'))
        .arg(QKnxNetIpFrameHeader::KnxNetIpVersion10, 2, 16, QLatin1Char('0'))
        .arg(quint16(code()), 2, 16, QLatin1Char('0'))
        .arg(totalSize(), 2, 16, QLatin1Char('0'));
}

QT_END_NAMESPACE
