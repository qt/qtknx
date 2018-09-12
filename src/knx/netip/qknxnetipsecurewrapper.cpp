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

#include "qknxnetipsecurewrapper.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSecureWrapperProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSecureWrapperProxy class provides the means to introspect
    secure wrapper data inside the generic \l QKnxNetIpFrame class and to create
    a KNXnet/IP secure wrapper frame from provided data.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent during secure KNXnet/IP communication and includes
    a fully encrypted KNXnet/IP frame as well as information needed to decrypt
    the encapsulated frame and to ensure data integrity and freshness.

    \note When using QKnxNetIpSecureWrapperProxy, care must be taken to ensure
    that the referenced KNXnet/IP frame outlives the proxy on all code paths,
    lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the secure wrapper
    information:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpSecureWrapperProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        quint16 id = proxy.secureSessionId();
        quint48 seqNumber = proxy.sequenceNumber();
        QKnxByteArray number = proxy.serialNumber();
        quint16 tag = proxy.tag();
        QKnxByteArray frame = proxy.encapsulatedFrame();
        QKnxByteArray code = proxy.messageAuthenticationCode();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSecureWrapperProxy::QKnxNetIpSecureWrapperProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSecureWrapperProxy::~QKnxNetIpSecureWrapperProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSecureWrapperProxy::QKnxNetIpSecureWrapperProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the secure wrapper information carried by
    the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSecureWrapperProxy::QKnxNetIpSecureWrapperProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \note KNXnet/IP secure wrapper frames currently have a minimum size of
    \c 44 bytes.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSecureWrapperProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SecureWrapper
        && m_frame.size() >= 44;
}

/*!
    Returns the secure session ID from the generic KNXnet/IP secure wrapper
    frame.
*/
quint16 QKnxNetIpSecureWrapperProxy::secureSessionId() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData());
}

/*!
    Returns the sequence number from the generic KNXnet/IP secure wrapper
    frame.
*/
quint48 QKnxNetIpSecureWrapperProxy::sequenceNumber() const
{
    return QKnxUtils::QUint48::fromBytes(m_frame.constData().mid(2));
}

/*!
    Returns the serial number from the generic KNXnet/IP secure wrapper frame.
*/
QKnxByteArray QKnxNetIpSecureWrapperProxy::serialNumber() const
{
    return m_frame.constData().mid(8, 6);
}

/*!
    Returns the message tag from the generic KNXnet/IP secure wrapper frame.
*/
quint16 QKnxNetIpSecureWrapperProxy::messageTag() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData().mid(14));
}

/*!
    Returns the encrypted encapsulated frame from the generic KNXnet/IP secure
    wrapper frame.
*/
QKnxByteArray QKnxNetIpSecureWrapperProxy::encapsulatedFrame() const
{
    auto data = m_frame.constData().mid(16);
    return data.mid(0, data.size() - 16); // remove the MAC
}

/*!
    Returns the AES128 CCM message authentication code (MAC) from the generic
    KNXnet/IP secure wrapper frame with a fixed size of \c 16 bytes.
*/
QKnxByteArray QKnxNetIpSecureWrapperProxy::messageAuthenticationCode() const
{
    return m_frame.constData().mid(m_frame.dataSize() - 16);
}

/*!
    Returns a builder object to create a KNXnet/IP secure wrapper frame.
*/
QKnxNetIpSecureWrapperProxy::Builder QKnxNetIpSecureWrapperProxy::builder()
{
    return QKnxNetIpSecureWrapperProxy::Builder();
}


/*!
    \class QKnxNetIpSecureWrapperProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSecureWrapperProxy

    \brief The QKnxNetIpSecureWrapperProxy::Builder class provides the
    means to create a KNXnet/IP secure wrapper frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent during secure KNXnet/IP communication and includes
    a fully encrypted KNXnet/IP frame as well as information needed to decrypt
    the encapsulated frame and to ensure data integrity and freshness.

    The common way to create a secure wrapper frame is:

    \code
        auto auth = ... // create the full 128 bit CCM-MAC
        auto frame = ... // create encrypted frame

        auto netIpFrame = QKnxNetIpSecureWrapperProxy::builder()
            .setSequenceNumber(15021976)
            .setSerialNumber(QKnxByteArray::fromHex("0123456789AB"))
            .setEncapsulatedFrame(frame)
            .setMessageAuthenticationCode(auth)
            .create();
    \endcode

    \sa QKnxCryptographicEngine
*/

