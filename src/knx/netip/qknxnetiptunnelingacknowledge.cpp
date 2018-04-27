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

#include "qknxnetiptunnelingacknowledge.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTunnelingAcknowledgeProxy

    \inmodule QtKnx
    \brief The QKnxNetIpTunnelingAcknowledgeProxy class provides the means to
    read a tunneling acknowledgment from the generic \l QKnxNetIpFrame class
    and to create a KNXnet/IP frame based on the information.

    \e Tunneling means that Engineering Tool Software (ETS) sends a single
    KNX frame in a KNXnet/IP frame and waits until the response arrives or
    a  time-out is reached.

    A tunneling request is sent by a KNXnet/IP client to establish a data
    connection to the endpoint of a KNXnet/IP server for sending a KNX frame.
    When a KNXnet/IP server receives a tunneling request, it immediately sends
    a tunneling acknowledgment to the KNXnet/IP client to confirm the reception
    of the request and then processes the received KNX frame.

    A tunneling acknowledgment frame contains the ID and status of the
    communication channel between a KNXnet/IP client and server, as well
    as the sequence number of a tunneling request frame.

    The server only processes frames with the expected sequence number or one
    less the expected sequence number. It discards frames with other sequence
    numbers and does not send a tunneling acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the tunneling
    acknowledgment information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpTunnelingAcknowledgeProxy tunnelAck(netIpFrame);
        if (!tunnelAck.isValid())
            return;

        auto chanId = tunnelAck.channelId();
        auto seqNum = tunnelAck.sequenceNumber();
        auto tunnelStatus = tunnelAck.status();
    \endcode

    \sa builder(), QKnxNetIpTunnelingRequestProxy
*/

/*!
    \fn QKnxNetIpTunnelingAcknowledgeProxy::QKnxNetIpTunnelingAcknowledgeProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingAcknowledgeProxy::~QKnxNetIpTunnelingAcknowledgeProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingAcknowledgeProxy::QKnxNetIpTunnelingAcknowledgeProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the tunneling acknowledgment information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpTunnelingAcknowledgeProxy::QKnxNetIpTunnelingAcknowledgeProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpTunnelingAcknowledgeProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 10
        && m_frame.serviceType() == QKnxNetIp::ServiceType::TunnelingAcknowledge;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpTunnelingAcknowledgeProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a tunneling acknowledgment frame.
*/
quint8 QKnxNetIpTunnelingAcknowledgeProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns the status of the communication channel between a KNXnet/IP client
    and server.
*/
QKnxNetIp::Error QKnxNetIpTunnelingAcknowledgeProxy::status() const
{
    return QKnxNetIp::Error(m_frame.serviceTypeSpecificValue());
}

/*!
    Returns a builder object to create a KNXnet/IP tunneling acknowledgment
    frame.
*/
QKnxNetIpTunnelingAcknowledgeProxy::Builder QKnxNetIpTunnelingAcknowledgeProxy::builder()
{
    return QKnxNetIpTunnelingAcknowledgeProxy::Builder();
}


/*!
    \class QKnxNetIpTunnelingAcknowledgeProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpTunnelingAcknowledgeProxy::Builder class provides the
    means to create a KNXnet/IP tunneling acknowledgment frame.

    A tunneling acknowledgment frame contains the ID and status of the
    communication channel between a KNXnet/IP client and server, as well
    as the sequence number of a tunneling request frame.

    The server only processes frames with the expected sequence number or one
    less the expected sequence number. It discards frames with other sequence
    numbers and does not send a tunneling acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a tunneling acknowledgment is:

    \code
        auto netIpFrame = QKnxNetIpTunnelingAcknowledgeProxy::builder()
            .setChannelId(4)
            .setSequenceNumber(3)
            .setStatus(QKnx::NetIp::Error::None)
            .create()
    \endcode

    If the KNXnet/IP client does not receive a tunneling acknowledgment within
    the timeout of one second or the status of a received acknowledgment frame
    indicates that errors occurred, the client repeats the tunneling request
    frame once with the same sequence number and then terminates the connection
    by sending a disconnection request, \l QKnxNetIpDisconnectRequestProxy, to
    the server's control endpoint.
*/


/*!
    Sets the ID of the communication channel between a KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpTunnelingAcknowledgeProxy::Builder &
    QKnxNetIpTunnelingAcknowledgeProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a tunneling request frame to \a sequenceNumber
    and returns a reference to the builder.
*/
QKnxNetIpTunnelingAcknowledgeProxy::Builder &
    QKnxNetIpTunnelingAcknowledgeProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the status of the communication channel between a KNXnet/IP client and
    server to \a status and returns a reference to the builder.
*/
QKnxNetIpTunnelingAcknowledgeProxy::Builder &
    QKnxNetIpTunnelingAcknowledgeProxy::Builder::setStatus(QKnxNetIp::Error status)
{
    m_status = status;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP tunneling request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpTunnelingAcknowledgeProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::TunnelingAcknowledge,
        QKnxNetIpConnectionHeader { m_channelId, m_sequenceNumber, quint8(m_status) } };
}

QT_END_NAMESPACE
