/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknxnetipframeheader.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

constexpr const quint8 QKnxNetIpFrameHeader::HeaderSize10;
constexpr const quint8 QKnxNetIpFrameHeader::KnxNetIpVersion10;

/*!
    \class QKnxNetIpFrameHeader

    \inmodule QtKnx

    \brief The QKnxNetIpFrameHeader class is a KNXnet/IP frame header.

    The QKnxNetIpFrameHeader class contains the data and meta data related
    to a KNXnet/IP frame. Every KNXnet/IP frame consists of at least the common
    KNXnet/IP header. The common KNXnet/IP header contains information about
    the protocol version, the header and total packet size, and a KNXnet/IP
    service type identifier.
*/

/*!
    \fn QKnxNetIpFrameHeader::QKnxNetIpFrameHeader

    Constructs an empty invalid frame header object.

    \sa isNull(), isValid()
*/

/*!
    \fn QKnxNetIpFrameHeader::~QKnxNetIpFrameHeader()

    Destroys the frame header object and releases all allocated resources.
*/

/*!
    Constructs a valid frame header object and sets the KNXnet/IP service type
    identifier to \a type.

    \note By default a KNXnet/IP Version 1.0 frame header is constructed.

    \sa isValid(), serviceType(), setServiceType()
*/
QKnxNetIpFrameHeader::QKnxNetIpFrameHeader(QKnxNetIp::ServiceType type)
    : QKnxNetIpFrameHeader(type, 0)
{}

/*!
    Constructs a valid frame header object. Sets the KNXnet/IP service type
    identifier to \a type and the data size to \a dataSize and updates the
    total size accordingly.

    \note By default a KNXnet/IP Version 1.0 frame header is constructed.

    \sa isValid(), serviceType(), setServiceType(), dataSize(), setDataSize()
*/
QKnxNetIpFrameHeader::QKnxNetIpFrameHeader(QKnxNetIp::ServiceType type, quint16 dataSize)
{
    m_bytes[0] = HeaderSize10;
    m_bytes[1] = KnxNetIpVersion10;

    setServiceType(type);
    setDataSize(dataSize);
}

/*!
    Returns \c true if this is a default constructed header, otherwise returns
    \c false. A header is considered null if it contains no initialized values.
*/
bool QKnxNetIpFrameHeader::isNull() const
{
    return m_bytes[0] == 0x00 && m_bytes[1] == 0x00;
}

/*!
    Returns \c true if the frame header contains initialized values and is in
    itself valid, otherwise returns \c false. A valid KNXnet/IP frame header
    consist of a header size, protocol version, a known \l serviceType identifier,
    and a minimum total size.

    \sa isNull()
*/
bool QKnxNetIpFrameHeader::isValid() const
{
    if (isNull())
        return false;

    const auto headerSize = m_bytes[0];
    if (m_bytes[1] == KnxNetIpVersion10) {
        if (headerSize != HeaderSize10)
            return {};
    } else { // TODO: Adapt once we support other versions.
        return {};
    }
    const auto totalSize = quint16(quint16(m_bytes[4]) << 8 | m_bytes[5]);
    return QKnxNetIp::isFrameType(serviceType()) && totalSize >= headerSize;
}

/*!
    Returns the size of the KNXnet/IP frame header.

    \sa isNull(), isValid()
*/
quint8 QKnxNetIpFrameHeader::size() const
{
    return m_bytes[0];
}

/*!
    Sets the size of the KNXnet/IP frame header to \size.

    \sa size()
*/
quint8 QKnxNetIpFrameHeader::setSize(quint8 size)
{
    return m_bytes[0] = size;
}

/*!
    Returns the protocol version of the KNXnet/IP frame.

    \sa isNull(), isValid()
*/
quint8 QKnxNetIpFrameHeader::protocolVersion() const
{
    return m_bytes[1];
}

/*!
    Sets the protocol version of the KNXnet/IP frame header to \version.

    \sa protocolVersion()
*/
void QKnxNetIpFrameHeader::setProtocolVersion(quint8 version)
{
    m_bytes[1] = version;
}

/*!
    Returns the total size of the KNXnet/IP frame if the header is valid,
    otherwise returns \c null. The total size includes the size of the header
    and the size of the KNXnet/IP frame data.

    \sa isNull(), isValid(), size(), dataSize()
*/
quint16 QKnxNetIpFrameHeader::totalSize() const
{
    if (!isValid())
        return 0;
    return quint16(quint16(m_bytes[4]) << 8 | m_bytes[5]);
}