/*!
    Sets the secure session ID to \a sessionId and returns a reference to the
    builder. By default value is set to \c 0x0000.

    For multicast connections the fixed identifier \c 0x0000 must be used. For
    unicast connections the ID was established during a previous successful
    secure session setup procedure.
*/
QKnxNetIpSecureWrapperProxy::Builder &
    QKnxNetIpSecureWrapperProxy::Builder::setSecureSessionId(quint16 sessionId)
{
    m_sessionId = sessionId;
    return *this;
}

/*!
    Sets the sequence number to \a seqNumber and returns a reference to the
    builder.

    For unicast connections it is a monotonically increasing sequence number
    assigned by the sender; incremented by the sender after each frame sent.
    For multicast connections this is the device's current multicast timer
    value in millisecond resolution.

    \note The size of a sequence number is limited to 48 bits, so the maximum
    number can be \c 281474976710655. Passing a larger value will result in
    creating an invalid frame.
*/
QKnxNetIpSecureWrapperProxy::Builder &
    QKnxNetIpSecureWrapperProxy::Builder::setSequenceNumber(quint48 seqNumber)
{
    m_seqNumber = seqNumber;
    return *this;
}

/*!
    Sets the serial number to \a serialNumber and returns a reference to the
    builder.

    \note The serial number must contain exactly 6 bytes.
*/
QKnxNetIpSecureWrapperProxy::Builder &
    QKnxNetIpSecureWrapperProxy::Builder::setSerialNumber(const QKnxByteArray &serialNumber)
{
    m_serial = serialNumber;
    return *this;
}

/*!
    Sets the message tag of the generic KNXnet/IP secure wrapper frame to \a tag
    and returns a reference to builder. By default value is set to \c 0x0000.

    This field contains an arbitrary value to differentiate two KNXnet/IP
    secure wrapper multicast frames sent by one KNXnet/IP device within the
    same millisecond (thus the same timer value).
    For unicast connections this field is ignored and must be set to \c 0x0000.
*/
QKnxNetIpSecureWrapperProxy::Builder &
    QKnxNetIpSecureWrapperProxy::Builder::setMessageTag(quint16 tag)
{
    m_tag = tag;
    return *this;
}

/*!
    Sets the encapsulated and already encrypted KNXnet/IP frame to \a frame and
    returns a reference to the builder.

    \note A secure wrapper frame cannot be encapsulated in another secure
    wrapper frame and will result in creating an invalid frame.
*/
QKnxNetIpSecureWrapperProxy::Builder &
    QKnxNetIpSecureWrapperProxy::Builder::setEncapsulatedFrame(const QKnxByteArray &frame)
{
    m_frame = frame;
    return *this;
}

/*!
    Sets the AES128 CCM message authentication code (MAC) of the generic
    KNXnet/IP secure wrapper frame to \a data and returns a reference to
    builder. The MAC has a fixed size of \c 16 bytes.
*/
QKnxNetIpSecureWrapperProxy::Builder &
    QKnxNetIpSecureWrapperProxy::Builder::setMessageAuthenticationCode(const QKnxByteArray &data)
{
    m_authCode = data;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP secure wrapper frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSecureWrapperProxy::Builder::create() const
{
    if (m_seqNumber > Q_UINT48_MAX || m_serial.size() != 6 ||  m_authCode.size() != 16)
        return { QKnxNetIp::ServiceType::SecureWrapper };

    // TODO: introspect the frame and reject secure wrapper frames

    return { QKnxNetIp::ServiceType::SecureWrapper, QKnxUtils::QUint16::bytes(m_sessionId)
        + QKnxUtils::QUint48::bytes(m_seqNumber) + m_serial + QKnxUtils::QUint16::bytes(m_tag)
        + m_frame + m_authCode };
}

QT_END_NAMESPACE
