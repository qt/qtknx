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

#include "qknxnetiproutingbusy.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpRoutingBusyProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpRoutingBusyProxy class provides the means to read
    a router busy message from the generic \l QKnxNetIpFrame class and to
    create a KNXnet/IP frame based on the information.

    Depending on the configuration, a KNXnet/IP router or device can receive
    more datagrams than it can forward to the KNX subnetwork. This can lead
    to an overflow of the datagram queue and subsequent loss of one or more
    KNXnet/IP telegrams, because they could not be transferred from the
    network buffer to the queue to the underlying KNX subnetwork. Flow control
    is implemented by means of sending router busy messages to avoid the loss
    of datagrams due to overflowing queues in routers and devices.

    A router busy message is sent by a KNXnet/IP router or device to notify
    all other devices that its incoming queue is filling up and it may loose
    datagrams if they do not stop sending. The message contains the state of
    the router or device (\l QKnx::NetIp::DeviceState), the timeout used to
    empty the incoming queue of the router or device, and the routing busy
    control field. The control field is set to \c 0000h, which requires all
    routers and devices to act upon receiving the router busy message.

    The following code sample illustrates how to read the router busy message
    information sent by a KNXnet/IP router or device:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpRoutingBusyProxy routerAnswer(netIpFrame);
        if (!routerAnswer.isValid())
            return;

        auto busyTime = routerAnswer.routingBusyWaitTime();
    \endcode

    The sending device will wait for \c busyTime before sending the next frame.

    \sa builder(), QKnxNetIpRoutingIndicationProxy,
    QKnxNetIpRoutingLostMessageProxy, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpRoutingBusyProxy::QKnxNetIpRoutingBusyProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingBusyProxy::~QKnxNetIpRoutingBusyProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingBusyProxy::QKnxNetIpRoutingBusyProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the router busy message information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpRoutingBusyProxy::QKnxNetIpRoutingBusyProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the state of a KNXnet/IP router or device.
*/
QKnxNetIp::DeviceState QKnxNetIpRoutingBusyProxy::deviceState() const
{
    return QKnxNetIp::DeviceState(m_frame.constData().value(1));
}

/*!
    Returns the timeout after which a KNXnet/IP router or device empties its
    incoming datagram queue. The timeout can be from 20 to 100 milliseconds.
*/
quint16 QKnxNetIpRoutingBusyProxy::routingBusyWaitTime() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData(), 2);
}

/*!
    Returns the router busy control field.
*/
quint16 QKnxNetIpRoutingBusyProxy::routingBusyControl() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData(), 4);
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of at
    least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpRoutingBusyProxy::isValid() const
{
    quint16 time = routingBusyWaitTime();
    if (time < 20)
        return false;
    return m_frame.isValid() && m_frame.size() == 12
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingBusy;
}

/*!
    Returns a builder object to create a KNXnet/IP router busy frame.
*/
QKnxNetIpRoutingBusyProxy::Builder QKnxNetIpRoutingBusyProxy::builder()
{
    return QKnxNetIpRoutingBusyProxy::Builder();
}


/*!
    \class QKnxNetIpRoutingBusyProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpRoutingBusyProxy

    \brief The QKnxNetIpRoutingBusyProxy::Builder class provides the means to
    create a router busy message frame.

    A router busy message is sent by a KNXnet/IP router or device to notify
    all other devices that its incoming queue is filling up and it may loose
    datagrams if they do not stop sending. The message contains the state of
    the router or device (\l QKnx::NetIp::DeviceState), the timeout used to
    empty the incoming queue of the router or device, and the routing busy
    control field. The control field is set to \c 0000h, which requires all
    routers and devices to act upon receiving the router busy message.

    The common way to create a routing-busy message is:

    \code
        auto frame = QKnxNetIpRoutingBusyProxy::builder()
            .setDeviceState(QKnx::NetIp::DeviceState::IpFault)
            .setRoutingBusyWaitTime(99)
            .setRoutingBusyControl(0xffff)
            .create();
    \endcode

    If a KNXnet/IP router or device receives a router busy message from another
    router or device, it immediately stops sending routing indication frames and
    waits until the time specified in the router busy message has elapsed to
    resume sending frames.
*/

/*!
    Sets the state of a KNXnet/IP router or device to \a state and returns a
    reference to the builder.
*/
QKnxNetIpRoutingBusyProxy::Builder &
    QKnxNetIpRoutingBusyProxy::Builder::setDeviceState(QKnxNetIp::DeviceState state)
{
    m_state = state;
    return *this;
}

/*!
    Sets the timeout after which a KNXnet/IP router or device empties its
    incoming datagram queue to \a waitTime and returns a reference to the
    builder.

    The timeout can be from 20 to 100 milliseconds.
*/
QKnxNetIpRoutingBusyProxy::Builder &
    QKnxNetIpRoutingBusyProxy::Builder::setRoutingBusyWaitTime(quint16 waitTime)
{
    m_waitTime = waitTime < 20 ? 20 : waitTime;
    return *this;
}

/*!
    Sets the router busy control field to \a busyControl and returns a reference
    to the builder.

    By default, the control field is set to \c 0000h, which requires all
    routers and devices to act upon receiving the router busy message.
*/
QKnxNetIpRoutingBusyProxy::Builder &
    QKnxNetIpRoutingBusyProxy::Builder::setRoutingBusyControl(quint16 busyControl)
{
    m_busyControl = busyControl;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP router busy message frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpRoutingBusyProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::RoutingBusy, QKnxByteArray { 0x06, quint8(m_state) }
        + QKnxUtils::QUint16::bytes(m_waitTime) + QKnxUtils::QUint16::bytes(m_busyControl) };
}

QT_END_NAMESPACE
