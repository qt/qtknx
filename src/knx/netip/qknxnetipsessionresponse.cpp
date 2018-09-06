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

#include "qknxnetipsessionresponse.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSessionResponseProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSessionResponseProxy class provides the means to
    introspect session response data inside the generic \l QKnxNetIpFrame
    class and to create a KNXnet/IP session response frame from provided
    data.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent by the KNXnet/IP secure server to the KNXnet/IP
    secure client control endpoint in response to a received secure session
    request frame.

    \note When using QKnxNetIpSessionResponseProxy, care must be taken to
    ensure that the referenced KNXnet/IP frame outlives the proxy on all code
    paths, lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the session response
    information:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpSessionResponseProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        auto endPoint = proxy.controlEndpoint();
        auto publicKey = proxy.publicKey();
        auto authenticationCode = proxy.messageAuthenticationCode();
    \endcode

    \sa builder(), QKnxNetIpSessionRequestProxy, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSessionResponseProxy::QKnxNetIpSessionResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionResponseProxy::~QKnxNetIpSessionResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionResponseProxy::QKnxNetIpSessionResponseProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the session response information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSessionResponseProxy::QKnxNetIpSessionResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \note KNXnet/IP session response frames currently have a fixed size of
    \c 56 bytes.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSessionResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SessionResponse
        && m_frame.size() == 56;
}

/*!
    Returns the secure session ID from the generic KNXnet/IP session response
    frame if the object that was passed during construction was valid; otherwise
    returns a \l {default-constructed value} which can be \c 0.
*/
quint16 QKnxNetIpSessionResponseProxy::secureSessionId() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData());
}

/*!
    Returns the public key from the generic KNXnet/IP session response frame.
    The public key needs to be generated using the Curve25519 algorithm and
    has a fixed size of \c 32 bytes.
*/
QKnxByteArray QKnxNetIpSessionResponseProxy::publicKey() const
{
    return m_frame.constData().mid(sizeof(quint16), 32);
}

/*!
    Returns the AES128 CCM message authentication code from the generic
    KNXnet/IP session response frame with a fixed size of \c 16 bytes.
*/
QKnxByteArray QKnxNetIpSessionResponseProxy::messageAuthenticationCode() const
{
    return m_frame.constData().mid(sizeof(quint16) + 32);
}

/*!
    Returns a builder object to create a KNXnet/IP session response frame.
*/
QKnxNetIpSessionResponseProxy::Builder QKnxNetIpSessionResponseProxy::builder()
{
    return QKnxNetIpSessionResponseProxy::Builder();
}


/*!
    \class QKnxNetIpSessionResponseProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSessionResponseProxy

    \brief The QKnxNetIpSessionResponseProxy::Builder class provides the
    means to create a KNXnet/IP session response frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent by the KNXnet/IP secure server to the KNXnet/IP
    secure client control endpoint in response to a received secure session
    request frame.

    The common way to create a session response frame is:

    \code
        auto publicKey = ... // create the public key
        auto auth = ... // create the full 128 bit CCM-MAC

        auto netIpFrame = QKnxNetIpSessionResponseProxy::builder()
            .setSecureSessionId(0x1976)
            .setPublicKey(publicKey)
            .setMessageAuthenticationCode(auth)
            .create();
    \endcode

    \sa QKnxCryptographicEngine
*/

/*!
    Sets the secure session ID of the KNXnet/IP session response frame to
    \a sessionId and returns a reference to the builder.
*/
QKnxNetIpSessionResponseProxy::Builder &
    QKnxNetIpSessionResponseProxy::Builder::setSecureSessionId(quint16 sessionId)
{
    m_id = sessionId;
    return *this;
}

/*!
    Sets the public key of the KNXnet/IP session response frame to \a publicKey
    and returns a reference to the builder. The public key needs to be generated
    using the Curve25519 algorithm and has a fixed size of \c 32 bytes.
*/
QKnxNetIpSessionResponseProxy::Builder &
    QKnxNetIpSessionResponseProxy::Builder::setPublicKey(const QKnxByteArray &publicKey)
{
    m_publicKey = publicKey;
    return *this;
}

/*!
    Sets the AES128 CCM message authentication code of the generic KNXnet/IP
    session response frame to \a data and returns a reference to builder. The
    message authentication code has a fixed size of \c 16 bytes.
*/
QKnxNetIpSessionResponseProxy::Builder &
    QKnxNetIpSessionResponseProxy::Builder::setMessageAuthenticationCode(const QKnxByteArray &data)
{
    m_authCode = data;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP session response frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSessionResponseProxy::Builder::create() const
{
    if (m_id < 0 || m_publicKey.size() != 32 || m_authCode.size() != 16)
        return { QKnxNetIp::ServiceType::SessionResponse };
    return { QKnxNetIp::ServiceType::SessionResponse, QKnxUtils::QUint16::bytes(m_id) + m_publicKey
        + m_authCode };
}

QT_END_NAMESPACE
