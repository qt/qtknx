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

#include "qknxnetiproutinginterface.h"
#include "qknxnetiproutinginterface_p.h"
#include "qknxnetip.h"
#include "qknxnetipframe.h"
#include "qknxnetiproutingbusy.h"
#include "qknxnetiproutingindication.h"
#include "qknxnetiproutinglostmessage.h"
#include "qknxnetiproutingsystembroadcast.h"

#include <QtCore/qrandom.h>
#include <QtCore/qtimer.h>
#include <QtNetwork/qnetworkdatagram.h>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpRoutingInterface

    \inmodule QtKnx
    \ingroup qtknx-routing
    \ingroup qtknx-netip

    \brief The QKnxNetIpRoutingInterface class enables sending and receiving
    routing KNXnet/IP packets to and from other KNXnet/IP routers.

    KNXnet/IP routing is defined as a set of KNXnet/IP routers communicating
    over a one-to-many communication relationship (multicast), in which KNX
    data shall be transferred from one device to one or more other devices
    simultaneously over an IP network. A set of KNXnet/IP routers can replace
    KNX line and backbone couplers and connected main lines, allowing usage
    of existing cabling (such as Ethernet) and faster transmission times (and
    simultaneousness) between KNX subnets. The IP network acts as a fast
    backbone that connects KNX subnets and is a high-speed replacement for
    the KNX backbone.

    The QKnxNetIpRoutingInterface is bound to a physical network interface
    which is used for transmitting and receiving the routing frames. It also
    requires the multicast address used by the KNX installation.

    The following code sample illustrates how to use the
    QKnxNetIpRoutingInterface to send and receive KNXnet/IP frames:

    \code
        QKnxNetIpRoutingInterface router;
        router.setInterfaceAffinity(QNetworkInterface::interfaceFromName("eth0"));
        router.setMulticastAddress(QHostAddress("224.0.23.32"));
        router.start();

        auto busyWaitTime = ...
        auto busyControlField = ...

        // Sending a routing indication to another router on the same IP network
        auto routingBusyFrame = QKnxNetIpRoutingBusyProxy::builder()
            .setDeviceState(QKnxNetIp::DeviceState::IpFault)
            .setRoutingBusyWaitTime(busyWaitTime)
            .setRoutingBusyControl(busyControlField)
            .create();
        router.sendRoutingBusy(routingBusyFrame);

        // Processing routing indications received
        QObject::connect(&router,
                         &QKnxNetIpRoutingInterface::routingIndicationReceived,
                     [](QKnxNetIpFrame frame) {
                QKnxNetIpRoutingIndicationProxy indication(frame);
                qInfo().noquote() << "Received routing indication:"
                                  << indication.isValid();
        });

    \endcode

    \sa QKnxLinkLayerFrame
*/

/*!
    \typedef QKnxNetIpRoutingInterface::FilterTable

    A synonym for QKnxNetIpRoutingInterface::QSet<QKnxAddress> which is the
    type used to store the filter table of the routing interface.
*/

/*!
    \enum QKnxNetIpRoutingInterface::State

    This enum holds the state of the QKnxNetIpRoutingInterface.

    \value NotInit
            Router not started yet.
    \value Routing
            Router is listening for incoming messages and accepting messages
            for sending.
    \value NeighborBusy
            A KNX router in the same network sent a busy message or the
            QKnxNetIpRoutingInterface itself detected a busy situation.
    \value Stop
            Router has been explicitly stopped by the user.
    \value Failure
            Any error that occurs in the router shall set this state.
*/

/*!
    \enum QKnxNetIpRoutingInterface::Error

    This enum holds the state of the QKnxNetIpRoutingInterface.

    \value None
            No errors.
    \value KnxRouting
            Error found at the KNX routing protocol level.
    \value Network
            UDP/IP network error.
*/

/*!
    \enum QKnxNetIpRoutingInterface::FilterAction

    This enum holds the possible courses of action of a router in response to a
    received layer service data unit (LSDU). An LSDU is a type of frame used by
    the L_Data service belonging to the data link layer.

    \value RouteUnmodified
        The LSDU shall be routed from one subnetwork (on the primary side or the
        secondary side) to the second subnetwork side (on the secondary side or
        the primary side) without modification of the hop count value. The LSDU
        shall only be routed to the second subnetwork after it has been
        positively acknowledged on the first subnetwork.
    \value RouteDecremented
        The LSDU shall be routed from one subnetwork (on the primary
        side or the secondary side) to the second subnetwork side (on the
        secondary side or the primary side) after the hop count value is
        decremented. The LSDU shall only be routed to the second subnetwork
        after it has been positively acknowledged on the first subnetwork.
    \value RouteLast
        The LSDU shall be routed from the first subnetwork to the second
        subnetwork and the hop count value shall be set to zero. The
        telegram shall be acknowledged according to the Data Link Layer
        specifications of the medium of the first subnetwork and then
        the LSDU shall be routed to the second subnetwork.
    \value ForwardLocally
        The LSDU shall be processed to an NSDU and given to the local
        network layer user after it has been acknowledged positively on
        the subnetwork from which it has arrived.
    \value IgnoreTotally
        The LSDU shall be ignored; no acknowledgment shall be sent back
        to the originator of the LSDU.
    \value IgnoreAcked
        The LSDU shall be ignored, nonetheless it shall be acknowledged
        on the subnetwork from which it has arrived.
*/

