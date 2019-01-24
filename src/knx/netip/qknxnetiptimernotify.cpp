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
#include "qknxnetiptimernotify.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTimerNotifyProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpTimerNotifyProxy class provides the means to introspect
    timer notify data inside the generic \l QKnxNetIpFrame class and to create
    a KNXnet/IP timer notify frame from provided data.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent during secure KNXnet/IP multicast group
    communication to keep the multicast group member's timer values
    synchronized. The frame is therefore sent to the KNXnet/IP routing
    endpoint on port \c 3671 of the configured routing multicast address.

    \note When using QKnxNetIpTimerNotifyProxy, care must be taken to ensure
    that the referenced KNXnet/IP frame outlives the proxy on all code paths,
    lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the timer notify
    information:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpTimerNotifyProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        quint48 value = proxy.timerValue();
        QKnxByteArray number = proxy.serialNumber();
        quint16 tag = proxy.tag();
        QKnxByteArray code = proxy.messageAuthenticationCode();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpTimerNotifyProxy::QKnxNetIpTimerNotifyProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTimerNotifyProxy::~QKnxNetIpTimerNotifyProxy()
    \internal
*/

/*!
    \fn QKnxNetIpTimerNotifyProxy::QKnxNetIpTimerNotifyProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the timer notify information carried by
    the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpTimerNotifyProxy::QKnxNetIpTimerNotifyProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \note KNXnet/IP timer notify frames currently have a fixed size of \c 36
    bytes.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpTimerNotifyProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::TimerNotify
        && m_frame.size() == 36;
}

/*!
    Returns the timer value from the generic KNXnet/IP secure timer notify frame.
*/
quint48 QKnxNetIpTimerNotifyProxy::timerValue() const
{
    return QKnxUtils::QUint48::fromBytes(m_frame.constData());
}

/*!
    Returns the serial number from the generic KNXnet/IP secure timer notify
    frame.
*/
QKnxByteArray QKnxNetIpTimerNotifyProxy::serialNumber() const
{
    return m_frame.constData().mid(6, 6);
}

/*!
    Returns the message tag from the generic KNXnet/IP secure timer notify frame.
*/
quint16 QKnxNetIpTimerNotifyProxy::messageTag() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData().mid(12));
}

/*!
    Returns the AES128 CCM message authentication code (MAC) from the generic
    KNXnet/IP secure timer notify frame with a fixed size of \c 16 bytes.
*/
QKnxByteArray QKnxNetIpTimerNotifyProxy::messageAuthenticationCode() const
{
    return m_frame.constData().mid(14);
}

/*!
    Returns a builder object to create a KNXnet/IP timer notify frame.
*/
QKnxNetIpTimerNotifyProxy::Builder QKnxNetIpTimerNotifyProxy::builder()
{
    return QKnxNetIpTimerNotifyProxy::Builder();
}


/*!
    \class QKnxNetIpTimerNotifyProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpTimerNotifyProxy

    \brief The QKnxNetIpTimerNotifyProxy::Builder class provides the
    means to create a KNXnet/IP timer notify frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame will be sent during secure KNXnet/IP multicast group
    communication to keep the multicast group member's timer values
    synchronized. The frame is therefore sent to the KNXnet/IP routing
    endpoint on port \c 3671 of the configured routing multicast address.

    The common way to create a timer notify frame is:

    \code
        auto auth = ... // create the full 128 bit CCM-MAC

        auto netIpFrame = QKnxNetIpTimerNotifyProxy::builder()
            .setTimerValue(15021976)
            .setSerialNumber(QKnxByteArray::fromHex("0123456789AB"))
            .setMessageTag(quint16(QRandomGenerator::global()->generate())
            .setMessageAuthenticationCode(auth)
            .create();
    \endcode

    \sa QKnxCryptographicEngine
*/

