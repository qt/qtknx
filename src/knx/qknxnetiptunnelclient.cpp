/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipclient_p.h"
#include "qknxnetipconnectresponse.h"
#include "qknxnetiptunnelclient.h"
#include "qknxnetiptunnelingrequest.h"

QT_BEGIN_NAMESPACE

class QKnxNetIpTunnelClientPrivate : public QKnxNetIpClientPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpTunnelClient)

public:
    QKnxNetIpTunnelClientPrivate(const QHostAddress &a, quint16 p, QKnxNetIp::TunnelingLayer layer)
        : QKnxNetIpClientPrivate(a, p, QKnxNetIpCRI(layer), 1, QKnxNetIp::TunnelingRequestTimeout)
        , m_layer(layer)
    {}

    void process(const QKnxTunnelingFrame &frame) override
    {
        Q_Q(QKnxNetIpTunnelClient);
        emit q->receivedTunnelingFrame(frame);
    }

    void process(const QKnxNetIpConnectResponse &response, const QNetworkDatagram &dg) override
    {
        if (response.status() == QKnxNetIp::Error::NoMoreUniqueConnections) {
            Q_ASSERT_X(false, "QKnxNetIpTunnelClientPrivate::process", "NoMoreUniqueConnections "
                "error handling not implemented yet.");
            // TODO: Maybe implement 03_08_04 Tunnelling v01.05.03 AS.pdf, paragraph 3.3
        }
        QKnxNetIpClientPrivate::process(response, dg);

        Q_Q(QKnxNetIpTunnelClient);
        if (q->state() == QKnxNetIpTunnelClient::Connected)
            m_individualAddress = response.responseData().individualAddress();
    }

private:
    QKnxAddress m_individualAddress;
    QKnxNetIp::TunnelingLayer m_layer;
};

QKnxNetIpTunnelClient::QKnxNetIpTunnelClient(QObject *parent)
    : QKnxNetIpTunnelClient({ QHostAddress::LocalHost }, 0, QKnxNetIp::TunnelingLayer::Link, parent)
{}

QKnxNetIpTunnelClient::QKnxNetIpTunnelClient(const QHostAddress &localAddress, QObject *parent)
    : QKnxNetIpTunnelClient(localAddress, 0, QKnxNetIp::TunnelingLayer::Link, parent)
{}

QKnxNetIpTunnelClient::QKnxNetIpTunnelClient(const QHostAddress &localAddress, quint16 localPort,
        QObject *parent)
    : QKnxNetIpTunnelClient(localAddress, localPort, QKnxNetIp::TunnelingLayer::Link, parent)
{}

QKnxNetIpTunnelClient::QKnxNetIpTunnelClient(const QHostAddress &localAddress, quint16 localPort,
        QKnxNetIp::TunnelingLayer layer, QObject *parent)
    : QKnxNetIpClient(*new QKnxNetIpTunnelClientPrivate(localAddress, localPort, layer), parent)
{}

QKnxAddress QKnxNetIpTunnelClient::individualAddress() const
{
    return d_func()->m_individualAddress;
}

void QKnxNetIpTunnelClient::setIndividualAddress(const QKnxAddress &address)
{
    Q_ASSERT_X(false, "QKnxNetIpTunnelClient::setIndividualAddress", "Setting the individual "
        "address used for tunnel connections not implemented yet.");

    Q_UNUSED(address) // TODO: Maybe implement 03_08_04 Tunnelling v01.05.03 AS.pdf, paragraph 3.2
}

void QKnxNetIpTunnelClient::sendTunnelingFrame(const QKnxTunnelingFrame &frame)
{
    if (state() != State::Connected)
        return;

    Q_D(QKnxNetIpTunnelClient);
    if (d->m_layer == QKnxNetIp::TunnelingLayer::Busmonitor)
        return; // 03_08_04 Tunnelling v01.05.03, paragraph 2.4

    d->sendTunnelingRequest(frame);
}

QT_END_NAMESPACE
