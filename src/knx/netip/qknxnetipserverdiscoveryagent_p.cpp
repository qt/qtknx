/******************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "qknxnetipserverdiscoveryagent.h"
#include "qknxnetipserverdiscoveryagent_p.h"

#include <QtCore/qthread.h>

QT_BEGIN_NAMESPACE

namespace QKnxPrivate
{
    static void clearSocket(QUdpSocket **socket)
    {
        if (*socket) {
            (*socket)->disconnect();
            (*socket)->deleteLater();
            (*socket) = nullptr;
        }
    }

    static void clearTimer(QTimer **timer)
    {
        if (*timer) {
            (*timer)->stop();
            (*timer)->disconnect();
            (*timer)->deleteLater();
            (*timer) = nullptr;
        }
    }

    static QList<Adapter> adaptersForAddresses(const QList<QHostAddress> addresses)
    {
        QList<Adapter> adapters;
        auto all = QNetworkInterface::allInterfaces();
        for (const auto &iface : all) {
            const auto flags = iface.flags();
            if (!flags.testFlag(QNetworkInterface::IsUp)
                && !flags.testFlag(QNetworkInterface::IsRunning)
                && !flags.testFlag(QNetworkInterface::CanMulticast)) continue;

            const auto type = iface.type();
            if (type != QNetworkInterface::Wifi
                && type != QNetworkInterface::Loopback
                && type != QNetworkInterface::Ethernet) continue;

            const auto entries = iface.addressEntries();
            for (const auto &entry : entries) {
                const auto address = entry.ip();
                if (address.protocol() != QAbstractSocket::IPv4Protocol)
                    continue;
                if (!addresses.contains(address))
                    continue;
                adapters.append({ address, iface });
                break;
            }
        }
        return adapters;
    }
}

Discoverer::Discoverer(const QHostAddress &address, const QNetworkInterface iface, const DiscovererConfig &c)
    : QObject(nullptr)
    , m_address(address)
    , m_iface(iface)
    , m_config(c)
{}

Discoverer::~Discoverer()
{
    finish();
}

void Discoverer::start()
{
    if (m_state != QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        return;

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Starting);

    auto isIPv4 = true;
    m_address.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        emit errorOccurred(QKnxNetIpServerDiscoveryAgent::Error::NotIPv4,
            QKnxNetIpServerDiscoveryAgent::tr("Only IPv4 local address supported."));
        return finish();
    }

    m_socket = new QUdpSocket;
    connect(m_socket, &QUdpSocket::readyRead, this, &Discoverer::onReadyRead);
    connect(m_socket, &QUdpSocket::errorOccurred, this, &Discoverer::onError);

    bool bound = m_socket->bind(m_config.Multicast ? QHostAddress::AnyIPv4
                                                   : m_address,
                                m_config.Multicast ? 3671 : 0,
                                QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!bound)
        return finish();

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Running);

    if (m_config.Multicast) {
        m_socket->setMulticastInterface(m_iface);
        if (!m_socket->joinMulticastGroup(m_multicast, m_iface)) {
            emit errorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                QKnxNetIpServerDiscoveryAgent::tr("Could not join multicast group."));
            return finish();
        }
        m_socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, m_config.Ttl);
    }

    m_timer = new QTimer;
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &Discoverer::onTimeout);

    if (m_config.DiscoveryCoreV1) {
        const auto frame = QKnxNetIpSearchRequestProxy::builder()
            .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::Builder()
                .setPort(m_config.Nat ? 0 : (m_config.Multicast ? 3671 : m_socket->localPort()))
                .setHostAddress(m_config.Nat ? QHostAddress::AnyIPv4
                    : (m_config.Multicast ? m_multicast : m_address))
                .create())
            .create();

        if (frame.isValid())
            m_socket->writeDatagram(frame.bytes().toByteArray(), m_multicast, 3671);
    }

    if (m_config.DiscoveryCoreV2) {
        const auto frame = QKnxNetIpSearchRequestProxy::extendedBuilder()
            .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::Builder()
                .setPort(m_config.Nat ? 0 : (m_config.Multicast ? 3671 : m_socket->localPort()))
                .setHostAddress(m_config.Nat ? QHostAddress::AnyIPv4
                    : (m_config.Multicast ? m_multicast : m_address))
                .create())
            .setExtendedParameters(m_config.ExtendedSearchParameters)
            .create();
        if (frame.isValid())
            m_socket->writeDatagram(frame.bytes().toByteArray(), m_multicast, 3671);
    }

    m_timer->start(m_config.Timeout);
}

void Discoverer::finish()
{
    if (m_state == QKnxNetIpServerDiscoveryAgent::State::Stopping
        || m_state == QKnxNetIpServerDiscoveryAgent::State::NotRunning) {
        return;
    }

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Stopping);

    QKnxPrivate::clearTimer(&m_timer);

    if (m_socket) {
        if (m_config.Multicast && m_socket->state() == QUdpSocket::BoundState) {
            m_socket->setMulticastInterface({});
            m_socket->leaveMulticastGroup(m_multicast, m_iface);
        }
        QKnxPrivate::clearSocket(&m_socket);
    }

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::NotRunning);
}

void Discoverer::onTimeout()
{
    if (m_devices.isEmpty())
        emit timeout();
    finish();
}

void Discoverer::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        if (m_state != QKnxNetIpServerDiscoveryAgent::State::Running)
            break;

        const auto datagram = m_socket->receiveDatagram();
        const auto data = QKnxByteArray::fromByteArray(datagram.data());
        const auto header = QKnxNetIpFrameHeader::fromBytes(data, 0);
        if (!header.isValid())
            continue;

        if (header.serviceType() != QKnxNetIp::ServiceType::SearchResponse
            && header.serviceType() != QKnxNetIp::ServiceType::ExtendedSearchResponse) {
            continue;
        }

        const auto frame = QKnxNetIpFrame::fromBytes(data);
        const auto proxy = QKnxNetIpSearchResponseProxy(frame);
        if (!proxy.isValid())
            continue;

        if (m_config.DiscoveryCoreV1 && !proxy.isExtended()) {
            emit deviceDiscovered(
                {
                    (m_config.Nat ? QKnxNetIpHpaiProxy::builder()
                                        .setHostAddress(datagram.senderAddress())
                                        .setPort(datagram.senderPort()).create()
                                  : proxy.controlEndpoint()),
                    proxy.deviceHardware(),
                    proxy.supportedFamilies(),
                    m_address, m_iface
                }
            );
        }

        if (m_config.DiscoveryCoreV2 && proxy.isExtended()) {
            const auto optionalDibs = proxy.optionalDibs();
            emit deviceDiscovered(
                {
                    (m_config.Nat ? QKnxNetIpHpaiProxy::builder()
                                        .setHostAddress(datagram.senderAddress())
                                        .setPort(datagram.senderPort()).create()
                                  : proxy.controlEndpoint()),
                    proxy.deviceHardware(),
                    proxy.supportedFamilies(),
                    m_address, m_iface,
                    [&optionalDibs]() -> QKnxNetIpDib {
                        for (const auto &dib : qAsConst(optionalDibs)) {
                            if (dib.code() == QKnxNetIp::DescriptionType::TunnelingInfo)
                                return dib;
                        }
                        return {};
                    }(),
                    [&optionalDibs]() -> QKnxNetIpDib {
                        for (const auto &dib : qAsConst(optionalDibs)) {
                            if (dib.code() == QKnxNetIp::DescriptionType::ExtendedDeviceInfo)
                                return dib;
                        }
                        return {};
                    }()
                }
            );
        }
    }
}

void Discoverer::onError(QUdpSocket::SocketError /* error */)
{
    emit errorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
        m_socket ? m_socket->errorString()
                 : QKnxNetIpServerDiscoveryAgent::tr("Unknown socket error happened.")
    );
    finish();
}

