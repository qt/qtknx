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

#include "qknxnetiphpai.h"
#include "qknxnetipserverdiscoveryagent.h"
#include "qknxnetipserverdiscoveryagent_p.h"

#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpServerDiscoveryAgent

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpServerDiscoveryAgent class discovers KNXnet/IP servers
    by sending a search request in the network that the client is connected to.

    The agent may be set up with the \l Unicast response type to receive the
    answers from the different KNXnet/IP servers directly in a point-to-point
    manner. Typically, it should use the \l Multicast response type to ensure
    reception from KNXnet/IP servers that are on a different subnetwork.

    Here is an example on how to use this discovery agent:

    \code
        QKnxNetIpServerDiscoveryAgent agent;
        QHostAddress clientLocalAddress = ...
        agent.setLocalAddress(clientLocalAddress);
        agent.start();
    \endcode

    The discovery agent further provides the option to send extended search
    requests as specified by the KNX application note AN184. This allows the
    discovery client to include zero or more search request parameter (SRP)
    blocks to transfer additional information regarding the search. This can
    be used for example to restrict the set of devices that are expected to
    respond or to influence the type of device information blocks (DIBs) which
    the client is interested in.

    \note A combination of both \l{DiscoveryMode}{discovery modes} is permitted.

    Here is an example on how to use this discovery agent with extended search
    features to indicate that the KNXnet/IP client is interested only in the
    response from the KNXnet/IP server with the given MAC address:

    \code
        QHostAddress clientLocalAddress = ...
        auto builder = QKnxNetIpSrpProxy::macAddressBuilder();

        QKnxNetIpServerDiscoveryAgent agent(clientLocalAddress);
        agent.setDiscoveryMode(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2);
        agent.setExtendedSearchParameters({
            builder.setMac(QKnxByteArray::fromHex("bcaec56690f9")).create(),
            builder.setMac(QKnxByteArray::fromHex("4ccc6ae40000")).create()
        });
        agent.start();
    \endcode

    To retrieve the list of discovered servers:
    \code
        auto servers = agent.discoveredServers();
    \endcode

    \sa {Qt KNXnet/IP Connection Classes}
*/

/*!
    \enum QKnxNetIpServerDiscoveryAgent::State

    This enum value holds the state of the discovery agent.

    \value NotRunning
           The discovery agent is not running.
    \value Starting
           The discovery agent is starting up.
    \value Running
           The discovery agent is running.
    \value Stopping
           The discovery agent is stopping.
*/

/*!
    \enum QKnxNetIpServerDiscoveryAgent::Error

    This enum value holds the type of an error that occurred.

    \value None
           No errors occurred.
    \value Network
           A network error occurred.
    \value NotIPv4
           The network protocol used is not IPv4.
    \value Unknown
           An unknown error occurred.
*/

/*!
    \enum QKnxNetIpServerDiscoveryAgent::ResponseType

    This enum type holds the response type that the agent is set up with to
    receive search response messages.

    \value Unicast      Receive responses in a point-to-point manner.
    \value Multicast    Collect responses from multicast messages.
*/

/*!
    \enum QKnxNetIpServerDiscoveryAgent::DiscoveryMode
    \since 5.12

    This enum type holds the mode that the agent is set up with to send
    search request messages. A combination of both values is permitted.

    \value CoreV1
            The server uses search request frames as specified in KNXnet/IP
            Core Version 1 to discover KNXnet/IP devices.
    \value CoreV2
            The server uses extended search request frames as specified in
            KNXnet/IP Core Version 2 to discover KNXnet/IP devices.
*/

/*!
    \fn QKnxNetIpServerDiscoveryAgent::deviceDiscovered(QKnxNetIpServerInfo server)

    This signal is emitted when the server \a server is discovered.
*/

/*!
    \fn QKnxNetIpServerDiscoveryAgent::errorOccurred(QKnxNetIpServerDiscoveryAgent::Error error, QString errorString)

    This signal is emitted when the error \a error with the message
    \a errorString occurs.
*/

/*!
    \fn QKnxNetIpServerDiscoveryAgent::finished()

    This signal is emitted when the discovery agent has finished discovering
    servers.
*/

/*!
    \fn QKnxNetIpServerDiscoveryAgent::started()

    This signal is emitted when the discovery agent starts discovering servers.
*/

/*!
    \fn QKnxNetIpServerDiscoveryAgent::stateChanged(QKnxNetIpServerDiscoveryAgent::State state)

    This signal is emitted when the state of the discovery agent changes to
    \a state.
*/

QKnxNetIpServerDiscoveryAgentPrivate::QKnxNetIpServerDiscoveryAgentPrivate(const QHostAddress &addr,
        quint16 prt)
    : port(prt)
    , address(addr)
{}

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
}

