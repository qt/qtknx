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

#include "qknxnetipconnectresponse.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpConnectResponseProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpConnectResponseProxy class provides the means to read
    a connection response from the generic \l QKnxNetIpFrame class and to
    create a KNXnet/IP frame based on the information.

    A connection response frame is sent by a KNXnet/IP server in response to a
    connection request received from a KNXnet/IP client. The connection response
    provides the status of the request. If the server accepted the request, the
    frame also contains the identifier and host address protocol information
    (HPAI) of the data endpoint that the server prepared for the communication
    channel.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the connection response
    information sent by a KNXnet/IP server:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);
        const QKnxNetIpConnectResponseProxy connectResponse(netIpFrame);
        if (!connectResponse.isValid())
            return;

        auto chanId = connectResponse.channelId();
        auto data = connectResponse.responseData();
        auto endPoint = connectResponse.dataEndpoint();
        auto netIpError = connectResponse.status();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpConnectResponseProxy::QKnxNetIpConnectResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpConnectResponseProxy::~QKnxNetIpConnectResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpConnectResponseProxy::QKnxNetIpConnectResponseProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the connection response information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpConnectResponseProxy::QKnxNetIpConnectResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpConnectResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::ConnectResponse
        && (status() == QKnxNetIp::Error::None ? m_frame.size() >= 18 : true);
}

/*!
    Returns the ID of the communication channel prepared by the KNXnet/IP
    server.
*/
quint8 QKnxNetIpConnectResponseProxy::channelId() const
{
    return m_frame.constData().value(0);
}

/*!
    Returns the status of the connection request received from a KNXnet/IP
    client.

    \sa QKnxNetIpConnectRequestProxy
*/
QKnxNetIp::Error QKnxNetIpConnectResponseProxy::status() const
{
    return QKnxNetIp::Error(m_frame.constData().value(1));
}

/*!
    Returns the data endpoint of the KNXnet/IP server.
*/
QKnxNetIpHpai QKnxNetIpConnectResponseProxy::dataEndpoint() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 2);
}

/*!
    Returns a connection response data (CRD) structure from the KNXnet/IP
    connection request frame.
*/
QKnxNetIpCrd QKnxNetIpConnectResponseProxy::responseData() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpCrd::fromBytes(m_frame.constData(), 10);
}

/*!
    Returns a builder object to create a KNXnet/IP connection response frame.
*/
QKnxNetIpConnectResponseProxy::Builder QKnxNetIpConnectResponseProxy::builder()
{
    return QKnxNetIpConnectResponseProxy::Builder();
}


/*!
    \class QKnxNetIpConnectResponseProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpConnectResponseProxy

    \brief The QKnxNetIpConnectResponseProxy::Builder class provides the means
    to create a KNXnet/IP connection response frame.

    A KNXnet/IP connection response contains the status of the corresponding
    connection request. If the server accepted the request, the frame also
    contains an identifier and the host address protocol information (HPAI) of
    the data endpoint that the server prepared for the communication channel.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a connection response is:

    \code
        QKnxNetIpHpai hpai;
        QKnxNetIpCrdProxy data;
        auto netIpFrame = QKnxNetIpConnectResponseProxy::builder()
            .setChannelId(200)
            .setStatus(QKnx::NetIp::Error::None)
            .setDataEndpoint(hpai)
            .setResponseData(data)
            .create();
    \endcode
*/

/*!
    Sets the ID of the communication channel prepared by the KNXnet/IP
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpConnectResponseProxy::Builder &
    QKnxNetIpConnectResponseProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the status of the connection request received from a KNXnet/IP
    client to \a status and returns a reference to the builder.
*/
QKnxNetIpConnectResponseProxy::Builder &
    QKnxNetIpConnectResponseProxy::Builder::setStatus(QKnxNetIp::Error status)
{
    m_status = status;
    return *this;
}

/*!
    Sets the data endpoint of the KNXnet/IP server to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpConnectResponseProxy::Builder &
    QKnxNetIpConnectResponseProxy::Builder::setDataEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Sets the connection response data (CRD) to \a crd and returns a reference to
    the builder.
*/
QKnxNetIpConnectResponseProxy::Builder &
    QKnxNetIpConnectResponseProxy::Builder::setResponseData(const QKnxNetIpCrd &crd)
{
    m_crd = crd;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP connection response frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpConnectResponseProxy::Builder::create() const
{
    QKnxByteArray data { m_channelId, quint8(m_status) };
    if (m_status == QKnxNetIp::Error::None)
        data += m_hpai.bytes() + m_crd.bytes();
    return { QKnxNetIp::ServiceType::ConnectResponse, data };
}

QT_END_NAMESPACE
