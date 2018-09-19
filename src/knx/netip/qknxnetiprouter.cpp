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

#include "qknxnetiprouter.h"
#include "qknxnetiprouter_p.h"
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
    \class QKnxNetIpRouter

    \inmodule QtKnx
    \ingroup qtknx-routing
    \ingroup qtknx-netip
    \since 5.12

    \brief The QKnxNetIpRouter class enables sending and receiving
    routing KNXnet/IP packets to and from other KNXnet/IP routers.

    The QKnxNetIpRouter is bound to a physical network interface
    which is used for transmitting and receiving the routing frames. It also
    requires the multicast address used by the KNX installation.

    The following code sample illustrates how to use the
    QKnxNetIpRouter to send and receive KNXnet/IP frames:

    \code
        QKnxNetIpRouter router1;
        QKnxNetIpRouter router2;
        router1.setInterfaceAffinity(QNetworkInterface::interfaceFromName("eth0"));
        router1.setMulticastAddress(QHostAddress("224.0.23.32"));
        router1.start();

        auto busyWaitTime = ...
        auto busyControlField = ...

        // Sending a routing indication to another router on the same IP network
        auto routingBusyFrame = QKnxNetIpRoutingBusyProxy::builder()
            .setDeviceState(QKnxNetIp::DeviceState::IpFault)
            .setRoutingBusyWaitTime(busyWaitTime)
            .setRoutingBusyControl(busyControlField)
            .create();
        router1.sendRoutingBusy(routingBusyFrame);

        // Processing routing indications received
        QObject::connect(&router1,
                         &QKnxNetIpRouter::routingIndicationReceived,
                     [](QKnxNetIpFrame frame,
                        QKnxNetIpRouter::FilterAction routingAction) {
                QKnxNetIpRoutingIndicationProxy indication(frame);
                qInfo().noquote() << "Received routing indication:"
                                  << indication.isValid();

                switch (routingAction) {
                case QKnxNetIpRouter::FilterAction::RouteDecremented:
                    auto cemi = indication.cemi();
                    auto extCtrl = cemi.extendedControlField();
                    count = extCtrl.hopCount();
                    // decrement and send to other subnet
                    extCtrl.setHopCount(--count);
                    auto newIndication =  QKnxNetIpRoutingIndicationProxy::builder()
                                            .setCemi(cemi)
                                            .create();
                    router2.sendRoutingIndication(newIndication)
                    // ....
                case QKnxNetIpRouter::FilterAction::RouteLast:
                case QKnxNetIpRouter::FilterAction::ForwardLocally:
                case QKnxNetIpRouter::FilterAction::IgnoreTotally:
                case QKnxNetIpRouter::FilterAction::IgnoreAcked:
                //....
                default:
                    break;
                }
                //....
        });

    \endcode

    \sa QKnxLinkLayerFrame, Routing
*/

/*!
    \typedef QKnxNetIpRouter::KnxAddressWhitelist

    A synonym for QKnxNetIpRouter::QSet<QKnxAddress>, which is the type used
    to store the filter table of the router. The filter table is interpreted
    as a whitelist by the routing algorithm. The addresses stored are allowed
    to be routed. Any frame with a destination address that is not contained
    in the table is blocked.
*/

/*!
    \enum QKnxNetIpRouter::State

    This enum holds the state of the QKnxNetIpRouter.

    \value NotInit
            Router not started yet.
    \value Routing
            Router is listening for incoming messages and accepting messages
            for sending.
    \value NeighborBusy
            A KNX router in the same network sent a busy message or the
            QKnxNetIpRouter itself detected a busy situation.
    \value Stop
            Router has been explicitly stopped by the user.
    \value Failure
            Any error that occurs in the router shall set this state.
*/

/*!
    \enum QKnxNetIpRouter::Error

    This enum holds the state of the QKnxNetIpRouter.

    \value None
            No errors.
    \value KnxRouting
            Error found at the KNX routing protocol level.
    \value Network
            UDP/IP network error.
*/

