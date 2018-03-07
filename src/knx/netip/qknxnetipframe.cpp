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

class QKnxNetIpFramePrivate : public QSharedData
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
QKnxNetIpFrame::QKnxNetIpFrame()
    : d_ptr(new QKnxNetIpFramePrivate)
{}

/*!
    Destroys the frame object and releases all allocated resources.
*/
QKnxNetIpFrame::~QKnxNetIpFrame() = default;

QKnxNetIpFrame::QKnxNetIpFrame(QKnxNetIp::ServiceType type, const QKnxByteArray &data)
    : QKnxNetIpFrame(type, {}, data)
{}

QKnxNetIpFrame::QKnxNetIpFrame(QKnxNetIp::ServiceType type,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data)
    : d_ptr(new QKnxNetIpFramePrivate)
{
    d_ptr->m_connectionHeader = connectionHeader;
    d_ptr->m_data = data;
    d_ptr->m_header = { type, quint16(connectionHeader.size() + data.size()) };
}

QKnxNetIpFrame::QKnxNetIpFrame(const QKnxNetIpFrameHeader &header,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data)
    : d_ptr(new QKnxNetIpFramePrivate)
{
    d_ptr->m_header = header;
    d_ptr->m_connectionHeader = connectionHeader;
    d_ptr->m_data = data;
}

bool QKnxNetIpFrame::isNull() const
{
    return d_ptr->m_header.isNull() && d_ptr->m_connectionHeader.isNull() && d_ptr->m_data.isNull();
}

bool QKnxNetIpFrame::isValid() const
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

quint16 QKnxNetIpFrame::size() const
{
    return d_ptr->m_header.totalSize();
}

quint16 QKnxNetIpFrame::dataSize() const
{
    return d_ptr->m_header.dataSize() - d_ptr->m_connectionHeader.size();
}

QKnxNetIpFrameHeader QKnxNetIpFrame::header() const
{
    return d_ptr->m_header;
}

void QKnxNetIpFrame::setHeader(const QKnxNetIpFrameHeader & header)
{
    d_ptr->m_header = header;
}

quint8 QKnxNetIpFrame::protocolVersion() const
{
    return d_ptr->m_header.protocolVersion();
}

QKnxNetIp::ServiceType QKnxNetIpFrame::serviceType() const
{
    return d_ptr->m_header.serviceType();
}

void QKnxNetIpFrame::setServiceType(QKnxNetIp::ServiceType type)
{
    d_ptr->m_header.setServiceType(type);
}

QKnxNetIpConnectionHeader QKnxNetIpFrame::connectionHeader() const
{
    return d_ptr->m_connectionHeader;
}

void QKnxNetIpFrame::setConnectionHeader(const QKnxNetIpConnectionHeader &header)
{
    quint16 dataSize = d_ptr->m_header.dataSize() - d_ptr->m_connectionHeader.size();
    d_ptr->m_connectionHeader = header;
    d_ptr->m_header.setDataSize(dataSize + header.size());
}

quint8 QKnxNetIpFrame::channelId() const
{
    return d_ptr->m_connectionHeader.channelId();
}

quint8 QKnxNetIpFrame::sequenceNumber() const
{
    return d_ptr->m_connectionHeader.sequenceNumber();
}

quint8 QKnxNetIpFrame::serviceTypeSpecificValue() const
{
    return d_ptr->m_connectionHeader.serviceTypeSpecificValue();
}

QKnxByteArray QKnxNetIpFrame::connectionTypeSpecificHeaderItems() const
{
    return d_ptr->m_connectionHeader.connectionTypeSpecificHeaderItems();
}

QKnxByteArray QKnxNetIpFrame::data() const
{
    return d_ptr->m_data;
}

const QKnxByteArray &QKnxNetIpFrame::constData() const
{
    return d_ptr->m_data;
}

void QKnxNetIpFrame::setData(const QKnxByteArray &data)
{
    auto dataSize = d_ptr->m_header.dataSize() - d_ptr->m_data.size();
    d_ptr->m_data = data;
    d_ptr->m_header.setDataSize(dataSize + data.size());
}

QKnxByteArray QKnxNetIpFrame::bytes() const
{
    return d_ptr->m_header.bytes() + d_ptr->m_connectionHeader.bytes() + d_ptr->m_data;
}

QKnxNetIpFrame QKnxNetIpFrame::fromBytes(const QKnxByteArray &bytes, quint16 index)
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
QKnxNetIpFrame::QKnxNetIpFrame(const QKnxNetIpFrame &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpFrame &QKnxNetIpFrame::operator=(const QKnxNetIpFrame &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxNetIpFrame::QKnxNetIpFrame(QKnxNetIpFrame &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxNetIpFrame &QKnxNetIpFrame::operator=(QKnxNetIpFrame &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxNetIpFrame::swap(QKnxNetIpFrame &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxNetIpFrame::operator==(const QKnxNetIpFrame &other) const
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
bool QKnxNetIpFrame::operator!=(const QKnxNetIpFrame &other) const
{
    return !operator==(other);
}

/*!
    \internal
*/
QKnxNetIpFrame::QKnxNetIpFrame(QKnxNetIpFramePrivate &dd)
    : d_ptr(new QKnxNetIpFramePrivate(dd))
{}

/*!
    \relates QKnxNetIpFrame

    Writes the KNX \a NetIpFrame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpFrame &frame)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << frame.bytes().toHex();
}

QT_END_NAMESPACE
