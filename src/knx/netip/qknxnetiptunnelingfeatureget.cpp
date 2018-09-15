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

#include "qknxnetiptunnelingfeatureget.h"
#include "qknxbuilderdata_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTunnelingFeatureGetProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip
    \ingroup qtknx-tunneling

    \brief The QKnxNetIpTunnelingFeatureGetProxy class provides the means to
    access the information carried by a generic tunneling feature-get service
    \l QKnxNetIpFrame frame and to create a KNXnet/IP frame based on the
    information.

    A tunneling feature-get service frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, and the \l {QKnx::InterfaceFeature} {interface feature} to read.

    The tunneling client initiates the tunneling feature-get service to read
    the value of an interface feature from the tunneling server. Within 3
    seconds after confirming the tunneling feature-get service the tunneling
    server sends a tunneling feature-response frame containing the value of the
    requested interface feature.

    If the tunneling server does not respond to the tunneling feature-get
    service frame the tunneling client may or may not repeat the request.

    \note When using QKnxNetIpTunnelingFeatureGetProxy, care must be taken to
    ensure that the referenced KNXnet/IP frame outlives the proxy on all code
    paths, lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the tunneling
    feature-get information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpTunnelingFeatureGetProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        auto id = proxy.featureIdentifier();
    \endcode

    \sa builder(), QKnxNetIpTunnelingFeatureInfoProxy,
    QKnxNetIpTunnelingFeatureResponseProxy,
    {Qt KNX Tunneling Classes}, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpTunnelingFeatureGetProxy::QKnxNetIpTunnelingFeatureGetProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingFeatureGetProxy::~QKnxNetIpTunnelingFeatureGetProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingFeatureGetProxy::QKnxNetIpTunnelingFeatureGetProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the tunneling feature-get service
    information carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpTunnelingFeatureGetProxy::QKnxNetIpTunnelingFeatureGetProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpTunnelingFeatureGetProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 12 && QKnx::isInterfaceFeature(featureIdentifier())
        && m_frame.serviceType() == QKnxNetIp::ServiceType::TunnelingFeatureGet;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpTunnelingFeatureGetProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a tunneling feature-get service frame.
*/
quint8 QKnxNetIpTunnelingFeatureGetProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns the tunneling interface feature identifier of a tunneling
    feature-get service frame.
*/
QKnx::InterfaceFeature QKnxNetIpTunnelingFeatureGetProxy::featureIdentifier() const
{
    return QKnx::InterfaceFeature(m_frame.constData().value(0));
}

/*!
    Returns a builder object to create a KNXnet/IP tunneling feature-get
    service frame.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder QKnxNetIpTunnelingFeatureGetProxy::builder()
{
    return QKnxNetIpTunnelingFeatureGetProxy::Builder();
}


/*!
    \class QKnxNetIpTunnelingFeatureGetProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpTunnelingFeatureGetProxy

    \brief The QKnxNetIpTunnelingFeatureGetProxy::Builder class provides
    the means to create a KNXnet/IP tunneling feature get service frame.

    A tunneling feature-get service frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, and the \l {QKnx::InterfaceFeature} {interface feature} to read.

    The tunneling client initiates the tunneling feature-get service to read
    the value of an interface feature from the tunneling server. Within 3
    seconds after confirming the tunneling feature-get service the tunneling
    server sends a tunneling feature-response frame containing the value of the
    requested interface feature.

    If the tunneling server does not respond to the tunneling feature-get
    service frame the tunneling client may or may not repeat the request.

    The common way to create a tunneling feature-get service frame is:
    \code
        auto frame = QKnxNetIpTunnelingFeatureGetProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .create();
    \endcode
*/

/*!
    Creates a new empty tunneling feature-get service frame builder object.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder::Builder()
    : d_ptr(new QKnxFeatureBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder::~Builder() = default;

/*!
    Sets the ID of the communication channel between a KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder &
    QKnxNetIpTunnelingFeatureGetProxy::Builder::setChannelId(quint8 channelId)
{
    d_ptr->m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a tunneling feature-get service frame to
    \a sequenceNumber and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder &
    QKnxNetIpTunnelingFeatureGetProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    d_ptr->m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the interface feature identifier of a tunneling feature-get service
    frame to \a id and returns a reference to the builder.

    Does nothing if \a id is not a \l QKnx::InterfaceFeature.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder &
    QKnxNetIpTunnelingFeatureGetProxy::Builder::setFeatureIdentifier(QKnx::InterfaceFeature id)
{
    if (QKnx::isInterfaceFeature(id))
        d_ptr->m_if = id;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP tunneling feature-get service frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpTunnelingFeatureGetProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::TunnelingFeatureGet,
        { d_ptr->m_channelId, d_ptr->m_sequenceNumber }, /* connection header */
        { quint8(d_ptr->m_if) , 0x00 } /* interface feature identifier */
    };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTunnelingFeatureGetProxy::Builder &
    QKnxNetIpTunnelingFeatureGetProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