/*!
    \enum QKnxNetIpRoutingInterface::RoutingMode

    This enum holds the possible parameterization actions that a router shall
    apply on telegrams of a point-to-multipoint connectionless communication
    mode with standard group addresses.

    \value BlockRouting
            No telegrams are allowed to be forwarded on the interface.
    \value RouteAll
            All telegrams are routed and the filter table is ignored.
    \value FilterTableRouting
            All telegrams with destination addresses not in the filter table are
            blocked, the rest of telegrams are forwarded.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingIndicationReceived(QKnxNetIpFrame frame, QKnxNetIpRoutingInterface::FilterAction routingAction)

    This signal is emitted when the KNXnet/IP router receives a routing indication
    \a frame and specifies the action \a routingAction to be applied by the router.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingBusyReceived(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router receives a routing busy
    \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingLostCountReceived(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router receives a routing lost
    count message held in \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingIndicationSent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing indication \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingBusySent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing busy \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingLostCountSent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing lost count \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingSystemBroadcastSent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing system broadcast \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::routingSystemBroadcastReceived(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router receives the routing system
    broadcast \a frame.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::stateChanged(QKnxNetIpRoutingInterface::State state)

    This signal is emitted when the KNXnet/IP router transitions to a different
    \a state.
*/

/*!
    \fn void QKnxNetIpRoutingInterface::errorOccurred(QKnxNetIpRoutingInterface::Error error, QString errorString)

    This signal is emitted when the KNXnet/IP router encounters the error \a
    error that is described by \a errorString.
*/

/*!
    Returns the current state of the KNX routing interface.
*/
QKnxNetIpRoutingInterface::State QKnxNetIpRoutingInterface::state() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_state;
}

/*!
    Returns a QString describing the latest known error that occurred in the
    KNX routing interface.
*/
QString QKnxNetIpRoutingInterface::errorString() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_errorMessage;
}

/*!
    Returns the filter table used by the routing algorithm.
*/
QKnxNetIpRoutingInterface::FilterTable QKnxNetIpRoutingInterface::filterTable() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_filterTable;
}

/*!
    Sets the filter table used by the routing algorithm to \a table.
 */
void QKnxNetIpRoutingInterface::setFilterTable(const QKnxNetIpRoutingInterface::FilterTable &table)
{
    Q_D(QKnxNetIpRoutingInterface);
    d->m_filterTable = table;
    d->m_routingMode = RoutingMode::FilterTableRouting;
}

/*!
    Returns the routing mode.
*/
QKnxNetIpRoutingInterface::RoutingMode QKnxNetIpRoutingInterface::routingMode() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_routingMode;
}

/*!
    Sets the interface routing mode to \a mode.
 */
void QKnxNetIpRoutingInterface::setRoutingMode(QKnxNetIpRoutingInterface::RoutingMode mode)
{
    Q_D(QKnxNetIpRoutingInterface);
    d->m_routingMode = mode;
}

/*!
    Returns an error code that describes the last error that occurred in the
    KNX routing interface.
*/
QKnxNetIpRoutingInterface::Error QKnxNetIpRoutingInterface::error() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_error;
}

/*!
    Constructs a KNXnet/IP routing interface with the parent \a parent.
*/
QKnxNetIpRoutingInterface::QKnxNetIpRoutingInterface(QObject *parent)
    : QObject(*new QKnxNetIpRoutingInterfacePrivate, parent)
{}

/*!
    Returns the current network interface used by this KNX routing instance.
 */
QNetworkInterface QKnxNetIpRoutingInterface::interfaceAffinity() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_iface;
}

/*!
    Searches for the network interface that is bound to \a address that the
    QKnxNetIpRoutingInterface instance shall use.
 */
void QKnxNetIpRoutingInterface::setInterfaceAffinity(const QHostAddress &address)
{
    Q_D(QKnxNetIpRoutingInterface);

    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const auto &iface : interfaces) {
        if (iface.addressEntries().isEmpty())
            continue;

        auto ifaceAddress = iface.addressEntries().first().ip();
        if (address != ifaceAddress)
            continue;

        if (!iface.flags().testFlag(QNetworkInterface::IsRunning)
            || !iface.flags().testFlag(QNetworkInterface::CanMulticast)) {
            break;
        }

        d->m_iface = iface;
        return;
    }

    d->errorOccurred(Error::Network, tr("Could not set an affinity to any interface"));
}

/*!
    Sets the network interface \a iface that the QNetworkInterface instance
    shall use.
 */
