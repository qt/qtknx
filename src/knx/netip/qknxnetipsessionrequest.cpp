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

#include "qknxnetipsessionrequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSessionRequestProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSessionRequestProxy class provides the means to
    introspect session request data inside the generic \l QKnxNetIpFrame
    class and to create a KNXnet/IP session request frame from provided data.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent by a KNXnet/IP secure client to the control
    endpoint of the KNXnet/IP secure server to initiate the secure session
    setup handshake for a new secure communication channel.
    The maximum time a KNXnet/IP secure client will wait for a response of the
    KNXnet/IP secure server is 10 seconds.

    \note When using QKnxNetIpSessionRequestProxy, care must be taken to ensure
    that the referenced KNXnet/IP frame outlives the proxy on all code paths,
    lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the session request
    information:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpSessionRequestProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        auto endPoint = proxy.controlEndpoint();
        auto publicKey = proxy.publicKey();
    \endcode

    \sa builder(), QKnxNetIpSessionResponseProxy, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSessionRequestProxy::QKnxNetIpSessionRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionRequestProxy::~QKnxNetIpSessionRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionRequestProxy::QKnxNetIpSessionRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the session request information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSessionRequestProxy::QKnxNetIpSessionRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \note KNXnet/IP session request frames currently have a fixed size of \c 46
    bytes.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSessionRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SessionRequest
        && m_frame.size() == 46;
}

/*!
    Returns the control endpoint from the generic KNXnet/IP session request
    frame.
*/
QKnxNetIpHpai QKnxNetIpSessionRequestProxy::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

/*!
    Returns the public key from the generic KNXnet/IP session request frame.
    The public key needs to be generated using the Curve25519 algorithm and
    has a fixed size of \c 32 bytes.
*/
QKnxByteArray QKnxNetIpSessionRequestProxy::publicKey() const
{
    const auto &data = m_frame.constData();
    auto header = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>::fromBytes(data, 0);
    if (header.isValid())
        return data.mid(header.totalSize());
    return {};
}

/*!
    Returns a builder object to create a KNXnet/IP session request frame.
*/
QKnxNetIpSessionRequestProxy::Builder QKnxNetIpSessionRequestProxy::builder()
{
    return QKnxNetIpSessionRequestProxy::Builder();
}


/*!
    \class QKnxNetIpSessionRequestProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSessionRequestProxy

    \brief The QKnxNetIpSessionRequestProxy::Builder class provides the
    means to create a KNXnet/IP session request frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent by a KNXnet/IP secure client to the control
    endpoint of the KNXnet/IP secure server to initiate the secure session
    setup handshake for a new secure communication channel.
    The maximum time a KNXnet/IP secure client will wait for a response of the
    KNXnet/IP secure server is 10 seconds.

    The common way to create a session request frame is:

    \code
        auto publicKey = ... // create Client Public Value X

        auto netIpFrame = QKnxNetIpSessionRequestProxy::builder()
            .setControlEndpoint(QKnxNetIpHpaiProxy::builder()
                .setHostAddress(QHostAddress::LocalHost)
                .setPort(3671).create())
            .setPublicKey(publicKey)
            .create();
    \endcode
*/

/*!
    Sets the control endpoint of the KNXnet/IP session request frame to \a hpai
    and returns a reference to the builder.
*/
QKnxNetIpSessionRequestProxy::Builder &
    QKnxNetIpSessionRequestProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Sets the unencrypted data of the KNXnet/IP session request frame to
    \a publicKey and returns a reference to the builder. The public key
    needs to be generated using the Curve25519 algorithm and has a fixed
    size of \c 32 bytes.
*/
QKnxNetIpSessionRequestProxy::Builder &
    QKnxNetIpSessionRequestProxy::Builder::setPublicKey(const QKnxByteArray &publicKey)
{
    m_publicKey = publicKey;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP session request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSessionRequestProxy::Builder::create() const
{
    if (m_hpai.isValid() && m_publicKey.size() == 32)
        return { QKnxNetIp::ServiceType::SessionRequest, m_hpai.bytes() + m_publicKey };
    return { QKnxNetIp::ServiceType::SessionRequest };
}

QT_END_NAMESPACE