void QKnxNetIpServerDiscoveryAgentPrivate::setupSocket()
{
    usedPort = port;
    usedAddress = address;
    QKnxPrivate::clearSocket(&socket);

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    socket = new QUdpSocket(q);

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

                const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(discoveryMode);
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

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(socket,
        static_cast<overload>(&QUdpSocket::error), q, [&](QUdpSocket::SocketError) {
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

            const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(discoveryMode);
            if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)
                && !response.isExtended()) {
                    setAndEmitDeviceDiscovered({
                        (nat ? QKnxNetIpHpaiProxy::builder()
                                    .setHostAddress(datagram.senderAddress())
                                    .setPort(datagram.senderPort()).create()
                            : response.controlEndpoint()
                        ), response.deviceHardware(), response.supportedFamilies()
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

namespace QKnxPrivate
{
    static void clearTimer(QTimer **timer)
    {
        if (*timer) {
            (*timer)->stop();
            (*timer)->disconnect();
            (*timer)->deleteLater();
            (*timer) = nullptr;
        }
    }
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

                const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(discoveryMode);
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


// -- QKnxNetIpServerDiscoveryAgent

/*!
    Creates a KNXnet/IP server discovery agent with the parent \a parent.
*/

QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(QObject *parent)
    : QKnxNetIpServerDiscoveryAgent(QHostAddress(QHostAddress::AnyIPv4), 0u, parent)
{}

/*!
    Deletes a KNXnet/IP server discovery agent.
*/
QKnxNetIpServerDiscoveryAgent::~QKnxNetIpServerDiscoveryAgent()
{
    stop();
}

/*!
    Creates a KNXnet/IP server discovery agent with the host address
    \a localAddress and the parent \a parent.
*/
QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(const QHostAddress &localAddress,
        QObject *parent)
    : QKnxNetIpServerDiscoveryAgent(localAddress, 0u, parent)
{}

/*!
    Creates a KNXnet/IP server discovery agent with the host address
    \a localAddress, the port number \a port, and the parent \a parent.

    \note If the port number is already bound by a different process, discovery
    will fail.
*/
QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(const QHostAddress &localAddress,
        quint16 port, QObject *parent)
    : QKnxNetIpServerDiscoveryAgent(*new QKnxNetIpServerDiscoveryAgentPrivate(localAddress, port),
        parent)
{}

/*!
    Returns the state of a KNXnet/IP server discovery agent.
*/
QKnxNetIpServerDiscoveryAgent::State QKnxNetIpServerDiscoveryAgent::state() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->state;
}

/*!
    Returns the type of an error that occurred when discovering servers.
*/
QKnxNetIpServerDiscoveryAgent::Error QKnxNetIpServerDiscoveryAgent::error() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->error;
}

/*!
    Returns a human-readable string that describes an error.
*/
QString QKnxNetIpServerDiscoveryAgent::errorString() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->errorString;
}

/*!
    Returns a list of servers that were discovered.
*/
QVector<QKnxNetIpServerInfo> QKnxNetIpServerDiscoveryAgent::discoveredServers() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->servers;
}

/*!
    Returns the port number used by a discovery agent.
*/
quint16 QKnxNetIpServerDiscoveryAgent::localPort() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::Running)
        return d->usedPort;
    return d->port;
}

/*!
    Sets the port number used by a discovery agent to \a port.

    \note If the port changes during discovery, the new port will not be used
    until the next run.
*/
void QKnxNetIpServerDiscoveryAgent::setLocalPort(quint16 port)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->port = port;
}

/*!
    Returns the host address of a discovery agent.
*/
QHostAddress QKnxNetIpServerDiscoveryAgent::localAddress() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::Running)
        return d->usedAddress;
    return d->address;
}

/*!
    Sets the host address of a discovery agent to \a address.

    \note If the address changes during discovery, the new address will not be
    used until the next run.
*/
void QKnxNetIpServerDiscoveryAgent::setLocalAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->address = address;
}

/*!
    Returns the timeout value used by the discovery agent to wait for incoming
    search response messages. The default value is 3000 milliseconds.

    \sa setTimeout
*/
int QKnxNetIpServerDiscoveryAgent::timeout() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->timeout;
}

/*!
    Sets the timeout for the discovery agent to \a msec. If \a msec is \c -1,
    the agent will not timeout and has to be terminated by calling the \l stop()
    function.

    \sa timeout
    \sa searchFrequency
    \sa setSearchFrequency
*/
void QKnxNetIpServerDiscoveryAgent::setTimeout(int msec)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->timeout = msec;
    d->setupAndStartReceiveTimer();
}

/*!
    Returns the frequency at which the discovery agent sends search request
    messages. The default value is \c 0.

    \sa setSearchFrequency
*/
int QKnxNetIpServerDiscoveryAgent::searchFrequency() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->frequency;
}

/*!
    Sets the frequency at which the discovery agent sends search request
    messages to \a timesPerMinute. The default value is \c 0.

    \sa timeout
    \sa setTimeout
    \sa searchFrequency
*/
void QKnxNetIpServerDiscoveryAgent::setSearchFrequency(int timesPerMinute)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->frequency = timesPerMinute;
    if (d->frequencyTimer)
        d->frequencyTimer->setInterval(60000 / timesPerMinute);
}