/*!
    Returns the size of the KNXnet/IP frame's data if the header is valid,
    otherwise returns \c null.
    The data size excludes the size of the KNXnet/IP frame's header.

    \sa isNull(), isValid(), size(), totalSize()
*/
quint16 QKnxNetIpFrameHeader::dataSize() const
{
    return totalSize() - size();
}

/*!
    Sets the KNXnet/IP frame's data size to \a dataSize and updates the
    total size accordingly.
*/
void QKnxNetIpFrameHeader::setDataSize(quint16 dataSize)
{
    const auto totalSize = dataSize + size();
    m_bytes[4] = quint8((totalSize) >> 8);
    m_bytes[5] = quint8(totalSize);
}

/*!
    Returns the service type identifier of the KNXnet/IP frame.
*/
QKnxNetIp::ServiceType QKnxNetIpFrameHeader::serviceType() const
{
    return QKnxNetIp::ServiceType(quint16(quint16(m_bytes[2]) << 8 | m_bytes[3]));
}

/*!
    Sets the service type identifier of the KNXnet/IP frame to \a type.
*/
void QKnxNetIpFrameHeader::setServiceType(QKnxNetIp::ServiceType type)
{
    m_bytes[2] = quint8(quint16(type) >> 8);
    m_bytes[3] = quint8(quint16(type));
}

/*!
    Returns the KNXnet/IP header size, KNXnet/IP version, generic code and
    the total size as string. Header size, version, code and total size are
    formatted in hexadecimal notation.
*/
QString QKnxNetIpFrameHeader::toString() const
{
    return QStringLiteral("Header size { 0x%1 }, Version { 0x%2 }, Service type { 0x%3 }, "
        "Total size { 0x%4 }")
        .arg(size(), 2, 16, QLatin1Char('0'))
        .arg(protocolVersion(), 2, 16, QLatin1Char('0'))
        .arg(quint16(serviceType()), 2, 16, QLatin1Char('0'))
        .arg(totalSize(), 2, 16, QLatin1Char('0'));
}

/*!
    Returns the byte at position \a index in the header.
*/
quint8 QKnxNetIpFrameHeader::byte(quint8 index) const
{
    Q_ASSERT_X(index < size(), "QKnxNetIpFrameHeader::byte", "index out of range");
    return m_bytes[index];
}

/*!
    Returns an array of bytes that represent the KNXnet/IP frame header.
*/
QKnxByteArray QKnxNetIpFrameHeader::bytes() const
{
    if (!isValid())
        return {};
    return { m_bytes[0], m_bytes[1], m_bytes[2], m_bytes[3], m_bytes[4], m_bytes[5] };
}

/*!
    Constructs the KNXnet/IP frame header from the byte array \a bytes starting
    at position \a index inside the array.

    \sa isNull(), isValid()
*/
QKnxNetIpFrameHeader QKnxNetIpFrameHeader::fromBytes(const QKnxByteArray &bytes, quint16 index)
{
    const qint32 availableSize = bytes.size() - index;
    if (availableSize < 1)
        return {}; // Missing header size.

    const quint8 headerSize = QKnxUtils::QUint8::fromBytes(bytes, index);
    if (availableSize < headerSize || headerSize < 2)
        return {}; // Not enough bytes for the header or to read the version info.

    if (bytes[index + 1] == KnxNetIpVersion10) {
        if (headerSize != HeaderSize10)
            return {};
    } else { // TODO: Adapt once we support other versions.
        return {};
    }

    const quint16 code = QKnxUtils::QUint16::fromBytes(bytes, index + 2);
    if (!QKnxNetIp::isFrameType(QKnxNetIp::ServiceType(code)))
        return {};

    return { QKnxNetIp::ServiceType(code), quint16(QKnxUtils::QUint16::fromBytes(bytes, index + 4)
        - headerSize) };
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxNetIpFrameHeader::operator==(const QKnxNetIpFrameHeader &other) const
{
    return m_bytes == other.m_bytes;
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxNetIpFrameHeader::operator!=(const QKnxNetIpFrameHeader &other) const
{
    return !operator==(other);
}

QT_END_NAMESPACE
