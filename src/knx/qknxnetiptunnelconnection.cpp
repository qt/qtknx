/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetipconnectresponse.h"
#include "qknxnetipendpointconnection_p.h"
#include "qknxnetiptunnelconnection.h"
#include "qknxnetiptunnelingrequest.h"

QT_BEGIN_NAMESPACE

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
            // TODO: Maybe implement 03_08_04 Tunnelling v01.05.03 AS.pdf, paragraph 3.3
        }
        QKnxNetIpEndpointConnectionPrivate::process(response, dg);

        Q_Q(QKnxNetIpTunnelConnection);
        if (q->state() == QKnxNetIpTunnelConnection::Connected)
            m_individualAddress = response.responseData().individualAddress();
    }

private:
    QKnxAddress m_individualAddress;
    QKnxNetIp::TunnelingLayer m_layer;
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
    return d_func()->m_individualAddress;
}

void QKnxNetIpTunnelConnection::setIndividualAddress(const QKnxAddress &address)
{
    Q_ASSERT_X(false, "QKnxNetIpTunnelClient::setIndividualAddress", "Setting the individual "
        "address used for tunnel connections not implemented yet.");

    Q_UNUSED(address) // TODO: Maybe implement 03_08_04 Tunnelling v01.05.03 AS.pdf, paragraph 3.2
}

void QKnxNetIpTunnelConnection::sendTunnelFrame(const QKnxTunnelFrame &frame)
{
    if (state() != State::Connected)
        return;

    Q_D(QKnxNetIpTunnelConnection);
    if (d->m_layer == QKnxNetIp::TunnelingLayer::Busmonitor)
        return; // 03_08_04 Tunnelling v01.05.03, paragraph 2.4

    d->sendTunnelingRequest(frame);
}

QT_END_NAMESPACE
