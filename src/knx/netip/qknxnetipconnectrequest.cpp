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

#include "qknxnetipconnectrequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpConnectRequestProxy

    \inmodule QtKnx
    \brief The QKnxNetIpConnectRequestProxy class provides the means to read
    a connection request from the generic \l QKnxNetIpFrame class and to
    create a KNXnet/IP frame based on the information.

    A connection request is sent by a KNXnet/IP client to establish a
    communication channel to a KNXnet/IP server. The connection request
    provides information about the requested connection type, such as
    data tunneling or remote logging, general and connection type-specific
    options, such as data link layer or busmonitor mode, and the data
    endpoint host address protocol information (HPAI) that the client
    wants to use for this communication channel.

    In most programs, the QKnxNetIpConnectRequestProxy class will not be used
    directly. Instead, the \l QKnxNetIpTunnel or
    \l QKnxNetIpDeviceManagement class are used to establish a
    functional connection to a KNXnet/IP server.

    Before sending a connection request, the KNXnet/IP client should use
    \l QKnxNetIpServerDescriptionAgent and \l QKnxNetIpServerInfo to check
    that the server supports the requested connection type and options.

    The following code sample illustrates how to read the connection request
    information sent by a KNXnet/IP client:

    \code
        auto frame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpConnectRequestProxy request(frame);
        if (!request.isValid())
            return;

        auto ctrlEndpoint = request.controlEndpoint();
        auto dataEndpoint = request.dataEndpoint();
        auto cri = request. requestInformation();
    \endcode

    \sa builder()
*/

/*!
    \fn QKnxNetIpConnectRequestProxy::QKnxNetIpConnectRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpConnectRequestProxy::~QKnxNetIpConnectRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpConnectRequestProxy::QKnxNetIpConnectRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the connection request information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpConnectRequestProxy::QKnxNetIpConnectRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the control endpoint of the KNXnet/IP client sending the connection
    request.
*/
QKnxNetIpHpai QKnxNetIpConnectRequestProxy::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

/*!
    Returns the data endpoint of the KNXnet/IP client sending the connection
    request.
*/
QKnxNetIpHpai QKnxNetIpConnectRequestProxy::dataEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 8);
}

/*!
    Returns the connection request information (CRI) set by the KNXnet/IP client
    for the requested connection.
*/
QKnxNetIpCri QKnxNetIpConnectRequestProxy::requestInformation() const
{
    return QKnxNetIpCri::fromBytes(m_frame.constData(), 16);
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpConnectRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::ConnectRequest
        && m_frame.size() >= 24;
}

/*!
    Returns a builder object to create a KNXnet/IP connection request frame.
*/
QKnxNetIpConnectRequestProxy::Builder QKnxNetIpConnectRequestProxy::builder()
{
    return QKnxNetIpConnectRequestProxy::Builder();
}


// -- QKnxNetIpConnectRequestProxy::Builder

/*!
    \class QKnxNetIpConnectRequestProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpConnectRequestProxy

    \brief The QKnxNetIpConnectRequestProxy::Builder class provides the means to
    create a KNXnet/IP connection request frame.

    A KNXnet/IP connection request contains the information that is needed to
    establish a communication channel to a KNXnet/IP server. That is, the data
    and control endpoint host address protocol information (HPAI) that the
    client wants to use for this communication channel.

    In most programs, the QKnxNetIpConnectRequestProxy::Builder class will not be
    used directly. Instead, the \l QKnxNetIpTunnel or
    \l QKnxNetIpDeviceManagement class are used to establish a
    functional connection to a KNXnet/IP server.

    The common way to create a connection request is:

    \code
        // setup control (HPAI) and data endpoint (HPAI)
        auto crtlHpai = QKnxNetIpHpaiProxy::builder()
            ...
            .create();
        auto dataHpai = QKnxNetIpHpaiProxy::builder()
            ...
            .create()

        // setup connection request information (CRI)
        auto cri = QKnxNetIpCriProxy::builder()
            ....
            .create();

        auto connectRequest = QKnxNetIpConnectRequestProxy::builder
            .setControlEndpoint(crtlHpai)
            .setDataEndpoint(dataHpai)
            .setRequestInformation(cri)
            .create();

        controlSocket->writeDatagram(connectRequest.bytes().toByteArray(),
            remoteControlEndpointAddress, remoteControlEndpointPort);
    \endcode

    After sending the connection request, the KNXnet/IP client waits for the
    host protocol dependent time for the response frame from the KNXnet/IP
    server, \l QKnxNetIpConnectResponseProxy. After the timeout, received response
    frames are ignored by the client until it sends another connection request.
*/

/*!
    Sets the control endpoint of the KNXnet/IP server to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpConnectRequestProxy::Builder &
    QKnxNetIpConnectRequestProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_ceHpai = hpai;
    return *this;
}

/*!
    Sets the data endpoint of the KNXnet/IP server to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpConnectRequestProxy::Builder &
    QKnxNetIpConnectRequestProxy::Builder::setDataEndpoint(const QKnxNetIpHpai &hpai)
{
    m_deHpai = hpai;
    return *this;
}

/*!
    Sets the connection request information (CRI) to \a cri and returns a
    reference to the builder.
*/
QKnxNetIpConnectRequestProxy::Builder &
    QKnxNetIpConnectRequestProxy::Builder::setRequestInformation(const QKnxNetIpCri &cri)
{
    m_cri = cri;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP connection request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpConnectRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::ConnectRequest, m_ceHpai.bytes() + m_deHpai.bytes() + m_cri
        .bytes() };
}

QT_END_NAMESPACE
