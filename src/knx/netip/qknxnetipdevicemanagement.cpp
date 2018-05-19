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
#include "qknxnetipdevicemanagement.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceManagement

    \inmodule QtKnx
    \ingroup qtknx-device-management

    \brief The QKnxNetIpDeviceManagement class enables the opening and
    handling of a device management connection from a KNXnet/IP client
    to a KNXnet/IP server.

    A device management connection is established from a KNXnet/IP client to
    a KNXnet/IP server endpoint to access management functionalities on the
    server. The IP address of the client must be set. The client uses the
    connection to send device configuration request frames to the server.

    The following code sample illustrates how to connect to the server, request
    a data connection, and use the connection:

    \code
        QKnxNetIpDeviceManagement connection;
        QHostAddress clientLocalAddress = ...
        connection.setLocalAddress(clientLocalAddress);

        QHostAddress knxNetIpServerAddress = ...
        quint16 knxNetIpServerDataEndPointPort = ...
        connection.connectToHost(knxNetIpServerAddress, knxNetIpServerDataEndPointPort);

        QKnxDeviceManagementFrame frame = ...
        connection.sendFrame(frame);
    \endcode

    \sa QKnxDeviceManagementFrame, {Qt KNX Device Management Classes}
*/

/*!
    \fn void QKnxNetIpDeviceManagement::frameReceived(QKnxDeviceManagementFrame frame)

    This signal is emitted when the KNXnet/IP client receives data (with the
    link layer frame \a frame as payload) from the KNXnet/IP server.
*/

class QKnxNetIpDeviceManagementPrivate : public QKnxNetIpEndpointConnectionPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpDeviceManagement)

public:
    QKnxNetIpDeviceManagementPrivate(const QHostAddress &address, quint16 port)
        : QKnxNetIpEndpointConnectionPrivate(address, port,
            QKnxNetIpCri(QKnxNetIp::ConnectionType::DeviceManagement), 3,
            QKnxNetIp::DeviceConfigurationRequestTimeout)
    {}

    void process(const QKnxDeviceManagementFrame &frame) override
    {
        Q_Q(QKnxNetIpDeviceManagement);
        emit q->frameReceived(frame);
    }
};

/*!
    Creates a device management connection with the parent \a parent.
*/
QKnxNetIpDeviceManagement::QKnxNetIpDeviceManagement(QObject *parent)
    : QKnxNetIpDeviceManagement({ QHostAddress::LocalHost }, 0, parent)
{}

/*!
    Creates a device management connection with the KNXnet/IP client address
    \a localAddress and parent \a parent.
*/
QKnxNetIpDeviceManagement::QKnxNetIpDeviceManagement(const QHostAddress &localAddress,
        QObject *parent)
    : QKnxNetIpDeviceManagement(localAddress, 0, parent)
{}

/*!
    Creates a device management connection with the KNXnet/IP client address
    \a localAddress, port number \a localPort, and parent \a parent.
*/
QKnxNetIpDeviceManagement::QKnxNetIpDeviceManagement(const QHostAddress &localAddress,
        quint16 localPort, QObject *parent)
    : QKnxNetIpEndpointConnection(*new QKnxNetIpDeviceManagementPrivate(localAddress, localPort), parent)
{}

/*!
    Inserts the device management frame \a frame into a request that is sent to
    a KNXnet/IP server.

   If no connection is currently established, returns \c false and does not
   send the frame.

   \sa QKnxNetIpEndpointConnection::State
*/
bool QKnxNetIpDeviceManagement::sendFrame(const QKnxDeviceManagementFrame &frame)
{
    if (state() != State::Connected)
        return false;

    Q_D(QKnxNetIpDeviceManagement);
    return d->sendDeviceConfigurationRequest(frame);
}

QT_END_NAMESPACE
