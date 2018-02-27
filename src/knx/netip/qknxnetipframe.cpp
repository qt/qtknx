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

#include "qknxnetipframe.h"

QT_BEGIN_NAMESPACE

class QKnxNetIpFrameExPrivate : public QSharedData
{
public:
    QKnxNetIpFrameHeader m_header;
    QKnxNetIpConnectionHeader m_connectionHeader;
    QKnxByteArray m_data;
};


/*!
    \class QKnxNetIpFrame

    \inmodule QtKnx
    \brief The QKnxNetIpFrame class represents the base for all KNXnet/IP
    frames.
*/

/*!
    Constructs an empty invalid frame object.

    \sa isNull, isValid
*/
QKnxNetIpFrameEx::QKnxNetIpFrameEx()
    : d_ptr(new QKnxNetIpFrameExPrivate)
{}

/*!
    Destroys the frame object and releases all allocated resources.
*/
QKnxNetIpFrameEx::~QKnxNetIpFrameEx() = default;

QKnxNetIpFrameEx::QKnxNetIpFrameEx(QKnxNetIp::ServiceType type, const QKnxByteArray &data)
    : QKnxNetIpFrameEx(type, {}, data)
{}

QKnxNetIpFrameEx::QKnxNetIpFrameEx(QKnxNetIp::ServiceType type,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data)
    : d_ptr(new QKnxNetIpFrameExPrivate)
{
    d_ptr->m_connectionHeader = connectionHeader;
    d_ptr->m_data = data;
    d_ptr->m_header = { type, quint16(connectionHeader.size() + data.size()) };
}

QKnxNetIpFrameEx::QKnxNetIpFrameEx(const QKnxNetIpFrameHeader &header,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data)
    : d_ptr(new QKnxNetIpFrameExPrivate)
{
    d_ptr->m_header = header;
    d_ptr->m_connectionHeader = connectionHeader;
    d_ptr->m_data = data;
}

bool QKnxNetIpFrameEx::isNull() const
{
    return d_ptr->m_header.isNull() && d_ptr->m_connectionHeader.isNull() && d_ptr->m_data.isNull();
}

bool QKnxNetIpFrameEx::isValid() const
{
    if (isNull())
        return false;

    auto connHeaderValid = d_ptr->m_connectionHeader.isNull();
    switch (serviceType()) {
        case QKnxNetIp::ServiceType::TunnelingRequest:
        case QKnxNetIp::ServiceType::TunnelingAcknowledge:
        case QKnxNetIp::ServiceType::DeviceConfigurationRequest:
        case QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge:
            connHeaderValid = d_ptr->m_connectionHeader.isValid();
            break;
        default:
            break;
    }
    auto size = d_ptr->m_header.size() + d_ptr->m_connectionHeader.size() + d_ptr->m_data.size();

    return d_ptr->m_header.isValid() && connHeaderValid && d_ptr->m_header.totalSize() == size
        && d_ptr->m_header.dataSize() == (d_ptr->m_data.size() + d_ptr->m_connectionHeader.size());
}

quint16 QKnxNetIpFrameEx::size() const
{
    return d_ptr->m_header.totalSize();
}

quint16 QKnxNetIpFrameEx::dataSize() const
{
    return d_ptr->m_header.dataSize() - d_ptr->m_connectionHeader.size();
}

QKnxNetIpFrameHeader QKnxNetIpFrameEx::header() const
{
    return d_ptr->m_header;
}

void QKnxNetIpFrameEx::setHeader(const QKnxNetIpFrameHeader & header)
{
    d_ptr->m_header = header;
}

quint8 QKnxNetIpFrameEx::protocolVersion() const
{
    return d_ptr->m_header.protocolVersion();
}

QKnxNetIp::ServiceType QKnxNetIpFrameEx::serviceType() const
{
    return d_ptr->m_header.serviceType();
}

void QKnxNetIpFrameEx::setServiceType(QKnxNetIp::ServiceType type)
{
    d_ptr->m_header.setServiceType(type);
}

QKnxNetIpConnectionHeader QKnxNetIpFrameEx::connectionHeader() const
{
    return d_ptr->m_connectionHeader;
}

void QKnxNetIpFrameEx::setConnectionHeader(const QKnxNetIpConnectionHeader &header)
{
    quint16 dataSize = d_ptr->m_header.dataSize() - d_ptr->m_connectionHeader.size();
    d_ptr->m_connectionHeader = header;
    d_ptr->m_header.setDataSize(dataSize + header.size());
}

quint8 QKnxNetIpFrameEx::channelId() const
{
    return d_ptr->m_connectionHeader.channelId();
}

quint8 QKnxNetIpFrameEx::sequenceNumber() const
{
    return d_ptr->m_connectionHeader.sequenceNumber();
}

