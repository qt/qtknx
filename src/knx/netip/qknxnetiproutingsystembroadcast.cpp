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

#include "qknxnetiproutingsystembroadcast.h"

#include "qknxaddress.h"
#include "qknxcontrolfield.h"
#include "qknxextendedcontrolfield.h"
#include "qknxlinklayerframebuilder.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpRoutingSystemBroadcastProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpRoutingSystemBroadcastProxy class provides the means
    to introspect a generic routing system broadcast \l QKnxNetIpFrame and to
    create a KNXnet/IP frame based on the information.

    Routing system broadcast messages are useful in the situation when
    KNXnet/IP devices configured on different multicast addresses and secure
    KNXnet/IP devices configured on different backbone keys are not able to
    communicate between each other.

    The following code sample illustrates how to introspect a routing system
    broadcast frame:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpRoutingSystemBroadcastProxy sbc(netIpFrame);
        if (!sbc.isValid())
            return;

        auto linkFrame = sbc.linkLayerFrame();
    \endcode

    \sa builder(), QKnxNetIpRoutingSystemBroadcastProxy,
    {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpRoutingSystemBroadcastProxy::QKnxNetIpRoutingSystemBroadcastProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingSystemBroadcastProxy::~QKnxNetIpRoutingSystemBroadcastProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingSystemBroadcastProxy::QKnxNetIpRoutingSystemBroadcastProxy(const QKnxNetIpFrame &&)
    \internal
*/

namespace QKnxPrivate
{
    static bool isCemiValid(const QKnxLinkLayerFrame &linkFrame)
    {
        // TODO: Check if the cEMI message is an S-A_Data-PDU, S-A_Sync_Request-PDU or
        // S-A_Sync_-Response-PDU, the SBC bit in the Security Control Field (SCF) as
        // specified in (AN 158: Data security) shall be set to 1.
        return linkFrame.extendedControlField().destinationAddressType() == QKnxAddress::Type::Group
               && linkFrame.controlField().broadcast() == QKnxControlField::Broadcast::System
               && linkFrame.messageCode() != QKnxLinkLayerFrame::MessageCode::DataIndication
               && linkFrame.destinationAddress().type() != QKnxAddress::Type::Group;
    }
}

/*!
    Constructs a proxy object to read the routing system broadcast carried
    by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpRoutingSystemBroadcastProxy::QKnxNetIpRoutingSystemBroadcastProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.
*/
bool QKnxNetIpRoutingSystemBroadcastProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 8
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingSystemBroadcast
        && QKnxPrivate::isCemiValid(cemi());
}

/*!
    Returns a cEMI frame that contains the routing system broadcast message.
*/
QKnxLinkLayerFrame QKnxNetIpRoutingSystemBroadcastProxy::cemi() const
{
    return QKnxLinkLayerFrame::builder()
        .setMedium(QKnx::MediumType::NetIP)
        .setData(m_frame.constData())
        .createFrame();
}

/*!
    Returns a builder object to create a KNXnet/IP routing system broadcast
    frame.
*/
QKnxNetIpRoutingSystemBroadcastProxy::Builder QKnxNetIpRoutingSystemBroadcastProxy::builder()
{
    return QKnxNetIpRoutingSystemBroadcastProxy::Builder();
}


/*!
    \class QKnxNetIpRoutingSystemBroadcastProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpRoutingSystemBroadcastProxy

    \brief The QKnxNetIpRoutingSystemBroadcastProxy::Builder class provides
    the means to create a KNXnet/IP routing system broadcast frame.

    The common way to create a routing system broadcast frame is:

    \code
        QKnxLinkLayerFrame cemi = // build cEMI frame
        auto netIpFrame = QKnxNetIpRoutingSystemBroadcastProxy::builder()
            .setCemi(cemi)
            .create();
    \endcode
*/

/*!
    Sets the cEMI frame containing the routing system broadcast message to
    \a cemi.
*/
QKnxNetIpRoutingSystemBroadcastProxy::Builder &
    QKnxNetIpRoutingSystemBroadcastProxy::Builder::setCemi(const QKnxLinkLayerFrame &cemi)
{
    if (QKnxPrivate::isCemiValid(cemi))
        return *this;

    m_llf = cemi;
    return *this;
}

/*!
    Creates a KNXnet/IP frame that contains the routing system broadcast message.
*/
QKnxNetIpFrame QKnxNetIpRoutingSystemBroadcastProxy::Builder::create() const
{
    if (!m_llf.isValid())
        return {};
    return { QKnxNetIp::ServiceType::RoutingSystemBroadcast, m_llf.bytes() };
}

QT_END_NAMESPACE
