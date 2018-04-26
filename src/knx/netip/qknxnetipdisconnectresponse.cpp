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

#include "qknxnetipdisconnectresponse.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDisconnectResponseProxy

    \inmodule QtKnx
    \brief The QKnxNetIpDisconnectResponseProxy class provides the means to read
    a KNXnet/IP disconnection response from the generic \l QKnxNetIpFrame
    class and to create a disconnection response frame based on the information.

    A KNXnet/IP device terminates a data channel connection by sending a
    disconnection request to its communication partner's control endpoint.
    The KNXnet/IP device receiving the disconnection request acknowledges the
    operation with a KNXnet/IP disconnection response frame, which signals the
    final termination of a communication channel.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the disconnection response
    information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpDisconnectRequestProxy disconnectRequest(netIpFrame);
        if (!disconnectRequest.isValid())
            return;

        quint8 chanId = disconnectRequest.channelId();
        QKnx::NetIp::Error state = disconnectRequest.status();
    \endcode

    \sa builder(), QKnxNetIpDisconnectRequestProxy
*/

/*!
    \fn QKnxNetIpDisconnectResponseProxy::QKnxNetIpDisconnectResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDisconnectResponseProxy::~QKnxNetIpDisconnectResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDisconnectResponseProxy::QKnxNetIpDisconnectResponseProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the disconnection response information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpDisconnectResponseProxy::QKnxNetIpDisconnectResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpDisconnectResponseProxy::channelId() const
{
    return m_frame.constData().value(0);
}

/*!
    Returns the status of the communication channel between a KNXnet/IP client
    and server.
*/
QKnxNetIp::Error QKnxNetIpDisconnectResponseProxy::status() const
{
    return QKnxNetIp::Error(m_frame.constData().value(1));
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpDisconnectResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 8
        && m_frame.serviceType() == QKnxNetIp::ServiceType::DisconnectResponse;
}

/*!
    Returns a builder object to create a KNXnet/IP disconnection response frame.
*/
QKnxNetIpDisconnectResponseProxy::Builder QKnxNetIpDisconnectResponseProxy::builder()
{
    return QKnxNetIpDisconnectResponseProxy::Builder();
}


/*!
    \class QKnxNetIpDisconnectResponseProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpDisconnectResponseProxy::Builder class provides
    the means to read a KNXnet/IP disconnection response from the generic
    \l QKnxNetIpFrame class and to create a disconnection response frame
    based on the information.

    A KNXnet/IP device terminates a data channel connection by sending a
    disconnection request to its communication partner's control endpoint.
    The KNXnet/IP device receiving the disconnection request acknowledges the
    operation with a KNXnet/IP disconnection response frame, which signals the
    final termination of a communication channel.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a disconnection response is:

    \code
        auto frame = QKnxNetIpDisconnectResponseProxy::builder()
            .setChannelId(200)
            .setStatus(QKnx::NetIp::Error::None)
            .create();
    \endcode
*/

/*!
    Sets the ID of the communication channel between the KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpDisconnectResponseProxy::Builder &
    QKnxNetIpDisconnectResponseProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the status of the communication channel between the KNXnet/IP client
    and server to \a status and returns a reference to the builder.
*/
QKnxNetIpDisconnectResponseProxy::Builder &
    QKnxNetIpDisconnectResponseProxy::Builder::setStatus(QKnxNetIp::Error status)
{
    m_status = status;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP disconnection response frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpDisconnectResponseProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DisconnectResponse, QKnxByteArray { m_channelId,
        quint8(m_status) } };
}

QT_END_NAMESPACE