/*!
    Creates a new empty timer notify frame builder object.
*/
QKnxNetIpTimerNotifyProxy::Builder::Builder()
    : d_ptr(new QKnxNetIpTimerNotifyBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTimerNotifyProxy::Builder::~Builder() = default;

/*!
    Sets the timer value to \a timerValue and returns a reference to the
    builder.

    \note The size of a timer value is limited to 48 bits, so the maximum
    number can be \c 281474976710655. Passing a larger value will result in
    creating an invalid frame.
*/
QKnxNetIpTimerNotifyProxy::Builder &
    QKnxNetIpTimerNotifyProxy::Builder::setTimerValue(quint48 timerValue)
{
    d_ptr->m_timer = timerValue;
    return *this;
}

/*!
    Sets the serial number to \a serialNumber and returns a reference to the
    builder.

    \note The serial number must contain exactly 6 bytes.
*/
QKnxNetIpTimerNotifyProxy::Builder &
    QKnxNetIpTimerNotifyProxy::Builder::setSerialNumber(const QKnxByteArray &serialNumber)
{
    d_ptr->m_serial = serialNumber;
    return *this;
}

/*!
    Sets the message tag of the generic KNXnet/IP timer notify frame to \a tag
    and returns a reference to the builder.

    In case of a periodic or initial notify the tag contains a random value. In
    case of an update notify this is the value of the outdated frame triggering
    the update.
*/
QKnxNetIpTimerNotifyProxy::Builder &QKnxNetIpTimerNotifyProxy::Builder::setMessageTag(quint16 tag)
{
    d_ptr->m_tag = tag;
    return *this;
}

/*!
    Sets the AES128 CCM message authentication code (MAC) of the generic
    KNXnet/IP timer notify frame to \a data and returns a reference to the
    builder. The MAC has a fixed size of \c 16 bytes.
*/
QKnxNetIpTimerNotifyProxy::Builder &
    QKnxNetIpTimerNotifyProxy::Builder::setMessageAuthenticationCode(const QKnxByteArray &data)
{
    d_ptr->m_authCode = data;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP timer notify frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpTimerNotifyProxy::Builder::create() const
{
    if (d_ptr->m_timer <= Q_UINT48_MAX && d_ptr->m_serial.size() == 6 && d_ptr->m_tag >= 0
        && d_ptr->m_authCode.size() == 16) {
            return { QKnxNetIp::ServiceType::TimerNotify, QKnxUtils::QUint48::bytes(d_ptr->m_timer)
                + d_ptr->m_serial + QKnxUtils::QUint16::bytes(d_ptr->m_tag) + d_ptr->m_authCode };
    }
    return { QKnxNetIp::ServiceType::TimerNotify };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTimerNotifyProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTimerNotifyProxy::Builder &
    QKnxNetIpTimerNotifyProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


/*!
    \class QKnxNetIpTimerNotifyProxy::SecureBuilder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpTimerNotifyProxy

    \brief The QKnxNetIpTimerNotifyProxy::SecureBuilder class provides the
    means to create a KNXnet/IP timer notify frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    \note To use this class OpenSSL must be supported on your target system.

    This frame will be sent during secure KNXnet/IP multicast group
    communication to keep the multicast group member's timer values
    synchronized. The frame is therefore sent to the KNXnet/IP routing
    endpoint on port \c 3671 of the configured routing multicast address.

    The common way to create a timer notify frame is:

    \code
        auto sessionId = ...
        auto backboneKey = ... // the backbone key used

        auto netIpFrame = QKnxNetIpTimerNotifyProxy::builder()
            .setTimerValue(15021976)
            .setSerialNumber(QKnxByteArray::fromHex("0123456789AB"))
            .setMessageTag(quint16(QRandomGenerator::global()->generate())
            .create(backboneKey, sessionId);
    \endcode

    \sa QKnxCryptographicEngine
*/

/*!
    Creates a new empty timer notify frame builder object.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder::SecureBuilder()
    : d_ptr(new QKnxNetIpTimerNotifyBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder::~SecureBuilder() = default;

/*!
    Sets the timer value to \a timerValue and returns a reference to the
    builder.

    \note The size of a timer value is limited to 48 bits, so the maximum
    number can be \c 281474976710655. Passing a larger value will result in
    creating an invalid frame.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder &
    QKnxNetIpTimerNotifyProxy::SecureBuilder::setTimerValue(quint48 timerValue)
{
    d_ptr->m_timer = timerValue;
    return *this;
}

/*!
    Sets the serial number to \a serialNumber and returns a reference to the
    builder.

    \note The serial number must contain exactly 6 bytes.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder &
    QKnxNetIpTimerNotifyProxy::SecureBuilder::setSerialNumber(const QKnxByteArray &serialNumber)
{
    d_ptr->m_serial = serialNumber;
    return *this;
}

/*!
    Sets the message tag of the generic KNXnet/IP timer notify frame to \a tag
    and returns a reference to the builder.

    In case of a periodic or initial notify the tag contains a random value. In
    case of an update notify this is the value of the outdated frame triggering
    the update.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder &
    QKnxNetIpTimerNotifyProxy::SecureBuilder::setMessageTag(quint16 tag)
{
    d_ptr->m_tag = tag;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP timer notify frame.

    The function computes the AES128 CCM message authentication code (MAC)
    with the given backbone key \a backboneKey and the session ID \a ssid and
    appends it to the newly created frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame
    QKnxNetIpTimerNotifyProxy::SecureBuilder::create(const QKnxByteArray &backboneKey, quint16 ssid) const
{
#if QT_CONFIG(opensslv11)
    if (d_ptr->m_timer > Q_UINT48_MAX || d_ptr->m_serial.size() != 6 || d_ptr->m_tag < 0)
        return { QKnxNetIp::ServiceType::TimerNotify };

    auto builder = QKnxNetIpTimerNotifyProxy::builder();
    auto frame = builder
        .setTimerValue(d_ptr->m_timer)
        .setSerialNumber(d_ptr->m_serial)
        .setMessageTag(d_ptr->m_tag)
        .setMessageAuthenticationCode(QKnxByteArray(16, 0x00)) // dummy MAC to get a proper header
        .create();

    auto mac = QKnxCryptographicEngine::computeMessageAuthenticationCode(backboneKey,
        frame.header(), ssid, {}, d_ptr->m_timer, d_ptr->m_serial, d_ptr->m_tag);

    mac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(backboneKey, mac,
        d_ptr->m_timer, d_ptr->m_serial, d_ptr->m_tag);

    return builder.setMessageAuthenticationCode(mac).create();
#else
    Q_UNUSED(backboneKey)
    Q_UNUSED(ssid)
    return { QKnxNetIp::ServiceType::TimerNotify };
#endif
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder::SecureBuilder(const SecureBuilder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder &
    QKnxNetIpTimerNotifyProxy::SecureBuilder::operator=(const SecureBuilder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Returns a secure builder object to create a KNXnet/IP timer notify frame.
*/
QKnxNetIpTimerNotifyProxy::SecureBuilder QKnxNetIpTimerNotifyProxy::secureBuilder()
{
    return QKnxNetIpTimerNotifyProxy::SecureBuilder();
}

QT_END_NAMESPACE