void Discoverer::setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State state)
{
    m_state = state;
    emit stateChanged(m_state);

    if (m_state == QKnxNetIpServerDiscoveryAgent::State::Starting)
        emit started();
    if (m_state == QKnxNetIpServerDiscoveryAgent::State::Stopping)
        emit finished(this);
}

QKnxNetIpServerDiscoveryAgentPrivate::QKnxNetIpServerDiscoveryAgentPrivate(const QHostAddress &addr,
        quint16 prt)
    : port(prt)
    , address(addr)
{}

void QKnxNetIpServerDiscoveryAgentPrivate::setupSocket()
{
    usedPort = port;
    usedAddress = address;
    QKnxPrivate::clearSocket(&socket);

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    socket = new QUdpSocket(q);

    adapter = QKnxPrivate::adaptersForAddresses(QList<QHostAddress> { address }).value(0);
    QObject::connect(socket, &QUdpSocket::stateChanged, q, [&](QUdpSocket::SocketState s) {
        Q_Q(QKnxNetIpServerDiscoveryAgent);
        switch (s) {
        case QUdpSocket::BoundState:
            setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Running);
            socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);

            if (type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast) {
                QNetworkInterface mni;
                const auto interfaces = QNetworkInterface::allInterfaces();
                for (const auto &iface : interfaces) {
                    if (!iface.flags().testFlag(QNetworkInterface::CanMulticast))
                        continue;

                    const auto entries = iface.addressEntries();
                    for (const auto &entry : entries) {
                        auto ip = entry.ip();
                        if (ip.protocol() != QAbstractSocket::NetworkLayerProtocol::IPv4Protocol)
                            continue;
                        if (ip != address)
                            continue;
                        mni = iface;
                        break;
                    }
                }

                if (mni.isValid())
                    socket->setMulticastInterface(mni);

                if (socket->joinMulticastGroup(multicastAddress, mni)) {
                    usedPort = multicastPort;
                    usedAddress = multicastAddress;
                } else {
                    setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                        QKnxNetIpServerDiscoveryAgent::tr("Could not join multicast group."));
                    q->stop();
                }
            } else {
                usedPort = socket->localPort();
                usedAddress = socket->localAddress();
            }

            if (q->state() == QKnxNetIpServerDiscoveryAgent::State::Running) {
                servers.clear();

                const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(mode);
                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)) {
                    auto frame = QKnxNetIpSearchRequestProxy::builder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        ).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }

                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2)) {
                    auto frame = QKnxNetIpSearchRequestProxy::extendedBuilder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        )
                        .setExtendedParameters(srps).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }

                setupAndStartReceiveTimer();
                setupAndStartFrequencyTimer();
            }
            break;
        default:
            break;
        }
    });

    QObject::connect(socket, &QUdpSocket::errorOccurred, q, [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                socket->errorString());

            Q_Q(QKnxNetIpServerDiscoveryAgent);
            q->stop();
    });

    QObject::connect(socket, &QUdpSocket::readyRead, q, [&]() {
        Q_Q(QKnxNetIpServerDiscoveryAgent);
        while (socket->hasPendingDatagrams()) {
            if (q->state() != QKnxNetIpServerDiscoveryAgent::State::Running)
                break;

            auto datagram = socket->receiveDatagram();
            auto data = QKnxByteArray::fromByteArray(datagram.data());
            const auto header = QKnxNetIpFrameHeader::fromBytes(data, 0);
            if (!header.isValid())
                continue;

             if (header.serviceType() != QKnxNetIp::ServiceType::SearchResponse &&
                 header.serviceType() != QKnxNetIp::ServiceType::ExtendedSearchResponse) {
                    continue;
             }

            auto frame = QKnxNetIpFrame::fromBytes(data);
            auto response = QKnxNetIpSearchResponseProxy(frame);
            if (!response.isValid())
                continue;

            const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(mode);
            if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)
                && !response.isExtended()) {
                    setAndEmitDeviceDiscovered({
                        (nat ? QKnxNetIpHpaiProxy::builder()
                                    .setHostAddress(datagram.senderAddress())
                                    .setPort(datagram.senderPort()).create()
                            : response.controlEndpoint()
                        ), response.deviceHardware(), response.supportedFamilies(),
                        adapter.address, adapter.iface
                    });
            }

            if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2)
                && response.isExtended()) {
                    const auto optionalDibs = response.optionalDibs();
                    setAndEmitDeviceDiscovered({
                        (nat ? QKnxNetIpHpaiProxy::builder()
                                    .setHostAddress(datagram.senderAddress())
                                    .setPort(datagram.senderPort()).create()
                            : response.controlEndpoint()
                        ), response.deviceHardware(), response.supportedFamilies(),
                        adapter.address, adapter.iface,
                           [&optionalDibs]() -> QKnxNetIpDib {
                                for (const auto &dib : qAsConst(optionalDibs)) {
                                    if (dib.code() == QKnxNetIp::DescriptionType::TunnelingInfo)
                                        return dib;
                                }
                                return {};
                            }(),
                            [&optionalDibs]() -> QKnxNetIpDib {
                                for (const auto &dib : qAsConst(optionalDibs)) {
                                    if (dib.code() == QKnxNetIp::DescriptionType::ExtendedDeviceInfo)
                                        return dib;
                                }
                                return {};
                            }()
                    });
            }
        }
    });
}

