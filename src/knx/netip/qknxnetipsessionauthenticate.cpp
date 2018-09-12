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

#include "qknxbuilderdata_p.h"
#include "qknxnetipsessionauthenticate.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSessionAuthenticateProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSessionAuthenticateProxy class provides the means to
    introspect session authentication data inside the generic \l QKnxNetIpFrame
    class and to create a KNXnet/IP session authentication frame from provided
    data.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent by the KNXnet/IP secure client to the control
    endpoint of the KNXnet/IP secure server after the Diffie-Hellman handshake
    to authenticate the user against the server device.
    The maximum time a KNXnet/IP secure client will wait for an authentication
    status response of the KNXnet/IP secure server is 10 seconds.

    \note When using QKnxNetIpSessionAuthenticateProxy, care must be taken to
    ensure that the referenced KNXnet/IP frame outlives the proxy on all code
    paths, lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the session authentication
    information:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpSessionAuthenticateProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        auto id = proxy.userId();
        auto authenticatenCode = proxy.messageAuthenticationCode();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSessionAuthenticateProxy::QKnxNetIpSessionAuthenticateProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionAuthenticateProxy::~QKnxNetIpSessionAuthenticateProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionAuthenticateProxy::QKnxNetIpSessionAuthenticateProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the session authentication information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSessionAuthenticateProxy::QKnxNetIpSessionAuthenticateProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \note A userId() with the value \c 0x00 or a value above \c 0x80
    is considered invalid according to the KNX application note AN159.

    \note KNXnet/IP session authentication frames currently have a fixed size
    of \c 24 bytes.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSessionAuthenticateProxy::isValid() const
{
    const auto id = userId();
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SessionAuthenticate
        && m_frame.size() == 24 && id > 0 && id < 0x80; // 0x00 and values above 0x80 are reserved
}

/*!
    Returns the user ID from the generic KNXnet/IP session authentication
    frame. The value of \c 0 is a reserved value and indicates an invalid
    user ID.
*/
quint8 QKnxNetIpSessionAuthenticateProxy::userId() const
{
    return m_frame.constData().value(1);
}

/*!
    Returns the AES128 CCM message authentication code (MAC) from the generic
    KNXnet/IP session authentication frame with a fixed size of \c 16 bytes.
*/
QKnxByteArray QKnxNetIpSessionAuthenticateProxy::messageAuthenticationCode() const
{
    return m_frame.constData().mid(sizeof(quint16));
}

/*!
    Returns a builder object to create a KNXnet/IP session authentication frame.
*/
QKnxNetIpSessionAuthenticateProxy::Builder QKnxNetIpSessionAuthenticateProxy::builder()
{
    return QKnxNetIpSessionAuthenticateProxy::Builder();
}


/*!
    \class QKnxNetIpSessionAuthenticateProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSessionAuthenticateProxy

    \brief The QKnxNetIpSessionAuthenticateProxy::Builder class provides the
    means to create a KNXnet/IP session authentication frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent by the KNXnet/IP secure client to the control
    endpoint of the KNXnet/IP secure server after the Diffie-Hellman handshake
    to authenticate the user against the server device.
    The maximum time a KNXnet/IP secure client will wait for an authentication
    status response of the KNXnet/IP secure server is 10 seconds.

    The common way to create a session authentication frame is:

    \code
        quint16 mgmtLevelAccess = 0x0001;
        auto auth = ... // create the full 128 bit CCM-MAC

        auto netIpFrame = QKnxNetIpSessionAuthenticateProxy::builder()
            .setUserId(mgmtLevelAccess)
            .setMessageAuthenticationCode(auth)
            .create();
    \endcode

    \sa QKnxCryptographicEngine
*/

