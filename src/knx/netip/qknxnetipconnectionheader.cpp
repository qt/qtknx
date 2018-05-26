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

#include "qknxnetipconnectionheader.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpConnectionHeader

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpConnectionHeader class is a KNXnet/IP frame connection
    header.

    The body of every KNXnet/IP frame sent over an established communication
    channel starts with a data field that contains additional general
    information about the data connection. The amount of this data and what
    type of information is included there in particular is determined by
    several options during the connection phase of a communication channel.
    The total of these data fields is called connection header and its
    appearance varies greatly depending on the already mentioned connection
    options. Only the order in which the different data fields are stored in
    the connection header is fixed.

    \sa {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpConnectionHeader::QKnxNetIpConnectionHeader()

    Constructs an empty invalid frame header object.

    \sa isNull(), isValid()
*/

/*!
    \fn QKnxNetIpConnectionHeader::~QKnxNetIpConnectionHeader()

    Destroys the frame header object and releases all allocated resources.
*/

/*!
    Constructs a valid frame header object. Sets the object's communication
    channel ID to \a channelId and the sequence number to \a seqNumber. The
    header size is updated accordingly.

    \sa isValid(), channelId(), sequenceNumber()
*/
QKnxNetIpConnectionHeader::QKnxNetIpConnectionHeader(quint8 channelId, quint8 seqNumber)
    : QKnxNetIpConnectionHeader(channelId, seqNumber, 0)
{}

/*!
    Constructs a valid frame header object. Sets the object's communication
    channel ID to \a channelId, the sequence number to \a seqNumber, and the
    service specific value to \a serviceTypeSpecificValue. The header size is
    updated accordingly.

    \sa isValid(), channelId(), sequenceNumber(), serviceTypeSpecificValue()
*/
QKnxNetIpConnectionHeader::QKnxNetIpConnectionHeader(quint8 channelId, quint8 seqNumber,
        quint8 serviceTypeSpecificValue)
{
    m_bytes.set(0, 4);
    m_bytes.set(1, channelId);
    m_bytes.set(2, seqNumber);
    m_bytes.set(3, serviceTypeSpecificValue);
}

/*!
    Returns \c true if this is a default constructed header, otherwise returns
    \c false. A header is considered null if its header size is not initialized.
*/
bool QKnxNetIpConnectionHeader::isNull() const
{
    return m_bytes.at(0) == 0x00;
}

/*!
    Returns \c true if the frame header contains initialized values and is
    in itself valid, otherwise returns \c false. A valid KNXnet/IP frame
    connection header consist of a header size, communication channel ID,
    sequence Number and a service specific type. All values can be \c null
    except the header size.

    \sa isNull()
*/
bool QKnxNetIpConnectionHeader::isValid() const
{
    if (isNull())
        return false;
    return (m_bytes.at(0) >= 4) && (m_bytes.at(0) == m_bytes.size());
}

/*!
    Returns the total size of the header including all items. The minimum size
    for a valid header is 4 bytes.
*/
quint8 QKnxNetIpConnectionHeader::size() const
{
    return m_bytes.at(0);
}

/*!
    Returns the communication channel ID of the KNXnet/IP frame.
*/
quint8 QKnxNetIpConnectionHeader::channelId() const
{
    return m_bytes.at(1);
}

/*!
    Sets the communication channel ID of the KNXnet/IP frame to \a channelId.
*/
void QKnxNetIpConnectionHeader::setChannelId(quint8 channelId)
{
    m_bytes.set(1, channelId);
    m_bytes.set(0, (m_bytes.at(0) == 0x00 ? 4 : m_bytes.at(0)));
}

/*!
    Returns the sequence number of the KNXnet/IP frame.
*/
quint8 QKnxNetIpConnectionHeader::sequenceNumber() const
{
    return m_bytes.at(2);
}

/*!
    Sets the sequence number of the KNXnet/IP frame to \a seqNumber.
*/
void QKnxNetIpConnectionHeader::setSequenceNumber(quint8 seqNumber)
{
    m_bytes.set(2, seqNumber);
    m_bytes.set(0, (m_bytes.at(0) == 0x00 ? 4 : m_bytes.at(0)));
}

/*!
    Returns the service type specific value of the KNXnet/IP frame.
*/
quint8 QKnxNetIpConnectionHeader::serviceTypeSpecificValue() const
{
    return m_bytes.at(3);
}

/*!
    Sets the service type specific value of the KNXnet/IP frame to \a value.
*/
void QKnxNetIpConnectionHeader::setServiceTypeSpecificValue(quint8 value)
{
    m_bytes.set(3, value);
    m_bytes.set(0, (m_bytes.at(0) == 0x00 ? 4 : m_bytes.at(0)));
}

/*!
    Returns a byte array with connection type specific header items of the
    KNXnet/IP frame.
*/
QKnxByteArray QKnxNetIpConnectionHeader::connectionTypeSpecificHeaderItems() const
{
    return m_bytes.mid(4);
}

/*!
    Sets the connection type specific header items of the KNXnet/IP frame to
    \a items.
*/
void QKnxNetIpConnectionHeader::setConnectionTypeSpecificHeaderItems(const QKnxByteArray &items)
{
    m_bytes.set(0, items.size() + 4);
    m_bytes.resize(items.size() + 4);
    m_bytes.replace(4, items.size(), items);
}

/*!
    Returns the byte at position \a index in the header.
*/
quint8 QKnxNetIpConnectionHeader::byte(quint8 index) const
{
    Q_ASSERT_X(index < size(), "QKnxNetIpConnectionHeader::byte", "index out of range");
    return m_bytes.at(index);
}

/*!
    Returns an array of bytes that represent the KNXnet/IP frame connection
    header.
*/
QKnxByteArray QKnxNetIpConnectionHeader::bytes() const
{
    if (!isValid())
        return {};
    return m_bytes;
}

/*!
    Constructs the KNXnet/IP frame connection header from the byte array \a bytes
    starting at the position \a index inside the array.

    \sa isNull(), isValid()
*/
QKnxNetIpConnectionHeader QKnxNetIpConnectionHeader::fromBytes(const QKnxByteArray &bytes, quint16 index)
{
    const qint32 availableSize = bytes.size() - index;
    if (availableSize < 1)
        return {}; // total size missing

    const auto totalSize = bytes.at(index);
    // header needs at least 4 bytes and total size needs to match
    if (availableSize < 4 && availableSize < totalSize)
        return {}; // header might be corrupted

    QKnxNetIpConnectionHeader hdr{ bytes.at(index + 1), bytes.at(index + 2), bytes.at(index + 3) };
    if (totalSize > 4)
        hdr.setConnectionTypeSpecificHeaderItems(bytes.mid(index + 4, totalSize - 4));
    return hdr;
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxNetIpConnectionHeader::operator==(const QKnxNetIpConnectionHeader &other) const
{
    return m_bytes == other.m_bytes;
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxNetIpConnectionHeader::operator!=(const QKnxNetIpConnectionHeader &other) const
{
    return !operator==(other);
}

/*!
    \relates QKnxNetIpConnectionHeader

    Writes the KNXnet/IP connection header \a header to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpConnectionHeader &header)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << header.bytes().toHex();
}

QT_END_NAMESPACE
