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

#include "qknxnetiptunnelingrequest.h"
#include "qknxlinklayerframebuilder.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTunnelingRequestProxy

    \inmodule QtKnx
    \ingroup qtknx-tunneling
    \ingroup qtknx-netip

    \brief The QKnxNetIpTunnelingRequestProxy class provides the means to read
    a tunneling request from the generic \l QKnxNetIpFrame class and to create
    a KNXnet/IP frame based on the information.

    \e Tunneling means that Engineering Tool Software (ETS) sends a single KNX
    frame in an KNXnet/IP frame and waits until the response arrives or a
    time-out is reached.

    A tunneling request is sent by a KNXnet/IP client to establish a data
    connection to the endpoint of a KNXnet/IP server for sending a KNX frame.
    A tunneling request frame contains the ID of the communication channel
    between a KNXnet/IP client and server, the sequence number of the frame,
    and the KNX frame in the common external message interface (cEMI) format.

    The KNXnet/IP server discards frames with sequence numbers that are not
    the expected sequence numbers or one less the expected sequence numbers
    and does not send a tunneling acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement class is used to
    establish a functional connection to a KNXnet/IP server.

    Before sending a tunneling request, the KNXnet/IP client should use
    \l QKnxNetIpServerDescriptionAgent and \l QKnxNetIpServerInfo to check
    that the server supports the requested connection type and options.

    The following code sample illustrates how to read the tunneling request
    information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpTunnelingRequestProxy tunnelRequest(netIpFrame);
        if (!tunnelRequest.isValid())
            return;

        auto chanId = tunnelRequest.channelId();
        auto seqNum = tunnelRequest.sequenceNumber();
        auto knxLinkFrame = tunnelRequest.cemi();
    \endcode

    \sa builder(), QKnxNetIpTunnelingAcknowledgeProxy,
    {Qt KNX Tunneling Classes}, {Qt KNXnet/IP Connection Classes}
*/
/*!
    \fn QKnxNetIpTunnelingRequestProxy::QKnxNetIpTunnelingRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingRequestProxy::~QKnxNetIpTunnelingRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingRequestProxy::QKnxNetIpTunnelingRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the tunneling request information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpTunnelingRequestProxy::QKnxNetIpTunnelingRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpTunnelingRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 12 && cemi().isValid()
        && m_frame.serviceType() == QKnxNetIp::ServiceType::TunnelingRequest;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpTunnelingRequestProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a tunneling request frame.
*/
quint8 QKnxNetIpTunnelingRequestProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns the KNX frame in the cEMI format.
*/
QKnxLinkLayerFrame QKnxNetIpTunnelingRequestProxy::cemi() const
{
    return QKnxLinkLayerFrame::builder()
        .setMedium(QKnx::MediumType::NetIP)
        .setData(m_frame.constData())
        .createFrame();
}

/*!
    Returns a builder object to create a KNXnet/IP tunneling request frame.
*/
QKnxNetIpTunnelingRequestProxy::Builder QKnxNetIpTunnelingRequestProxy::builder()
{
    return QKnxNetIpTunnelingRequestProxy::Builder();
}


/*!
    \class QKnxNetIpTunnelingRequestProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpTunnelingRequestProxy

    \brief The QKnxNetIpTunnelingRequestProxy::Builder class provides the means
    to create a KNXnet/IP tunneling request frame.

    A KNXnet/IP tunneling request contains the ID of the communication channel
    between a KNXnet/IP client and server, the sequence number of the frame,
    and the KNX frame in the common external message interface (cEMI) format.

    The KNXnet/IP server discards frames with sequence numbers that are not
    the expected sequence numbers or one less the expected sequence numbers
    and does not send a tunneling acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement class is used to
    establish a functional connection to a KNXnet/IP server.

    The common way to create a tunneling request is:

    \code
        QKnxLinkLayerFrame linkFrame;
        auto tunnelRequest = QKnxNetIpTunnelingRequestProxy.builder()
            .setCemi(linkFrame)
            .setChannelId(10)
            .setSequenceNumber(0)
            .create();
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
QKnxNetIpTunnelingRequestProxy::Builder &
    QKnxNetIpTunnelingRequestProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a tunneling request frame to \a sequenceNumber
    and returns a reference to the builder.
*/
QKnxNetIpTunnelingRequestProxy::Builder &
    QKnxNetIpTunnelingRequestProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the KNX frame within the tunneling request frame to \a cemi and returns
    a reference to the builder.
*/
QKnxNetIpTunnelingRequestProxy::Builder &
    QKnxNetIpTunnelingRequestProxy::Builder::setCemi(const QKnxLinkLayerFrame &cemi)
{
    m_cemi = cemi;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP tunneling request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpTunnelingRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::TunnelingRequest, { m_channelId, m_sequenceNumber },
        m_cemi.bytes() };
}

QT_END_NAMESPACE
