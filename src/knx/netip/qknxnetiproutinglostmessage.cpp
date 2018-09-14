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

#include "qknxnetiproutinglostmessage.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpRoutingLostMessageProxy

    \inmodule QtKnx
    \ingroup qtknx-netip
    \ingroup qtknx-routing

    \brief The QKnxNetIpRoutingLostMessageProxy class provides the means to read
    a routing-lost message from the generic \l QKnxNetIpFrame class and to
    create a KNXnet/IP frame based on the information.

    Depending on the configuration, a KNXnet/IP router or device can receive
    more datagrams than it can forward to the KNX subnetwork. This can lead
    to an overflow of the datagram queue and subsequent loss of one or more
    KNXnet/IP telegrams, because they could not be transferred from the
    network buffer to the queue to the underlying KNX subnetwork.

    A routing-lost message is multicast by a KNXnet/IP router to notify that
    KNXnet/IP routing indication frames were lost. The message contains the
    state of the router or device (\l QKnx::NetIp::DeviceState) and the number
    of lost frames.

    \note When using QKnxNetIpRoutingLostMessageProxy, care must be taken to
    ensure that the referenced KNXnet/IP frame outlives the proxy on all code
    paths, lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the routing-lost message
    information sent by a KNXnet/IP router:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        QKnxNetIpRoutingLostMessageProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        if (proxy.deviceState() == QKnx::NetIp::DeviceState::KnxFault) {
            ....
        }
    \endcode

    \sa builder(), QKnxNetIpRoutingIndicationProxy, QKnxNetIpRoutingBusyProxy,
        {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpRoutingLostMessageProxy::QKnxNetIpRoutingLostMessageProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingLostMessageProxy::~QKnxNetIpRoutingLostMessageProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingLostMessageProxy::QKnxNetIpRoutingLostMessageProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the routing-lost message information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpRoutingLostMessageProxy::QKnxNetIpRoutingLostMessageProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the state of a KNXnet/IP router or device.
*/
QKnxNetIp::DeviceState QKnxNetIpRoutingLostMessageProxy::deviceState() const
{
    return QKnxNetIp::DeviceState(m_frame.constData().value(1));
}

/*!
    Returns the number of frames that were lost.
*/
quint16 QKnxNetIpRoutingLostMessageProxy::lostMessageCount() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData(), 2);
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpRoutingLostMessageProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 10
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingLostMessage;
}

/*!
    Returns a builder object to create a KNXnet/IP routing-lost message frame.
*/
QKnxNetIpRoutingLostMessageProxy::Builder QKnxNetIpRoutingLostMessageProxy::builder()
{
    return QKnxNetIpRoutingLostMessageProxy::Builder();
}


/*!
    \class QKnxNetIpRoutingLostMessageProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpRoutingLostMessageProxy

    \brief The QKnxNetIpRoutingLostMessageProxy::Builder class provides the
    means to create a routing-lost message frame.

    A routing-lost message is sent by a KNXnet/IP router to notify that
    KNXnet/IP routing indication frames were lost. The message contains
    the state of the router or device (\l QKnx::NetIp::DeviceState) and
    the number of lost frames.

    The following code sample illustrates how to create a routing-lost message
    about three frames being lost because an error occurred in the IP network:

    \code
        auto frame = QKnxNetIpRoutingLostMessageProxy::builder()
            .setDeviceState(QKnx::NetIp::DeviceState::IpFault)
            .setLostMessageCount(3)
            .create();
    \endcode
*/

/*!
    Sets the state of a KNXnet/IP router or device to \a state and returns a
    reference to the builder.
*/
QKnxNetIpRoutingLostMessageProxy::Builder &
    QKnxNetIpRoutingLostMessageProxy::Builder::setDeviceState(QKnxNetIp::DeviceState state)
{
    m_state = state;
    return *this;
}

/*!
    Sets the the number of frames that were lost to \a messageCount and returns
    a reference to the builder.
*/
QKnxNetIpRoutingLostMessageProxy::Builder &
    QKnxNetIpRoutingLostMessageProxy::Builder::setLostMessageCount(quint16 messageCount)
{
    m_lostMessageCount = messageCount;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP routing-lost message frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpRoutingLostMessageProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::RoutingLostMessage, QKnxByteArray { 0x04, quint8(m_state) }
        + QKnxUtils::QUint16::bytes(m_lostMessageCount) };
}

QT_END_NAMESPACE
