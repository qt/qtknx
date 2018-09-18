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

#ifndef QKNXNETIPROUTER_H
#define QKNXNETIPROUTER_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxlinklayerframe.h>
#include <QtKnx/qtknxglobal.h>

#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkinterface.h>
#include <QtNetwork/qudpsocket.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpRouterPrivate;
class Q_KNX_EXPORT QKnxNetIpRouter : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpRouter)
    Q_DECLARE_PRIVATE(QKnxNetIpRouter)

public:
    using KnxAddressWhitelist = QSet<QKnxAddress>;

    enum class State : quint8
    {
        NotInit,
        Routing,
        NeighborBusy,
        Stop,
        Failure
    };
    Q_ENUM(State)
    QKnxNetIpRouter::State state() const;

    enum class Error : quint8
    {
        None,
        KnxRouting,
        Network
    };
    Q_ENUM(Error)

    QString errorString() const;
    QKnxNetIpRouter::Error error() const;

    enum class FilterAction : quint8
    {
        RouteDecremented,
        RouteLast,
        ForwardLocally,
        IgnoreTotally,
        IgnoreAcked
    };
    Q_ENUM(FilterAction)

    enum class RoutingMode : quint8
    {
        Block,
        RouteAll,
        Filter
    };

    QKnxNetIpRouter(QObject *parent = nullptr);
    ~QKnxNetIpRouter() = default;

    RoutingMode routingMode() const;
    void setRoutingMode(RoutingMode mode);

    KnxAddressWhitelist filterTable() const;
    void setFilterTable(const KnxAddressWhitelist &table);

    QNetworkInterface interfaceAffinity() const;
    void setInterfaceAffinity(const QHostAddress &address);
    void setInterfaceAffinity(const QNetworkInterface &iface);

    QHostAddress multicastAddress() const;
    void setMulticastAddress(const QHostAddress &address);

    QKnxAddress individualAddress() const;
    void setIndividualAddress(const QKnxAddress &address);

public Q_SLOTS:
    void sendRoutingIndication(const QKnxLinkLayerFrame &linkFrame);
    void sendRoutingIndication(const QKnxNetIpFrame &frame);
    void sendRoutingBusy(const QKnxNetIpFrame &frame);
    void sendRoutingLostMessage(const QKnxNetIpFrame &frame);
    void sendRoutingSystemBroadcast(const QKnxLinkLayerFrame &linkFrame);

    void start();
    void stop();

Q_SIGNALS:
    void routingIndicationSent(QKnxNetIpFrame frame);
    void routingBusySent(QKnxNetIpFrame frame);
    void routingLostCountSent(QKnxNetIpFrame frame);
    void routingSystemBroadcastSent(QKnxNetIpFrame frame);

    void routingIndicationReceived(QKnxNetIpFrame frame, QKnxNetIpRouter::FilterAction action);
    void routingBusyReceived(QKnxNetIpFrame frame);
    void routingLostCountReceived(QKnxNetIpFrame frame);
    void routingSystemBroadcastReceived(QKnxNetIpFrame frame);

    void stateChanged(QKnxNetIpRouter::State state);
    void errorOccurred(QKnxNetIpRouter::Error error, QString errorString);
};

QT_END_NAMESPACE

#endif
