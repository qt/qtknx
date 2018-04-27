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

#include "qknxnetipconnectionstaterequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpConnectionStateRequestProxy

    \inmodule QtKnx
    \brief The QKnxNetIpConnectionStateRequestProxy class provides the means to
    read a KNXnet/IP connection state request from the generic \l QKnxNetIpFrame
    class and to create a connection state request frame based on the
    information.

    Host protocols like UDP/IP that do not provide mechanisms for checking the
    lifetime of a connection need a way to tell when communication fails. To
    detect failures on the KNX or in the tunneling network, heartbeat monitoring
    is implemented by both KNXnet/IP clients and servers, as follows.
    A KNXnet/IP client sends a connection state request frame regularly to the
    KNXnet/IP server's control endpoint to check the state of a connection
    established to the server. The server responds immediately with a
    connection state response frame.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the connection state
    request information sent by a KNXnet/IP client:

    \code
        auto frame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpConnectionStateRequestProxy connectionStateRequest(netIpFrame);
        if (!connectionStateRequest.isValid())
            return;

        quint8 chanId = connectionStateRequest.channelId();
        auto hpai = connectionStateRequest.controlEndpoint();
    \endcode

    \sa builder(), QKnxNetIpConnectionStateResponseProxy
*/

/*!
    \fn QKnxNetIpConnectionStateRequestProxy::QKnxNetIpConnectionStateRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpConnectionStateRequestProxy::~QKnxNetIpConnectionStateRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpConnectionStateRequestProxy::QKnxNetIpConnectionStateRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a wrapper object to read the connection state request information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpConnectionStateRequestProxy::QKnxNetIpConnectionStateRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the ID of the communication channel prepared by the KNXnet/IP
    server.
*/
quint8 QKnxNetIpConnectionStateRequestProxy::channelId() const
{
    return m_frame.constData().value(0);
}

/*!
    Returns the control endpoint of the KNXnet/IP client sending the connection
    state request.
*/
QKnxNetIpHpai QKnxNetIpConnectionStateRequestProxy::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 2);
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpConnectionStateRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 16
         && m_frame.serviceType() == QKnxNetIp::ServiceType::ConnectionStateRequest;
}

/*!
    Returns a builder object to create a KNXnet/IP connection state request
    frame.
*/
QKnxNetIpConnectionStateRequestProxy::Builder QKnxNetIpConnectionStateRequestProxy::builder()
{
    return QKnxNetIpConnectionStateRequestProxy::Builder();
}


/*!
    \class QKnxNetIpConnectionStateRequestProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpConnectionStateRequestProxy::Builder class provides the
    means to create a KNXnet/IP connection state request frame.

    A KNXnet/IP client sends a connection state request frame regularly to the
    KNXnet/IP server's control endpoint to check the state of a connection
    established to the server. The server responds immediately with a
    connection state response frame, \l QKnxNetIpConnectionStateResponseProxy.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a connection state request is:

    \code
        auto hpai = QKnxNetIpHpaiProxy::builder().create();

        auto netIpFrame = QKnxNetIpConnectionStateRequestProxy::builder()
            .setChannelId(255)
            .setControlEndpoint(hpai)
            .create();
    \endcode

    If the KNXnet/IP client does not receive the connection state response
    within a timeout, or the status of the received response indicates that
    errors occurred, the client repeats the connection state request three
    times. It then terminates the connection by sending a disconnect request,
    \l QKnxNetIpDisconnectRequestProxy, to the server's control endpoint.
*/

/*!
    Sets the ID of the communication channel to \a channelId and returns a
    reference to the builder.
*/
QKnxNetIpConnectionStateRequestProxy::Builder &
    QKnxNetIpConnectionStateRequestProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the control endpoint of the KNXnet/IP client to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpConnectionStateRequestProxy::Builder &
    QKnxNetIpConnectionStateRequestProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP connection state request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpConnectionStateRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::ConnectionStateRequest, QKnxByteArray { m_channelId, 0x00 }
        + m_hpai.bytes() };
}

QT_END_NAMESPACE