/*!
    \enum QKnxNetIpRouter::FilterAction

    This enum holds the possible courses of action of a router in response to a
    received layer service data unit (LSDU). An LSDU is a type of frame used by
    the L_Data service belonging to the data link layer.

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
    \enum QKnxNetIpRouter::RoutingMode

    This enum holds the possible parameterization actions that a router shall
    apply on telegrams of a point-to-multipoint connectionless communication
    mode with standard group addresses.

    \value Block
            No telegrams are allowed to be forwarded on the interface.
    \value RouteAll
            All telegrams are routed and the filter table is ignored.
    \value Filter
            All telegrams with destination addresses not in the filter table are
            blocked, the rest of telegrams are forwarded.
*/

/*!
    \fn void QKnxNetIpRouter::routingIndicationReceived(QKnxNetIpFrame frame, QKnxNetIpRouter::FilterAction routingAction)

    This signal is emitted when the KNXnet/IP router receives a routing indication
    \a frame and specifies the action \a routingAction to be applied by the router.
*/

/*!
    \fn void QKnxNetIpRouter::routingBusyReceived(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router receives a routing busy
    \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::routingLostCountReceived(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router receives a routing lost
    count message held in \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::routingIndicationSent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing indication \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::routingBusySent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing busy \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::routingLostCountSent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing lost count \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::routingSystemBroadcastSent(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router has finished sending the
    routing system broadcast \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::routingSystemBroadcastReceived(QKnxNetIpFrame frame)

    This signal is emitted when the KNXnet/IP router receives the routing system
    broadcast \a frame.
*/

/*!
    \fn void QKnxNetIpRouter::stateChanged(QKnxNetIpRouter::State state)

    This signal is emitted when the KNXnet/IP router transitions to a different
    \a state.
*/

/*!
    \fn void QKnxNetIpRouter::errorOccurred(QKnxNetIpRouter::Error error, QString errorString)

    This signal is emitted when the KNXnet/IP router encounters the error \a
    error that is described by \a errorString.
*/

/*!
    Returns the current state of the KNX router.
*/
QKnxNetIpRouter::State QKnxNetIpRouter::state() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_state;
}

/*!
    Returns a QString describing the latest known error that occurred in the
    KNX router.
*/
QString QKnxNetIpRouter::errorString() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_errorMessage;
}

/*!
    Returns the filter table used by the routing algorithm.
*/
QKnxNetIpRouter::KnxAddressWhitelist QKnxNetIpRouter::filterTable() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_filterTable;
}

/*!
    Sets the filter table used by the routing algorithm to \a table.
 */
void QKnxNetIpRouter::setFilterTable(const QKnxNetIpRouter::KnxAddressWhitelist &table)
{
    Q_D(QKnxNetIpRouter);
    d->m_filterTable = table;
}

/*!
    Returns the routing mode.
*/
QKnxNetIpRouter::RoutingMode QKnxNetIpRouter::routingMode() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_routingMode;
}

/*!
    Sets the routing mode to \a mode.
 */
void QKnxNetIpRouter::setRoutingMode(QKnxNetIpRouter::RoutingMode mode)
{
    Q_D(QKnxNetIpRouter);
    d->m_routingMode = mode;
}

/*!
    Returns a routing error code that describes the last error to occur in the
    KNX router.
*/
QKnxNetIpRouter::Error QKnxNetIpRouter::error() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_error;
}

/*!
    Constructs a KNXnet/IP router with the parent \a parent.
*/
QKnxNetIpRouter::QKnxNetIpRouter(QObject *parent)
    : QObject(*new QKnxNetIpRouterPrivate, parent)
{}

/*!
    Returns the current network interface used by this KNX routing instance.
 */
QNetworkInterface QKnxNetIpRouter::interfaceAffinity() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_iface;
}

/*!
    Searches for the network interface that is bound to \a address that the
    QKnxNetIpRouter instance shall use.
 */
void QKnxNetIpRouter::setInterfaceAffinity(const QHostAddress &address)
{
    Q_D(QKnxNetIpRouter);

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
void QKnxNetIpRouter::setInterfaceAffinity(const QNetworkInterface &iface)
{
    Q_D(QKnxNetIpRouter);
    if (iface.flags().testFlag(QNetworkInterface::IsRunning))
        d->m_iface = iface;
    else
        d->errorOccurred(Error::Network, tr("Could not set an affinity to any interface"));
}

/*!
    Returns the multicast address used by the QKnxNetIpRouter.
 */
QHostAddress QKnxNetIpRouter::multicastAddress() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_multicastAddress;
}