/*!
    Creates a new empty session authentication frame builder.
*/
QKnxNetIpSessionAuthenticateProxy::Builder::Builder()
    : d_ptr(new QKnxNetIpSessionAuthenticateBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpSessionAuthenticateProxy::Builder::~Builder() = default;

/*!
    Sets the user ID of the KNXnet/IP session authentication frame to \a userId
    and returns a reference to the builder.

    \note A userId() with the value \c 0x00 or a value above \c 0x80
    is considered invalid according to the KNX application note AN159.
*/
QKnxNetIpSessionAuthenticateProxy::Builder &
    QKnxNetIpSessionAuthenticateProxy::Builder::setUserId(quint8 userId)
{
    d_ptr->m_id = userId;
    return *this;
}

/*!
    Sets the AES128 CCM message authentication code (MAC) of the generic
    KNXnet/IP session authentication frame to \a data and returns a reference
    to the builder. The MAC has a fixed size of \c 16 bytes.
*/
QKnxNetIpSessionAuthenticateProxy::Builder &
    QKnxNetIpSessionAuthenticateProxy::Builder::setMessageAuthenticationCode(const QKnxByteArray &data)
{
    d_ptr->m_authCode = data;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP session authentication frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSessionAuthenticateProxy::Builder::create() const
{
    if (d_ptr->m_id == 0 || d_ptr->m_id >= 0x80 ||  d_ptr->m_authCode.size() != 16)
        return { QKnxNetIp::ServiceType::SessionAuthenticate };

    return { QKnxNetIp::ServiceType::SessionAuthenticate, QKnxUtils::QUint16::bytes(d_ptr->m_id)
        + d_ptr->m_authCode };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSessionAuthenticateProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpSessionAuthenticateProxy::Builder &
    QKnxNetIpSessionAuthenticateProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


/*!
    \class QKnxNetIpSessionAuthenticateProxy::SecureBuilder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSessionAuthenticateProxy

    \brief The QKnxNetIpSessionAuthenticateProxy::SecureBuilder class provides the
    means to create a KNXnet/IP session authentication frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    \note To use this class OpenSSL must be supported on your target system.

    This frame will be sent by the KNXnet/IP secure client to the control
    endpoint of the KNXnet/IP secure server after the Diffie-Hellman handshake
    to authenticate the user against the server device.
    The maximum time a KNXnet/IP secure client will wait for an authentication
    status response of the KNXnet/IP secure server is 10 seconds.

    The common way to create a session authentication frame is:

    \code
        quint16 mgmtLevelAccess = 0x0001;

        auto netIpFrame = QKnxNetIpSessionAuthenticateProxy::secureBuilder()
            .setUserId(mgmtLevelAccess)
            .create(passwordHash, clientKey, serverKey);
    \endcode

    \sa QKnxCryptographicEngine
*/

/*!
    Creates a new empty session authentication frame builder.
*/
QKnxNetIpSessionAuthenticateProxy::SecureBuilder::SecureBuilder()
    : d_ptr(new QKnxNetIpSessionAuthenticateBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpSessionAuthenticateProxy::SecureBuilder::~SecureBuilder() = default;

/*!
    Sets the user ID of the KNXnet/IP session authentication frame to \a userId
    and returns a reference to the builder.

    \note A userId() with the value \c 0x00 or a value above \c 0x80
    is considered invalid according to the KNX application note AN159.
*/
QKnxNetIpSessionAuthenticateProxy::SecureBuilder &
    QKnxNetIpSessionAuthenticateProxy::SecureBuilder::setUserId(quint8 userId)
{
    d_ptr->m_id = userId;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP session authentication frame.

    The function calculates the AES128 CCM message authentication code (MAC)
    with the given user session password \a sessionPassword, the Curve25519
    client public key \a clientPublicKey, the Curve25519 server public key
    \a serverPublicKey and appends it to the newly created frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSessionAuthenticateProxy::SecureBuilder::create(
                                                        const QByteArray &sessionPassword,
                                                        const QKnxByteArray &clientPublicKey,
                                                        const QKnxByteArray &serverPublicKey) const
{
#if QT_CONFIG(opensslv11)
    if (d_ptr->m_id == 0 || d_ptr->m_id >= 0x80 )
        return { QKnxNetIp::ServiceType::SessionAuthenticate };

    auto builder = QKnxNetIpSessionAuthenticateProxy::builder();
    auto frame = builder
        .setUserId(d_ptr->m_id)
        .setMessageAuthenticationCode(QKnxByteArray(16, 0x00)) // dummy MAC to get a proper header
        .create();

    auto userPasswordHash = QKnxCryptographicEngine::userPasswordHash(sessionPassword);
    auto mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(userPasswordHash, frame.
        header(), d_ptr->m_id, QKnxCryptographicEngine::XOR(clientPublicKey, serverPublicKey));
    mac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(userPasswordHash, mac);

    return builder.setMessageAuthenticationCode(mac).create();
#else
    Q_UNUSED(sessionPassword)
    Q_UNUSED(clientPublicKey)
    Q_UNUSED(serverPublicKey)
    return { QKnxNetIp::ServiceType::SessionAuthenticate };
#endif
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSessionAuthenticateProxy::SecureBuilder::SecureBuilder(const SecureBuilder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpSessionAuthenticateProxy::SecureBuilder &
    QKnxNetIpSessionAuthenticateProxy::SecureBuilder::operator=(const SecureBuilder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Returns a builder object to create a KNXnet/IP session authentication frame.
*/
QKnxNetIpSessionAuthenticateProxy::SecureBuilder QKnxNetIpSessionAuthenticateProxy::secureBuilder()
{
    return QKnxNetIpSessionAuthenticateProxy::SecureBuilder();
}

QT_END_NAMESPACE