void QKnxNetIpServerDiscoveryAgentPrivate::setupAndStartReceiveTimer()
{
    Q_Q(QKnxNetIpServerDiscoveryAgent);

    QKnxPrivate::clearTimer(&receiveTimer);
    if (timeout >= 0) {
        receiveTimer = new QTimer(q);
        receiveTimer->setSingleShot(true);
        receiveTimer->start(timeout);
        QObject::connect(receiveTimer, &QTimer::timeout, q, &QKnxNetIpServerDiscoveryAgent::stop);
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::setupAndStartFrequencyTimer()
{
    Q_Q(QKnxNetIpServerDiscoveryAgent);

    QKnxPrivate::clearTimer(&frequencyTimer);
    if (frequency > 0) {
        frequencyTimer = new QTimer(q);
        frequencyTimer->setSingleShot(false);
        frequencyTimer->start(60000 / frequency);

        QObject::connect(frequencyTimer, &QTimer::timeout, q, [&]() {
            Q_Q(QKnxNetIpServerDiscoveryAgent);
            if (q->state() == QKnxNetIpServerDiscoveryAgent::State::Running) {
                servers.clear();

                const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(mode);
                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)) {
                    auto frame = QKnxNetIpSearchRequestProxy::builder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        ).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }

                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2)) {
                    auto frame = QKnxNetIpSearchRequestProxy::extendedBuilder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        )
                        .setExtendedParameters(srps).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }
            }
        });
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::setAndEmitStateChanged(
                                                     QKnxNetIpServerDiscoveryAgent::State newState)
{
    state = newState;

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    emit q->stateChanged(newState);

    if (state == QKnxNetIpServerDiscoveryAgent::State::Running)
        emit q->started();
    else if (state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        emit q->finished();
}

void QKnxNetIpServerDiscoveryAgentPrivate::setAndEmitDeviceDiscovered(
                                                 const QKnxNetIpServerInfo &discoveryInfo)
{
    servers.append(discoveryInfo);

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    emit q->deviceDiscovered(discoveryInfo);
}

void QKnxNetIpServerDiscoveryAgentPrivate::setAndEmitErrorOccurred(
                             QKnxNetIpServerDiscoveryAgent::Error newError, const QString &message)
{
    error = newError;
    errorString = message;

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    emit q->errorOccurred(error, errorString);
}

void QKnxNetIpServerDiscoveryAgentPrivate::start()
{
    if (state != QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        return;

    if (address.isNull())
        address = QHostAddress(QHostAddress::AnyIPv4);

    auto isIPv4 = true;
    address.toIPv4Address(&isIPv4);
    if (isIPv4) {
        setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Starting);
        setupSocket();
        if (type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast) {
            socket->bind(QHostAddress::AnyIPv4, multicastPort, QUdpSocket::ShareAddress
                | QAbstractSocket::ReuseAddressHint);
        } else {
            socket->bind(address, port);
        }
    } else {
        setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::NotIPv4,
            QKnxNetIpServerDiscoveryAgent::tr("Only IPv4 local address supported."));
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::start(const QList<Adapter> &adapters)
{
    if (state != QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        return;

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Starting);

    if (adapters.isEmpty())
        return stop();

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    const QFlags<QKnxNetIpServerDiscoveryAgent::ResponseType> rtype(type);
    const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> dmode(mode);
    const DiscovererConfig config {
        ttl,
        timeout,
        nat,
        rtype.testFlag(QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast),
        dmode.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1),
        dmode.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2),
        srps
    };

    bool connectError = (adapters.count() == 1);
    for (const auto &adapter : adapters) {
        auto discoverer = new Discoverer(adapter.address, adapter.iface, config);
        discoveries.append(discoverer);

        auto thread = new QThread;
        discoverer->moveToThread(thread);
        QObject::connect(thread, &QThread::started, discoverer, &Discoverer::start);
        QObject::connect(thread, &QThread::finished, discoverer, &Discoverer::deleteLater);

        QObject::connect(thread, &QThread::started, discoverer, &Discoverer::start);
        QObject::connect(thread, &QThread::finished, discoverer, &Discoverer::finish);

        QObject::connect(discoverer, &Discoverer::finished, q, [&](Discoverer *ds) {
            discoveries.removeAll(ds);
            if (discoveries.isEmpty())
                stop();
        });
        QObject::connect(discoverer, &Discoverer::finished, thread, &QThread::quit);
        QObject::connect(discoverer, &Discoverer::deviceDiscovered, q, [&](const QKnxNetIpServerInfo &server) {
            setAndEmitDeviceDiscovered(server);
        });
        if (connectError) {
            QObject::connect(discoverer, &Discoverer::errorOccurred, q,
                [&](QKnxNetIpServerDiscoveryAgent::Error error, const QString &errorString) {
                setAndEmitErrorOccurred(error, errorString);
            });
        }
        thread->start();
    }

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Running);
}

