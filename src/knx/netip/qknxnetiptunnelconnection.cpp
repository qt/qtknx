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

#include "qknxnetipconnectresponse.h"
#include "qknxnetipendpointconnection_p.h"
#include "qknxnetiptunnelconnection.h"
#include "qknxnetiptunnelingrequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTunnelConnection

    \inmodule QtKnx
    \brief The QKnxNetIpTunnelConnection class enables the opening and handling
    of a client connection to a KNXnet/IP server.

    The QKnxNetIpTunnelConnection is a data connection between a client and a
    KNXnet/IP server endpoint. This is used to access functionalities of devices
    on a KNX bus. The IP address of the client must be set. It is then possible
    to connect to a chosen KNXnet/IP server and to send \l QKnxTunnelFrame
    frames to the KNXnet/IP server.

    The class takes care of connecting to the server, asking for a data
    connection, and monitoring the connection:

    \code
        QKnxNetIpTunnelConnection tunnel;
        QHostAddress clientLocalAddress = ...
        tunnel.setLocalAddress(clientLocalAddress);

        QHostAddress knxNetIpServerAddress = ...
        quint16 knxNetIpServerDataEndPointPort = ...
        tunnel.connectToHost(knxNetIpServerAddress, knxNetIpServerDataEndPointPort);

        QKnxTunnelFrame frame = ...
        tunnel.sendTunnelFrame(frame);
    \endcode
*/

class QKnxNetIpTunnelConnectionPrivate : public QKnxNetIpEndpointConnectionPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpTunnelConnection)

public:
    QKnxNetIpTunnelConnectionPrivate(const QHostAddress &a, quint16 p, QKnxNetIp::TunnelingLayer l)
        : QKnxNetIpEndpointConnectionPrivate(a, p, QKnxNetIpCri(l), 1, QKnxNetIp::TunnelingRequestTimeout)
        , m_layer(l)
    {}

    void process(const QKnxCemiFrame &frame) override
    {
        Q_Q(QKnxNetIpTunnelConnection);
        emit q->receivedTunnelFrame(frame);
    }

    void process(const QKnxNetIpConnectResponse &response, const QNetworkDatagram &dg) override
    {
        if (response.status() == QKnxNetIp::Error::NoMoreUniqueConnections) {
            Q_ASSERT_X(false, "QKnxNetIpTunnelConnectionPrivate::process", "NoMoreUniqueConnections "
                "error handling not implemented yet.");
            // TODO: Maybe implement 03_08_04 Tunneling v01.05.03 AS.pdf, paragraph 3.3
        }

        Q_Q(QKnxNetIpTunnelConnection);
        if (q->state() != QKnxNetIpTunnelConnection::Connected)
            m_address = response.responseData().individualAddress();
        QKnxNetIpEndpointConnectionPrivate::process(response, dg);
    }

private:
    QKnxAddress m_address;
    QKnxNetIp::TunnelingLayer m_layer { QKnxNetIp::TunnelingLayer::Unknown };
};

QKnxNetIpTunnelConnection::QKnxNetIpTunnelConnection(QObject *parent)
    : QKnxNetIpTunnelConnection({ QHostAddress::LocalHost }, 0, QKnxNetIp::TunnelingLayer::Link,
        parent)
{}

QKnxNetIpTunnelConnection::QKnxNetIpTunnelConnection(const QHostAddress &localAddress,
        QObject *parent)
    : QKnxNetIpTunnelConnection(localAddress, 0, QKnxNetIp::TunnelingLayer::Link, parent)
{}

QKnxNetIpTunnelConnection::QKnxNetIpTunnelConnection(const QHostAddress &localAddress,
        quint16 localPort, QObject *parent)
    : QKnxNetIpTunnelConnection(localAddress, localPort, QKnxNetIp::TunnelingLayer::Link, parent)
{}

QKnxNetIpTunnelConnection::QKnxNetIpTunnelConnection(const QHostAddress &localAddress,
        quint16 localPort, QKnxNetIp::TunnelingLayer layer, QObject *parent)
    : QKnxNetIpEndpointConnection(*new QKnxNetIpTunnelConnectionPrivate(localAddress, localPort,
        layer), parent)
{}

QKnxAddress QKnxNetIpTunnelConnection::individualAddress() const
{
    return d_func()->m_address;
}

void QKnxNetIpTunnelConnection::setIndividualAddress(const QKnxAddress &address)
{
    Q_ASSERT_X(false, "QKnxNetIpTunnelClient::setIndividualAddress", "Setting the individual "
        "address used for tunnel connections not implemented yet.");

    Q_UNUSED(address) // TODO: Maybe implement 03_08_04 Tunneling v01.05.03 AS.pdf, paragraph 3.2
}

QKnxNetIp::TunnelingLayer QKnxNetIpTunnelConnection::layer() const
{
    return d_func()->m_layer;
}

void QKnxNetIpTunnelConnection::setTunnelingLayer(QKnxNetIp::TunnelingLayer layer)
{
    if (state() != State::Disconnected)
        return;
    d_func()->m_layer = layer;
}

bool QKnxNetIpTunnelConnection::sendTunnelFrame(const QKnxTunnelFrame &frame)
{
    if (state() != State::Connected)
        return false;

    Q_D(QKnxNetIpTunnelConnection);
    if (d->m_layer == QKnxNetIp::TunnelingLayer::Busmonitor)
        return false; // 03_08_04 Tunneling v01.05.03, paragraph 2.4

    return d->sendTunnelingRequest(frame);
}

QT_END_NAMESPACE