/*!
    Returns \c true if the server discovery agent uses network address
    translation (NAT).
*/
bool QKnxNetIpServerDiscoveryAgent::natAware() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->nat;
}

/*!
    Sets whether the server discovery agent is using NAT to \a useNat.

    \note If the setting changes during discovery, it will not be used until the
    next run.
*/
void QKnxNetIpServerDiscoveryAgent::setNatAware(bool useNat)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->nat = useNat;
}

/*!
    Returns the time to live (TTL) used for multicast search response messages.
    TTL is the maximum number of IP routers that may route the message. Each IP
    router that the message passes decrements the TTL by one. When the TTL has
    reached zero, the message is discarded.
*/
quint8 QKnxNetIpServerDiscoveryAgent::multicastTtl() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->ttl;
}

/*!
    Sets the TTL used for multicasting to \a ttl. The value \c 1 means that the
    message does not leave the local network.
*/
void QKnxNetIpServerDiscoveryAgent::setMulticastTtl(quint8 ttl)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->ttl = ttl;
    if (d->socket)
        d->socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);
}

/*!
    Returns the response type of search response messages.
*/
QKnxNetIpServerDiscoveryAgent::ResponseType QKnxNetIpServerDiscoveryAgent::responseType() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->type;
}

/*!
    Sets the response type of search response messages to \a type.
*/
void QKnxNetIpServerDiscoveryAgent::setResponseType(QKnxNetIpServerDiscoveryAgent::ResponseType type)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->type = type;
}

/*!
    \since 5.12

    Returns the search mode used to discover a KNXnet/IP server on the network
    and by default returns \l {QKnxNetIpServerDiscoveryAgent::CoreV1}.
*/
QKnxNetIpServerDiscoveryAgent::DiscoveryModes QKnxNetIpServerDiscoveryAgent::discoveryMode() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->discoveryMode;
}

/*!
    \since 5.12

    Sets the search mode used to discover a KNXnet/IP server on the network to
    \a mode. The function supports \l {QKnxNetIpServerDiscoveryAgent::CoreV1},
    \l {QKnxNetIpServerDiscoveryAgent::CoreV2}, or a combination of both.
*/
void QKnxNetIpServerDiscoveryAgent::setDiscoveryMode(QKnxNetIpServerDiscoveryAgent::DiscoveryModes mode)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->discoveryMode = mode;
}

/*!
    \since 5.12
    Returns the search request parameter (SRP) objects used in an
    \l {QKnxNetIpServerDiscoveryAgent::CoreV2} {extended search request}.
*/
QVector<QKnxNetIpSrp> QKnxNetIpServerDiscoveryAgent::extendedSearchParameters() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->srps;
}

/*!
    \since 5.12

    Sets the extended search request parameter (SRP) objects to \a srps.

    The discovery agent may include zero or more SRP objects to transfer
    additional information regarding the search (for example to restrict the
    set of KNXnet/IP servers that are expected to respond).

    \sa QKnxNetIpSrpProxy::MacAddress, QKnxNetIpSrpProxy::ProgrammingMode,
        QKnxNetIpSrpProxy::SupportedFamily, QKnxNetIpSrpProxy::RequestDibs
*/
void QKnxNetIpServerDiscoveryAgent::setExtendedSearchParameters(const QVector<QKnxNetIpSrp> &srps)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->srps = srps;
}

/*!
    Starts a server discovery agent.
*/
void QKnxNetIpServerDiscoveryAgent::start()
{
    Q_D(QKnxNetIpServerDiscoveryAgent);

    if (d->state != QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        return;

    auto isIPv4 = true;
    d->address.toIPv4Address(&isIPv4);
    if (isIPv4) {
        d->setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Starting);

        d->setupSocket();
        if (d->type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast) {
            d->socket->bind(QHostAddress::AnyIPv4, d->multicastPort, QUdpSocket::ShareAddress
                | QAbstractSocket::ReuseAddressHint);
        } else {
            d->socket->bind(d->address, d->port);
        }
    } else {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 local address supported."));
    }
}

/*!
    Starts a server discovery agent with the timeout \a timeout.

    \sa setTimeout(), timeout()
*/
void QKnxNetIpServerDiscoveryAgent::start(int timeout)
{
    d_func()->timeout = timeout;
    start();
}

/*!
    Stops a server discovery agent.
*/
void QKnxNetIpServerDiscoveryAgent::stop()
{
    Q_D(QKnxNetIpServerDiscoveryAgent);

    if (d->state == State::Stopping || d->state == State::NotRunning)
        return;

    d->setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Stopping);

    if (d->type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast
        && d->socket->state() == QUdpSocket::BoundState) {
            d->socket->leaveMulticastGroup(d->multicastAddress);
    }
    d->socket->close();

    QKnxPrivate::clearSocket(&(d->socket));
    QKnxPrivate::clearTimer(&(d->receiveTimer));
    QKnxPrivate::clearTimer(&(d->frequencyTimer));

    d->setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::NotRunning);
}

/*!
    \internal
*/
QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(QKnxNetIpServerDiscoveryAgentPrivate &dd,
        QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
