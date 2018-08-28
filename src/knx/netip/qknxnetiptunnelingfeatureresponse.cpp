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

#include "qknxnetiptunnelingfeatureresponse.h"
#include "qknxbuilderdata_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTunnelingFeatureResponseProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip
    \ingroup qtknx-tunneling

    \brief The QKnxNetIpTunnelingFeatureResponseProxy class provides the means
    to access the information carried by a generic tunneling feature-response
    service \l QKnxNetIpFrame frame and to create a KNXnet/IP frame based on
    the information.

    A tunneling feature-response service frame is sent by a KNXnet/IP server
    after receiving a tunneling feature-get or feature-set service frame. A
    tunneling feature-response service frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, the \l {QKnx::InterfaceFeature} {interface feature identifier}
    service used, the \l {QKnx::ReturnCode} {return code} in case of feature-set
    service, and the feature value as a byte array.

    The following code sample illustrates how to read the tunneling
    feature-response information sent by a KNXnet/IP server:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpTunnelingFeatureResponseProxy response(netIpFrame);
        if (!response.isValid())
            return;

        auto id = response.featureIdentifier();
        auto returnCode = response.returnCode();
        auto value = response.featureValue();
    \endcode

    \sa builder(), QKnxNetIpTunnelingFeatureGetProxy,
    QKnxNetIpTunnelingFeatureSetProxy,
    {Qt KNX Tunneling Classes}, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpTunnelingFeatureResponseProxy::QKnxNetIpTunnelingFeatureResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingFeatureResponseProxy::~QKnxNetIpTunnelingFeatureResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTunnelingFeatureResponseProxy::QKnxNetIpTunnelingFeatureResponseProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the tunneling feature-response information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpTunnelingFeatureResponseProxy::QKnxNetIpTunnelingFeatureResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpTunnelingFeatureResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 13 && QKnx::isInterfaceFeature(featureIdentifier())
        && m_frame.serviceType() == QKnxNetIp::ServiceType::TunnelingFeatureResponse;
}

/*!
    Returns the ID of the communication channel between a KNXnet/IP client and
    server.
*/
quint8 QKnxNetIpTunnelingFeatureResponseProxy::channelId() const
{
    return m_frame.channelId();
}

/*!
    Returns the sequence number of a tunneling feature-response service frame.
*/
quint8 QKnxNetIpTunnelingFeatureResponseProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

/*!
    Returns the tunneling interface feature identifier of a tunneling
    feature-response service frame.
*/
QKnx::InterfaceFeature QKnxNetIpTunnelingFeatureResponseProxy::featureIdentifier() const
{
    return QKnx::InterfaceFeature(m_frame.constData().value(0));
}

/*!
    Returns the tunneling interface feature return code of a tunneling
    feature-response service frame. If the return code is not present, the
    function always returns \l QKnx::Error.
*/
QKnx::ReturnCode QKnxNetIpTunnelingFeatureResponseProxy::returnCode() const
{
    return QKnx::ReturnCode(m_frame.constData().value(1, 0xff));
}

/*!
    Returns the tunneling interface feature value of a tunneling
    feature-response service frame.
*/
QKnxByteArray QKnxNetIpTunnelingFeatureResponseProxy::featureValue() const
{
    return m_frame.constData().mid(2);
}

/*!
    Returns a builder object to create a KNXnet/IP tunneling feature-response
    service frame.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder QKnxNetIpTunnelingFeatureResponseProxy::builder()
{
    return QKnxNetIpTunnelingFeatureResponseProxy::Builder();
}


/*!
    \class QKnxNetIpTunnelingFeatureResponseProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpTunnelingFeatureResponseProxy

    \brief The QKnxNetIpTunnelingFeatureResponseProxy::Builder class provides
    the means to create a KNXnet/IP tunneling feature-response service frame.

    A tunneling feature-response service frame is sent by a KNXnet/IP server
    after receiving a tunneling feature-get or feature-set service frame. A
    tunneling feature-response service frame contains the ID of the communication
    channel between a KNXnet/IP client and server, the sequence number of the
    frame, the \l {QKnx::InterfaceFeature} {interface feature identifier}
    service used, the \l {QKnx::ReturnCode} {return code} in case of feature-set
    service, and the feature value as a byte array.

    The common way to create a tunneling feature-response service frame is:
    \code
        auto frame = QKnxNetIpTunnelingFeatureResponseProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .setReturnCode(QKnx::ReturnCode::SuccessWithCrc)
            .setFeatureValue(QKnxAddress::Individual::Unregistered.bytes())
            .create();
    \endcode
*/

/*!
    Creates a new empty tunneling feature-response service frame builder object.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder::Builder()
    : d_ptr(new QKnxFeatureBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder::~Builder() = default;

/*!
    Sets the ID of the communication channel between a KNXnet/IP client and
    server to \a channelId and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder &
    QKnxNetIpTunnelingFeatureResponseProxy::Builder::setChannelId(quint8 channelId)
{
    d_ptr->m_channelId = channelId;
    return *this;
}

/*!
    Sets the sequence number of a tunneling feature-response service frame to
    \a sequenceNumber and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder &
    QKnxNetIpTunnelingFeatureResponseProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    d_ptr->m_sequenceNumber = sequenceNumber;
    return *this;
}

/*!
    Sets the interface feature identifier of a tunneling feature-response
    service frame to \a id and returns a reference to the builder.

    Does nothing if \a id is not a \l QKnx::InterfaceFeature.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder &
    QKnxNetIpTunnelingFeatureResponseProxy::Builder::setFeatureIdentifier(QKnx::InterfaceFeature id)
{
    if (QKnx::isInterfaceFeature(id))
        d_ptr->m_if = id;
    return *this;
}

/*!
    Sets the result code of a tunneling feature-response service frame to
    \a code and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder &
    QKnxNetIpTunnelingFeatureResponseProxy::Builder::setReturnCode(QKnx::ReturnCode code)
{
    d_ptr->m_result = code;
    return *this;
}

/*!
    Sets the feature value of the a tunneling feature-response service frame to
    \a value and returns a reference to the builder.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder &
    QKnxNetIpTunnelingFeatureResponseProxy::Builder::setFeatureValue(const QKnxByteArray &value)
{
    d_ptr->m_featureValue = value;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP tunneling feature-response service frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpTunnelingFeatureResponseProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::TunnelingFeatureResponse,
        { d_ptr->m_channelId, d_ptr->m_sequenceNumber }, /* connection header */
        QKnxByteArray { quint8(d_ptr->m_if) , quint8(d_ptr->m_result) }
        + d_ptr->m_featureValue
    };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTunnelingFeatureResponseProxy::Builder &
    QKnxNetIpTunnelingFeatureResponseProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