quint8 QKnxNetIpFrameEx::serviceTypeSpecificValue() const
{
    return d_ptr->m_connectionHeader.serviceTypeSpecificValue();
}

QKnxByteArray QKnxNetIpFrameEx::connectionTypeSpecificHeaderItems() const
{
    return d_ptr->m_connectionHeader.connectionTypeSpecificHeaderItems();
}

QKnxByteArray QKnxNetIpFrameEx::data() const
{
    return d_ptr->m_data;
}

const QKnxByteArray &QKnxNetIpFrameEx::constData() const
{
    return d_ptr->m_data;
}

void QKnxNetIpFrameEx::setData(const QKnxByteArray &data)
{
    auto dataSize = d_ptr->m_header.dataSize() - d_ptr->m_data.size();
    d_ptr->m_data = data;
    d_ptr->m_header.setDataSize(dataSize + data.size());
}

QKnxByteArray QKnxNetIpFrameEx::bytes() const
{
    return d_ptr->m_header.bytes() + d_ptr->m_connectionHeader.bytes() + d_ptr->m_data;
}

QKnxNetIpFrameEx QKnxNetIpFrameEx::fromBytes(const QKnxByteArray &bytes, quint16 index)
{
    auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
    if (!header.isValid())
        return {};
    index += header.size();

    QKnxNetIpConnectionHeader connHeader;
    switch (header.serviceType()) {
        case QKnxNetIp::ServiceType::TunnelingRequest:
        case QKnxNetIp::ServiceType::TunnelingAcknowledge:
        case QKnxNetIp::ServiceType::DeviceConfigurationRequest:
        case QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge: {
            connHeader = QKnxNetIpConnectionHeader::fromBytes(bytes, index);
            if (!connHeader.isValid())
                return {};
            index += connHeader.size();
        }   break;
        default:
            break;
    }

    const auto dataSize = header.totalSize() - index;
    if ((bytes.size() - index) < dataSize)
        return {};
    return { header, connHeader, bytes.mid(index, dataSize) };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpFrameEx::QKnxNetIpFrameEx(const QKnxNetIpFrameEx &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpFrameEx &QKnxNetIpFrameEx::operator=(const QKnxNetIpFrameEx &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxNetIpFrameEx::QKnxNetIpFrameEx(QKnxNetIpFrameEx &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxNetIpFrameEx &QKnxNetIpFrameEx::operator=(QKnxNetIpFrameEx &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxNetIpFrameEx::swap(QKnxNetIpFrameEx &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxNetIpFrameEx::operator==(const QKnxNetIpFrameEx &other) const
{
    return d_ptr == other.d_ptr || [&]() -> bool {
        return d_ptr->m_header == other.d_ptr->m_header
            && d_ptr->m_connectionHeader == other.d_ptr->m_connectionHeader
            && d_ptr->m_data == other.d_ptr->m_data;
    }();
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxNetIpFrameEx::operator!=(const QKnxNetIpFrameEx &other) const
{
    return !operator==(other);
}

/*!
    \internal
*/
QKnxNetIpFrameEx::QKnxNetIpFrameEx(QKnxNetIpFrameExPrivate &dd)
    : d_ptr(new QKnxNetIpFrameExPrivate(dd))
{}

/*!
    \relates QKnxNetIpFrame

    Writes the KNX \a NetIpFrame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpFrameEx &frame)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << frame.bytes().toHex();
}


























/*!
    Creates a new QKnxNetIpFrame with the given \a code.
*/

/*!
    Creates a new QKnxNetIpFrame with the given \a header and \a payload.
    The \a header is expected to be fully setup and all values describing the
    payload match the given \a payload.
    \note No adjustments are made to the function arguments.
*/

/*!
    Returns the generic code stored in the KNXnet/IP frame header.
*/

/*!
    Sets the generic code stored in the KNXnet/IP frame header.
*/

/*!
    Returns the number of bytes representing the KNXnet/IP frame including
    the header and the payload.
*/

/*!
    \relates QKnxNetIpFrame

    Writes the KNX \a NetIpFrame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpFrame &package)
{
    QDebugStateSaver _(debug);
    if (package.isValid()) {
        QDebug &dbg = debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2)
                                                << qSetPadChar(QLatin1Char('0'));
        const auto bytes = package.bytes();
        for (quint8 byte : qAsConst(bytes))
            dbg << byte;
    } else {
        debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

/*!
    \relates QKnxNetIpFrame

    Writes a KNX \a NetIpFrame to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpFrame &package)
{
    if (!package.isValid())
        return out;

    const auto bytes = package.bytes();
    for (quint8 byte : qAsConst(bytes))
        out << byte;
    return out;
}

QT_END_NAMESPACE