void QKnxNetIpServerDiscoveryAgentPrivate::start(const QList<QHostAddress> addresses)
{
    start(QKnxPrivate::adaptersForAddresses(addresses));
}

void QKnxNetIpServerDiscoveryAgentPrivate::start(QKnxNetIpServerDiscoveryAgent::InterfaceTypes types)
{
    QList<Adapter> adapters;
    auto all = QNetworkInterface::allInterfaces();
    for (const auto &iface : all) {
        const auto flags = iface.flags();
        if (!flags.testFlag(QNetworkInterface::IsUp)
            && !flags.testFlag(QNetworkInterface::IsRunning)
            && !flags.testFlag(QNetworkInterface::CanMulticast)) continue;

        const bool wifi = types.testFlag(QKnxNetIpServerDiscoveryAgent::InterfaceType::Wifi);
        const bool loopback = types.testFlag(QKnxNetIpServerDiscoveryAgent::InterfaceType::Loopback);
        const bool ethernet = types.testFlag(QKnxNetIpServerDiscoveryAgent::InterfaceType::Ethernet);

        const auto type = iface.type();
        if (!(wifi && type == QNetworkInterface::Wifi)
            && !(loopback && type == QNetworkInterface::Loopback)
            && !(ethernet && type == QNetworkInterface::Ethernet)) continue;

        const auto entries = iface.addressEntries();
        for (const auto &entry : entries) {
            const auto address = entry.ip();
            if (address.protocol() != QAbstractSocket::IPv4Protocol)
                continue;
            adapters.append({ address, iface });
            break;
        }
    }
    start(adapters);
}

void QKnxNetIpServerDiscoveryAgentPrivate::stop()
{
    if (state == QKnxNetIpServerDiscoveryAgent::State::Stopping
            || state == QKnxNetIpServerDiscoveryAgent::State::NotRunning) {
        return;
    }

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Stopping);

    if (socket) {
        if (type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast
            && socket->state() == QUdpSocket::BoundState) {
            socket->leaveMulticastGroup(multicastAddress);
        }
        socket->close();
    }

    QKnxPrivate::clearSocket(&(socket));
    QKnxPrivate::clearTimer(&(receiveTimer));
    QKnxPrivate::clearTimer(&(frequencyTimer));

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::NotRunning);
}

QT_END_NAMESPACE
