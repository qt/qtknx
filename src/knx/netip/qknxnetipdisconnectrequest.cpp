/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include "qknxnetipdisconnectrequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDisconnectRequestProxy

    \inmodule QtKnx
    \brief The QKnxNetIpDisconnectRequestProxy class provides the means to read
    a KNXnet/IP disconnection request from the generic \l QKnxNetIpFrame
    class and to create a disconnection request frame based on the information.

    A KNXnet/IP client terminates a data channel connection to a KNXnet/IP
    server by sending a disconnection request to the server's control endpoint.
    The client should try to disconnect gracefully, even under error conditions.

    The server may disconnect from the client by sending a disconnection
    request due to internal problems or reception of invalid data packets.
    However, it is recommended to let the client terminate the connection.

    The KNXnet/IP device receiving the disconnection request acknowledges the
    operation with a KNXnet/IP disconnection response frame,
    \l QKnxNetIpDisconnectResponseProxy, which signals the final termination
    of a communication channel.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the disconnection request
    information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpDisconnectRequestProxy disconnectRequest(netIpFrame);
        if (!disconnectRequest.isValid())
            return;

        quint8 chanId = disconnectRequest.channelId();
        auto hpai = disconnectRequest.controlEndpoint();
    \endcode

    \sa builder()
*/

/*!
    \fn QKnxNetIpDisconnectRequestProxy::QKnxNetIpDisconnectRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDisconnectRequestProxy::~QKnxNetIpDisconnectRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDisconnectRequestProxy::QKnxNetIpDisconnectRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the disconnection request information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpDisconnectRequestProxy::QKnxNetIpDisconnectRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the ID of the communication channel between the KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpDisconnectRequestProxy::channelId() const
{
    return m_frame.constData().value(0);
}

/*!
    Returns the control endpoint of the KNXnet/IP device sending the
    disconnection request.
*/
QKnxNetIpHpai QKnxNetIpDisconnectRequestProxy::controlEndpoint() const
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
bool QKnxNetIpDisconnectRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 16
         && m_frame.serviceType() == QKnxNetIp::ServiceType::DisconnectRequest;
}

/*!
    Returns a builder object to create a KNXnet/IP disconnection request frame.
*/
QKnxNetIpDisconnectRequestProxy::Builder QKnxNetIpDisconnectRequestProxy::builder()
{
    return QKnxNetIpDisconnectRequestProxy::Builder();
}


/*!
    \class QKnxNetIpDisconnectRequestProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpDisconnectRequestProxy::Builder class provides the means
    to read a KNXnet/IP disconnection request from the generic \l QKnxNetIpFrame
    class and to create a disconnection request frame based on the information.

    A KNXnet/IP client terminates a data channel connection to a KNXnet/IP
    server by sending a disconnection request to the server's control endpoint.
    The client should try to disconnect gracefully, even under error conditions.

    The server may disconnect from the client by sending a disconnection
    request due to internal problems or reception of invalid data packets.
    However, it is recommended to let the client terminate the connection.

    The KNXnet/IP device receiving the disconnection request acknowledges the
    operation with a KNXnet/IP disconnection response frame,
    \l QKnxNetIpDisconnectResponseProxy, which signals the final termination of
    a communication channel.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a disconnection request is:

    \code
        auto netIpFrame = QKnxNetIpDisconnectRequestProxy::builder()
            .setChannelId(200)
            .setControlEndpoint(QKnxNetIpHpaiProxy::builder()
                .setHostAddress(QHostAddress::LocalHost)
                .setPort(3671).create())
            .create();
    \endcode
*/

/*!
    Sets the ID of the communication channel between the KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpDisconnectRequestProxy::Builder &
    QKnxNetIpDisconnectRequestProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the control endpoint of the KNXnet/IP device sending the disconnection
    request to \a hpai and returns a reference to the builder.
*/
QKnxNetIpDisconnectRequestProxy::Builder &
    QKnxNetIpDisconnectRequestProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP disconnection request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpDisconnectRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DisconnectRequest, QKnxByteArray { m_channelId, 0x00 }
        + m_hpai.bytes() };
}

QT_END_NAMESPACE
