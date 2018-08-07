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

#include "qknxnetiptimernotify.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpTimerNotifyProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpTimerNotifyProxy class provides the means to introspect
    timer notify data inside the generic \l QKnxNetIpFrame class and to create
    a KNXnet/IP timer notify frame from provided data.

    TODO: Add more documentation. AN159 paragraph 2.2.2.4 TIMER_NOTIFY

    \note When using QKnxNetIpTimerNotifyProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the proxy on
    all code paths, lest the proxy ends up referencing deleted data.

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
    Returns the AES128 CCM message authentication code from the generic
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

    TODO: Add more documentation. AN159 paragraph 2.2.3.9 SESSION_STATUS

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
*/

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
    m_timer = timerValue;
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
    m_serial = serialNumber;
    return *this;
}

/*!
    Sets the message tag of the generic KNXnet/IP timer notify frame to \a tag
    and returns a reference to builder.

    In case of a periodic or initial notify the tag contains a random value. In
    case of an update notify this is the value of the outdated frame triggering
    the update.
*/
QKnxNetIpTimerNotifyProxy::Builder &QKnxNetIpTimerNotifyProxy::Builder::setMessageTag(quint16 tag)
{
    m_tag = tag;
    return *this;
}

/*!
    Sets the AES128 CCM message authentication code of the generic KNXnet/IP
    timer notify frame to \a data and returns a reference to builder. The
    message authentication code has a fixed size of \c 16 bytes.
*/
QKnxNetIpTimerNotifyProxy::Builder &
    QKnxNetIpTimerNotifyProxy::Builder::setMessageAuthenticationCode(const QKnxByteArray &data)
{
    m_authCode = data;
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
    if (m_timer <= Q_UINT48_MAX && m_serial.size() == 6 && m_tag >= 0 && m_authCode.size() == 16) {
        return { QKnxNetIp::ServiceType::TimerNotify, QKnxUtils::QUint48::bytes(m_timer)
            + m_serial + QKnxUtils::QUint16::bytes(m_tag) + m_authCode };
    }
    return { QKnxNetIp::ServiceType::TimerNotify };
}

QT_END_NAMESPACE
