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

#include "qknxnetiptunnelingfeatureset.h"
#include "qknxbuilderdata_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTunnelingFeatureSetProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip
    \ingroup qtknx-tunneling

    \brief The QKnxNetIpTunnelingFeatureSetProxy class provides the means to
    access the information carried by a generic tunneling feature-set service
    \l QKnxNetIpFrame frame and to create a KNXnet/IP frame based on the
    information.

    A tunneling feature-set service frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, the \l {QKnx::InterfaceFeature} {interface feature} to set, and the
    feature value as a byte array.

    The tunneling client initiates the tunneling feature-set service to set the
    value of an interface feature in a tunneling server. The tunneling server
    accepts the value and applies it immediately, unless specified differently.

    This means that the server applies the value before sending the tunneling
    feature-response or before routing the next tunneling request on the bus.

    Within 3 seconds of confirming the tunneling feature-set service, the
    tunneling server sends a tunneling feature-response frame. The frame
    contains the written value and a \l {QKnx::ReturnCode} {return code}.

    The following code sample illustrates how to read the tunneling feature-set
    service information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpTunnelingFeatureSetProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        auto id = proxy.featureIdentifier();
        auto value = proxy.featureValue();
    \endcode

    \sa builder(), QKnxNetIpTunnelingFeatureInfoProxy,
    QKnxNetIpTunnelingFeatureResponseProxy,
    {Qt KNX Tunneling Classes}, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpTunnelingFeatureSetProxy::QKnxNetIpTunnelingFeatureSetProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingFeatureSetProxy::~QKnxNetIpTunnelingFeatureSetProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingFeatureSetProxy::QKnxNetIpTunnelingFeatureSetProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the tunneling feature-set service
    information carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpTunnelingFeatureSetProxy::QKnxNetIpTunnelingFeatureSetProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpTunnelingFeatureSetProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 13 && QKnx::isInterfaceFeature(featureIdentifier())
        && m_frame.serviceType() == QKnxNetIp::ServiceType::TunnelingFeatureSet;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpTunnelingFeatureSetProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a tunneling feature-set service frame.
*/
quint8 QKnxNetIpTunnelingFeatureSetProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns the tunneling interface feature identifier of a tunneling
    feature-set service frame.
*/
QKnx::InterfaceFeature QKnxNetIpTunnelingFeatureSetProxy::featureIdentifier() const
{
    return QKnx::InterfaceFeature(m_frame.constData().value(0));
}

/*!
    Returns the tunneling interface feature value of a tunneling feature-set
    service frame.
*/
QKnxByteArray QKnxNetIpTunnelingFeatureSetProxy::featureValue() const
{
    return m_frame.constData().mid(2);
}

/*!
    Returns a builder object to create a KNXnet/IP tunneling feature-set
    service frame.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder QKnxNetIpTunnelingFeatureSetProxy::builder()
{
    return QKnxNetIpTunnelingFeatureSetProxy::Builder();
}


/*!
    \class QKnxNetIpTunnelingFeatureSetProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpTunnelingFeatureSetProxy

    \brief The QKnxNetIpTunnelingFeatureSetProxy::Builder class provides
    the means to create a KNXnet/IP tunneling feature-set service frame.

    A tunneling feature-set service frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, the \l {QKnx::InterfaceFeature} {interface feature} to set, and the
    feature value as a byte array.

    The tunneling client initiates the tunneling feature-set service to set the
    value of an interface feature in a tunneling server. The tunneling server
    accepts the value and applies it immediately, unless specified differently.

    This means that the server applies the value before sending the tunneling
    feature-response or before routing the next tunneling request on the bus.

    Within 3 seconds of confirming the tunneling feature-set service, the
    tunneling server sends a tunneling feature-response frame. The frame
    contains the written value and a \l {QKnx::ReturnCode} {return code}.

    The common way to create a tunneling feature-set service frame is:
    \code
        auto frame = QKnxNetIpTunnelingFeatureSetProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .setFeatureValue(QKnxAddress::Individual::Unregistered.bytes())
            .create();
    \endcode
*/

/*!
    Creates a new empty tunneling feature-set service frame builder object.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder::Builder()
    : d_ptr(new QKnxFeatureBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder::~Builder() = default;

/*!
    Sets the ID of the communication channel between a KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder &
    QKnxNetIpTunnelingFeatureSetProxy::Builder::setChannelId(quint8 channelId)
{
    d_ptr->m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a tunneling feature-set service frame to
    \a sequenceNumber and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder &
    QKnxNetIpTunnelingFeatureSetProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    d_ptr->m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the interface feature identifier of a tunneling feature-set service
    frame to \a id and returns a reference to the builder.

    Does nothing if \a id is not a \l QKnx::InterfaceFeature.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder &
    QKnxNetIpTunnelingFeatureSetProxy::Builder::setFeatureIdentifier(QKnx::InterfaceFeature id)
{
    if (QKnx::isInterfaceFeature(id))
        d_ptr->m_if = id;
    return *this;
}

/*!
    Sets the feature value of the a tunneling feature-set service frame to
    \a value and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder &
    QKnxNetIpTunnelingFeatureSetProxy::Builder::setFeatureValue(const QKnxByteArray &value)
{
    d_ptr->m_featureValue = value;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP tunneling feature-set service frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpTunnelingFeatureSetProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::TunnelingFeatureSet,
        { d_ptr->m_channelId, d_ptr->m_sequenceNumber }, /* connection header */
        QKnxByteArray { quint8(d_ptr->m_if) , 0x00 } /* interface feature identifier */
        + d_ptr->m_featureValue
    };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTunnelingFeatureSetProxy::Builder &
    QKnxNetIpTunnelingFeatureSetProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
