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

#include "qknxnetipdeviceconfigurationacknowledge.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceConfigurationAcknowledgeProxy

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceConfigurationAcknowledgeProxy class provides the
    means to read a device configuration acknowledgment from the generic
    \l QKnxNetIpFrame class and to create a KNXnet/IP frame based on the
    information.

    A KNXnet/IP client connects to a configuration and management data endpoint
    of a KNXnet/IP server to send device configuration request frames and to
    receive device configuration acknowledgment frames. A device configuration
    request is used to read and write KNXnet/IP device configuration data. When
    a server receives a configuration request, it responds with a device
    configuration acknowledgment to confirm the reception of the request.

    A device configuration acknowledgment frame contains the ID and status of
    the communication channel between a KNXnet/IP client and server, as well as
    the sequence number of the device configuration request frame.

    The KNXnet/IP server discards device configuration request frames with
    unexpected sequence numbers and does not send a device configuration
    acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the device configuration
    acknowledgment information sent by a KNXnet/IP client:

    \code
        auto frame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpDeviceConfigurationAcknowledgeProxy configAck(netIpFrame);
        if (!configAck.isValid())
            return;

        quint8 chanId = configAck.channelId();
        quint8 seqNum = configAck.sequenceNumber();
        QKnx::NetIp::Error error = configAck.status();
    \endcode

    \sa builder(), QKnxNetIpDeviceConfigurationRequestProxy
*/

/*!
    \fn QKnxNetIpDeviceConfigurationAcknowledgeProxy::QKnxNetIpDeviceConfigurationAcknowledgeProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDeviceConfigurationAcknowledgeProxy::~QKnxNetIpDeviceConfigurationAcknowledgeProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDeviceConfigurationAcknowledgeProxy::QKnxNetIpDeviceConfigurationAcknowledgeProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the device configuration acknowledgment
    information carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpDeviceConfigurationAcknowledgeProxy::
    QKnxNetIpDeviceConfigurationAcknowledgeProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpDeviceConfigurationAcknowledgeProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 10
        && m_frame.serviceType() == QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpDeviceConfigurationAcknowledgeProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a device configuration request frame.
*/
quint8 QKnxNetIpDeviceConfigurationAcknowledgeProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns the status of the communication channel between a KNXnet/IP client
    and server.
*/
QKnxNetIp::Error QKnxNetIpDeviceConfigurationAcknowledgeProxy::status() const
{
    return QKnxNetIp::Error(m_frame.serviceTypeSpecificValue());
}

/*!
    Returns a builder object to create a KNXnet/IP device configuration
    acknowledgment frame.
*/
QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder
    QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder()
{
    return QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder();
}


/*!
    \class QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpDeviceConfigurationAcknowledgeProxy

    \brief The QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder class
    provides the means to create a KNXnet/IP device configuration
    acknowledgment frame.

    A KNXnet/IP client connects to a configuration and management data endpoint
    of a KNXnet/IP server to send device configuration request frames and to
    receive device configuration acknowledgment frames. A device configuration
    request is used to read and write KNXnet/IP device configuration data. When
    a server receives a configuration request, it responds with a device
    configuration acknowledgment to confirm the reception of the request.

    A device configuration acknowledgment frame contains the ID and status of
    the communication channel between a KNXnet/IP client and server, as well as
    the sequence number of the device configuration request frame.

    The KNXnet/IP server discards device configuration request frames with
    unexpected sequence numbers and does not send a device configuration
    acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the device configuration
    request information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder()
            .setChannelId(200)
            .setSequenceNumber(250)
            .setStatus(QKnx::NetIp::Error::ConnectionId)
            .create();
    \endcode

    If the KNXnet/IP client does not receive a device configuration
    acknowledgment within the specified timeout or the status of a received
    acknowledgment frame indicates that errors occurred, the client repeats
    the device configuration request three times and then terminates the
    connection by sending a disconnection request frame,
    \l QKnxNetIpDisconnectRequestProxy, to the server’s control endpoint.
*/

/*!
    Sets the ID of the communication channel between a KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder &
    QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a device configuration request frame to
    \a sequenceNumber and returns a reference to the builder.
*/
QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder &
    QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the status of the communication channel between a KNXnet/IP client and
    server to \a status and returns a reference to the builder.
*/
QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder &
    QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder::setStatus(QKnxNetIp::Error status)
{
    m_status = status;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP device configuration request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpDeviceConfigurationAcknowledgeProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge,
        QKnxNetIpConnectionHeader { m_channelId, m_sequenceNumber, quint8(m_status) } };
}

QT_END_NAMESPACE
