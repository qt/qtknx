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

#include "qknxnetipdeviceconfigurationrequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceConfigurationRequestProxy

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceConfigurationRequestProxy class provides the means
    to read a device configuration request from the generic \l QKnxNetIpFrame
    class and to create a KNXnet/IP frame based on the information.

    KNXnet/IP device configuration and management is based on interface object
    properties. Devices accept configuration through KNX network telegrams,
    whereas routers accept configuration through routing frames that carry
    common external message interface (cEMI) frames addressed to the individual
    address of the router.

    Each subnetwork connected to a KNXnet/IP server is represented by a control
    endpoint. KNXnet/IP clients can discover the control endpoints by using the
    discovery procedure.

    In addition, each subnetwork is represented by a configuration and
    management endpoint that enables access to device parameters, filter table,
    and so on, after a connection is established through the control endpoint.
    The host address protocol information (HPAI) of the configuration and
    management endpoint is used by KNXnet/IP devices to exchange configuration
    information with ETS.

    A KNXnet/IP client connects to a configuration and management data endpoint
    of a KNXnet/IP server to send device configuration request frames and to
    receive device configuration acknowledgment frames. A device configuration
    request is used to read and write KNXnet/IP device configuration data. When
    a server receives a configuration request, it responds with a device
    configuration acknowledgment to confirm the reception of the request.

    A device configuration request frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, and a cEMI frame with the configuration message.

    The KNXnet/IP server discards frames with unexpected sequence numbers and
    does not send a device configuration acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the device configuration
    request information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpDeviceConfigurationRequestProxy configRequest(netIpFrame);
        if (!configRequest.isValid())
            return;

        quint8 chanId = configRequest.channelId();
        quint8 seqNum = configRequest.sequenceNumber();
        auto knxLinkFrame = configRequest.cemi();
    \endcode

    \sa builder(), QKnxNetIpServerDiscoveryAgent, QKnxNetIpDeviceConfigurationAcknowledgeProxy
*/

/*!
    \fn QKnxNetIpDeviceConfigurationRequestProxy::QKnxNetIpDeviceConfigurationRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDeviceConfigurationRequestProxy::~QKnxNetIpDeviceConfigurationRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDeviceConfigurationRequestProxy::QKnxNetIpDeviceConfigurationRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the device configuration request
    information carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpDeviceConfigurationRequestProxy::QKnxNetIpDeviceConfigurationRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpDeviceConfigurationRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 12 && cemi().isValid()
        && m_frame.serviceType() == QKnxNetIp::ServiceType::DeviceConfigurationRequest;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpDeviceConfigurationRequestProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a device configuration request frame.
*/
quint8 QKnxNetIpDeviceConfigurationRequestProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns a cEMI frame that contains the configuration message.
*/
QKnxDeviceManagementFrame QKnxNetIpDeviceConfigurationRequestProxy::cemi() const
{
    return QKnxDeviceManagementFrame::fromBytes(m_frame.data(), 0, m_frame.dataSize());
}

/*!
    Returns a builder object to create a KNXnet/IP device configuration request
    frame.
*/
QKnxNetIpDeviceConfigurationRequestProxy::Builder QKnxNetIpDeviceConfigurationRequestProxy::builder()
{
    return QKnxNetIpDeviceConfigurationRequestProxy::Builder();
}


/*!
    \class QKnxNetIpDeviceConfigurationRequestProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpDeviceConfigurationRequestProxy

    \brief The QKnxNetIpDeviceConfigurationRequestProxy::Builder class provides
    the means to create a KNXnet/IP device configuration request frame.

    A KNXnet/IP client connects to a configuration and management data endpoint
    of a KNXnet/IP server to send device configuration request frames and to
    receive device configuration acknowledgment frames. A device configuration
    request is used to read and write KNXnet/IP device configuration data. When
    a server receives a configuration request, it responds with a device
    configuration acknowledgment to confirm the reception of the request.

    A device configuration request frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, and a common external message interface (cEMI) frame with the
    configuration message.

    The KNXnet/IP server discards frames with unexpected sequence numbers and
    does not send a device configuration acknowledgment upon receiving them.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a device configuration request is:

    \code
        QKnxDeviceManagementFrame cemi = // build cEMI frame
        auto netIpFrame = QKnxNetIpDeviceConfigurationRequestProxy::builder()
            .setChannelId(1)
            .setSequenceNumber(1)
            .setCemi(cemi)
            .create();
    \endcode

    If the KNXnet/IP client does not receive a device configuration
    acknowledgment within the specified timeout or the status of a received
    acknowledgment frame indicates that errors occurred, the client repeats
    the request three times and then terminates the connection by sending a
    disconnection request, \l QKnxNetIpDisconnectRequestProxy, to the server’s
    control endpoint.
*/

/*!
    Sets the ID of the communication channel between a KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpDeviceConfigurationRequestProxy::Builder &
    QKnxNetIpDeviceConfigurationRequestProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a device configuration request frame to
    \a sequenceNumber and returns a reference to the builder.
*/
QKnxNetIpDeviceConfigurationRequestProxy::Builder &
    QKnxNetIpDeviceConfigurationRequestProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the cEMI frame that contains the device configuration message to
    \a cemi and returns a reference to the builder.
*/
QKnxNetIpDeviceConfigurationRequestProxy::Builder &
    QKnxNetIpDeviceConfigurationRequestProxy::Builder::setCemi(const QKnxDeviceManagementFrame &cemi)
{
    m_cemi = cemi;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP device configuration request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpDeviceConfigurationRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DeviceConfigurationRequest, { m_channelId, m_sequenceNumber },
        m_cemi.bytes() };
}

QT_END_NAMESPACE