void QKnxNetIpRoutingInterface::setInterfaceAffinity(const QNetworkInterface &iface)
{
    Q_D(QKnxNetIpRoutingInterface);
    if (iface.flags().testFlag(QNetworkInterface::IsRunning))
        d->m_iface = iface;
    else
        d->errorOccurred(Error::Network, tr("Could not set an affinity to any interface"));
}

/*!
    Returns the multicast address used by the QKnxNetIpRoutingInterface.
 */
QHostAddress QKnxNetIpRoutingInterface::multicastAddress() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_multicastAddress;
}

/*!
    Sets the multicast address to \a address.
 */
void QKnxNetIpRoutingInterface::setMulticastAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpRoutingInterface);

    auto isIPv4 = false;
    address.toIPv4Address(&isIPv4);
    if (!isIPv4 || !address.isMulticast())
        return;

    d->m_multicastAddress = address;
}

/*!
    Returns the routing interface's individual address.
*/
QKnxAddress QKnxNetIpRoutingInterface::individualAddress() const
{
    Q_D(const QKnxNetIpRoutingInterface);
    return d->m_individualAddress;
}

/*!
    Sets the routing interface's individual address to \a address.
*/
void QKnxNetIpRoutingInterface::setIndividualAddress(const QKnxAddress &address)
{
    Q_D(QKnxNetIpRoutingInterface);
    if (!address.isCouplerOrRouter()) {
        d->errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting, tr("Could not set "
            "individual address."));
    } else {
        d->m_individualAddress = address;
    }
}

/*!
    Multicasts the routing indication \a frame through the network interface
    associated with the QKnxNetIpRoutingInterface.
 */
void QKnxNetIpRoutingInterface::sendRoutingIndication(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRoutingInterface);

    if (d->m_state != QKnxNetIpRoutingInterface::State::Routing)
        return;

    QKnxNetIpRoutingIndicationProxy indication(frame);
    if (!indication.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting, tr("Could not send routing "
            "indication."));
    } else {
        emit routingIndicationSent(frame);
    }
}

/*!
    Multicasts the routing indication \a linkFrame through the network interface
    associated with the QKnxNetIpRoutingInterface.
 */
void QKnxNetIpRoutingInterface::sendRoutingIndication(const QKnxLinkLayerFrame &linkFrame)
{
    auto netIpFrame = QKnxNetIpRoutingIndicationProxy::builder()
        .setCemi(linkFrame)
        .create();
    sendRoutingIndication(netIpFrame);
}

/*!
    Multicasts the routing busy message containing \a frame through the
    network interface associated with the QKnxNetIpRoutingInterface.
*/
void QKnxNetIpRoutingInterface::sendRoutingBusy(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRoutingInterface);

    if (d->m_state != QKnxNetIpRoutingInterface::State::Routing)
        return;

    QKnxNetIpRoutingBusyProxy routingBusy(frame);
    if (!routingBusy.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting, tr("Could not send routing "
            "busy."));
    } else {
        emit routingBusySent(frame);
    }
}

/*!
    Multicasts the routing lost message \a frame through the network interface
    associated with the QKnxNetIpRoutingInterface.
*/
void QKnxNetIpRoutingInterface::sendRoutingLostMessage(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRoutingInterface);

    if (d->m_state != QKnxNetIpRoutingInterface::State::Routing)
        return;

    QKnxNetIpRoutingLostMessageProxy lostMessageFrame(frame);
    if (!lostMessageFrame.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting, tr("Could not send routing "
            "lost count."));
    } else {
        emit routingLostCountSent(frame);
    }
}

/*!
    Multicasts the routing system broadcast \a frame through the network interface
    associated with the QKnxNetIpRoutingInterface.
*/
void QKnxNetIpRoutingInterface::sendRoutingSystemBroadcast(const QKnxLinkLayerFrame &linkFrame)
{
    Q_D(QKnxNetIpRoutingInterface);

    auto netipFrame = QKnxNetIpRoutingSystemBroadcastProxy::builder()
        .setCemi(linkFrame)
        .create();

    QKnxNetIpRoutingSystemBroadcastProxy proxy(netipFrame);
    if (!proxy.isValid())
        return;

    if (!d->sendFrame(netipFrame)) {
        d->errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting, tr("Could not send routing "
            "system broadcast."));
    } else {
        emit routingSystemBroadcastSent(netipFrame);
    }
}

/*!
    Signals the QKnxNetIpRoutingInterface to start listening for messages
    received and accept sending messages.
*/
void QKnxNetIpRoutingInterface::start()
{
    Q_D(QKnxNetIpRoutingInterface);
    if (d->m_state == State::NotInit)
        d->start();
    else if (d->m_state == State::Failure || d->m_state == State::Stop)
        d->restart();
}

/*!
    Stops the QKnxNetIpRoutingInterface. No messages can be received and/or
    sent to the network.
*/
void QKnxNetIpRoutingInterface::stop()
{
    Q_D(QKnxNetIpRoutingInterface);
    d->stop();
    d->changeState(QKnxNetIpRoutingInterface::State::Stop);
}

QT_END_NAMESPACE
