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

#include "qknxnetipendpointconnection_p.h"
#include "qknxnetipdeviceconfigurationrequest.h"
#include "qknxnetipdevicemanagementconnection.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceManagementConnection

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceManagementConnection class enables the opening and
    handling of a client connection to a KNXnet/IP server.

    The QKnxNetIpDeviceManagementConnection is a data connection between a
    client and a KNXnet/IP server endpoint. This is used by the client side to
    access management functionalities on the KNXnet/IP server. The IP address
    of the client must be set. It is then possible to connect to a chosen

    KNXnet/IP server and to send \l QKnxLocalDeviceManagementFrame frames
    to the KNXnet/IP server.

    The class takes care of connecting to the server, asking for a data
    connection, and monitoring the connection.

    \code
        QKnxNetIpDeviceManagementConnection connection;
        QHostAddress clientLocalAddress = ...
        connection.setLocalAddress(clientLocalAddress);

        QHostAddress knxNetIpServerAddress = ...
        quint16 knxNetIpServerDataEndPointPort = ...
        connection.connectToHost(knxNetIpServerAddress, knxNetIpServerDataEndPointPort);

        QKnxLocalDeviceManagementFrame frame = ...
        connection.sendTunnelFrame(frame);
    \endcode
*/

class QKnxNetIpDeviceManagementConnectionPrivate : public QKnxNetIpEndpointConnectionPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpDeviceManagementConnection)

public:
    QKnxNetIpDeviceManagementConnectionPrivate(const QHostAddress &address, quint16 port)
        : QKnxNetIpEndpointConnectionPrivate(address, port,
            QKnxNetIpCri(QKnxNetIp::ConnectionType::DeviceManagement), 3,
            QKnxNetIp::DeviceConfigurationRequestTimeout)
    {}

    void process(const QKnxCemiFrame &frame) override
    {
        Q_Q(QKnxNetIpDeviceManagementConnection);
        emit q->receivedDeviceManagementFrame(frame);
    }
};

QKnxNetIpDeviceManagementConnection::QKnxNetIpDeviceManagementConnection(QObject *parent)
    : QKnxNetIpDeviceManagementConnection({ QHostAddress::LocalHost }, 0, parent)
{}

QKnxNetIpDeviceManagementConnection::QKnxNetIpDeviceManagementConnection(const QHostAddress &addr,
        QObject *o)
    : QKnxNetIpDeviceManagementConnection(addr, 0, o)
{}

QKnxNetIpDeviceManagementConnection::QKnxNetIpDeviceManagementConnection(const QHostAddress &addr,
        quint16 port, QObject *obj)
    : QKnxNetIpEndpointConnection(*new QKnxNetIpDeviceManagementConnectionPrivate(addr, port), obj)
{}

bool QKnxNetIpDeviceManagementConnection::sendDeviceManagementFrame(const QKnxLocalDeviceManagementFrame &frame)
{
    if (state() != State::Connected)
        return false;

    Q_D(QKnxNetIpDeviceManagementConnection);
    return d->sendDeviceConfigurationRequest(frame);
}

QT_END_NAMESPACE