/*!
    Sets the multicast address to \a address.
 */
void QKnxNetIpRouter::setMulticastAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpRouter);

    auto isIPv4 = false;
    address.toIPv4Address(&isIPv4);
    if (!isIPv4 || !address.isMulticast())
        return;

    d->m_multicastAddress = address;
}

/*!
    Returns the routing interface's individual address.
*/
QKnxAddress QKnxNetIpRouter::individualAddress() const
{
    Q_D(const QKnxNetIpRouter);
    return d->m_individualAddress;
}

/*!
    Sets the routing interface's individual address to \a address.
*/
void QKnxNetIpRouter::setIndividualAddress(const QKnxAddress &address)
{
    Q_D(QKnxNetIpRouter);
    if (!address.isCouplerOrRouter()) {
        d->errorOccurred(QKnxNetIpRouter::Error::KnxRouting, tr("Could not set "
            "individual address."));
    } else {
        d->m_individualAddress = address;
    }
}

/*!
    Multicasts the routing indication \a frame through the network interface
    associated with the QKnxNetIpRouter.
 */
void QKnxNetIpRouter::sendRoutingIndication(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRouter);

    if (d->m_state != QKnxNetIpRouter::State::Routing)
        return;

    QKnxNetIpRoutingIndicationProxy indication(frame);
    if (!indication.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRouter::Error::KnxRouting, tr("Could not send routing "
            "indication."));
    } else {
        emit routingIndicationSent(frame);
    }
}

/*!
    Multicasts the routing busy message containing \a frame through the
    network interface associated with the QKnxNetIpRouter.
*/
void QKnxNetIpRouter::sendRoutingBusy(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRouter);

    if (d->m_state != QKnxNetIpRouter::State::Routing)
        return;

    QKnxNetIpRoutingBusyProxy routingBusy(frame);
    if (!routingBusy.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRouter::Error::KnxRouting, tr("Could not send routing "
            "busy."));
    } else {
        emit routingBusySent(frame);
    }
}

/*!
    Multicasts the routing lost message \a frame through the network interface
    associated with the QKnxNetIpRouter.
*/
void QKnxNetIpRouter::sendRoutingLostMessage(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRouter);

    if (d->m_state != QKnxNetIpRouter::State::Routing)
        return;

    QKnxNetIpRoutingLostMessageProxy lostMessageFrame(frame);
    if (!lostMessageFrame.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRouter::Error::KnxRouting, tr("Could not send routing "
            "lost count."));
    } else {
        emit routingLostCountSent(frame);
    }
}

/*!
    Multicasts the routing system broadcast \a frame through the network
    interface associated with the QKnxNetIpRouter.
*/
void QKnxNetIpRouter::sendRoutingSystemBroadcast(const QKnxNetIpFrame &frame)
{
    Q_D(QKnxNetIpRouter);

    if (d->m_state != QKnxNetIpRouter::State::Routing)
        return;

    QKnxNetIpRoutingSystemBroadcastProxy proxy(frame);
    if (!proxy.isValid())
        return;

    if (!d->sendFrame(frame)) {
        d->errorOccurred(QKnxNetIpRouter::Error::KnxRouting, tr("Could not send routing "
            "system broadcast."));
    } else {
        emit routingSystemBroadcastSent(frame);
    }
}
/*!
    Signals the QKnxNetIpRouter to start listening for messages
    received and accept sending messages.
*/
void QKnxNetIpRouter::start()
{
    Q_D(QKnxNetIpRouter);
    if (d->m_state == State::NotInit)
        d->start();
    else if (d->m_state == State::Failure || d->m_state == State::Stop)
        d->restart();
}

/*!
    Stops the QKnxNetIpRouter. No messages can be received from or
    sent to the network.
*/
void QKnxNetIpRouter::stop()
{
    Q_D(QKnxNetIpRouter);
    d->stop();
    d->changeState(QKnxNetIpRouter::State::Stop);
}

QT_END_NAMESPACE
