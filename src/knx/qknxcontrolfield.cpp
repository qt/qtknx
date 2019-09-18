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

#include "qknxcontrolfield.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gPriorityMask = 0x0c;

/*!
    \class QKnxControlField

    \inmodule QtKnx
    \ingroup qtknx-general-classes

    \brief The QKnxControlField class represents an 8-bit KNX control field.

    A KNX frame contains several fields, one of which might be the control
    field. The control field must specify at least the \l FrameFormat, the
    \l Repeat flag, and the frame \l Priority. In addition, it may specify
    how widely the frame is \l Broadcast, and whether acknowledgment
    (\l Acknowledge) or confirmation (\l Confirm) is requested for the
    transmission or reception of the frame.

    \section2 Selecting Frame Format

    The control field selects the standard or extended frame format for the data
    link layer and includes information for the used extended frame type. The
    standard format should be preferred and the extended format should only be
    used when the encoding capabilities of the standard format are insufficient.

    \section2 Selecting Frame Priority

    The priority of frames should be selected carefully to ensure fair bus
    access. The maximum priority that is allowed for run-time communication is
    \e normal. The \e system priority is reserved for system configuration
    and management procedures.

    The \e urgent priority is only allowed for implementations of datapoints of
    functional blocks or channels for which this is specified explicitly. In a
    network, the frame traffic using urgent priority shall not exceed five
    percent of the total traffic.

    \sa builder(), QKnxExtendedControlField
*/

/*!
    \enum QKnxControlField::FrameFormat

    This enum type selects the standard or extended frame format for the data
    link layer and includes information for the used extended frame type.

    \value Extended
           This parameter is mapped to
           \l QKnxExtendedControlField::ExtendedFrameFormat. The extended frame
           format is used and the frame type is selected by the frame type
           parameter bit.
    \value Standard
           The standard frame format is used.
*/

/*!
    \enum QKnxControlField::Repeat

    This enum type holds whether the frame transmission is repeated.
    If one of the addressed bus devices has returned a negative acknowledgment,
    the repeat flag is set to \c Repeat.

    \value Repeat
           Frame transmission is repeated.
    \value DoNotRepeat
           Frame transmission is not repeated.
*/

/*!
    \enum QKnxControlField::Broadcast

    This enum type specifies whether the frame is transmitted using system
    broadcast communication mode or broadcast communication mode.

    \value System
           The frame is transmitted using system broadcast communication mode.
    \value Domain
           The frame is transmitted using broadcast communication mode.
*/

/*!
    \enum QKnxControlField::Priority

    This enum type holds the priority used for the transmission or reception of
    the frame.

    \value System
           System priority is used. It is reserved for high priority frames, as
           well as system configuration and management procedures.
    \value Normal
           Normal priority is used. It is the default priority for short frames.
    \value Urgent
           High priority is used. It is reserved for urgent frames.
    \value Low
           Low priority is used. It is mandatory for long frames, burst traffic,
           and so on.
*/

/*!
    \enum QKnxControlField::Acknowledge

    This enum type holds whether a Layer 2 acknowledge is requested for an
    \c {L_Data.req} frame that is used in client-to-server communiction on
    the KNX data link layer. This is not valid for all media.

    \value NotRequested
           Acknowledgment is not requested.
    \value Requested
           Acknowledgment is requested.

    \sa QKnxLinkLayerFrame::MessageCode
*/

/*!
    \enum QKnxControlField::Confirm

    This enum type holds whether there is an error in the transmitted frame.

    \value NoError
           No errors found.
    \value Error
           Errors found.
*/

/*!
    \fn QKnxControlField::frameFormat() const

    Returns the frame format.
*/

/*!
    \fn QKnxControlField::repeat() const

    Returns whether frame transmission is repeated.
*/

/*!
    \fn QKnxControlField::broadcast() const

    Returns whether the frame is transmitted using system broadcast
    communication mode or broadcast communication mode.
*/

/*!
    \fn QKnxControlField::acknowledge() const

    Returns whether a Layer 2 acknowledge is requested for an \c {L_Data.req}
    frame.

    \sa QKnxLinkLayerFrame::MessageCode
*/

/*!
    \fn QKnxControlField::confirm() const

    For an \c {L_Data.con} frame, returns \c true if there is an error in the
    transmitted frame; otherwise returns \c false.

    \sa QKnxLinkLayerFrame::MessageCode
*/

/*!
    \fn QKnxControlField::setFrameFormat(QKnxControlField::FrameFormat type)

    Sets the frame type bit of the the frame format parameter to \a type.
*/

/*!
    \fn QKnxControlField::setRepeat(QKnxControlField::Repeat rep)

    Sets the repetition flag to \a rep.
*/

/*!
    \fn QKnxControlField::setBroadcast(QKnxControlField::Broadcast bcst)

    Sets the broadcast flag to \a bcst.
*/

/*!
    \fn QKnxControlField::setAcknowledge(QKnxControlField::Acknowledge ack)

    Sets whether acknowledgment for an \c {L_Data.req} frame is requested to
    \a ack.

    \sa QKnxLinkLayerFrame::MessageCode
*/

/*!
    \fn QKnxControlField::setConfirm(QKnxControlField::Confirm conf)

    Sets the confirm flag to \a conf.
*/

/*!
    \fn quint8 QKnxControlField::byte() const

    Returns the control field as single byte.
*/

/*!
    \fn quint8 QKnxControlField::size() const

    Returns the number of bytes in the control field.
*/

/*!
    \fn QKnxControlField::QKnxControlField()

    Creates a KNX control field.
*/

/*!
    Creates a new control field from an 8-bit \a data value.
*/
QKnxControlField::QKnxControlField(quint8 data)
    : m_ctrl1(data)
{}

/*!
    Creates a new control field from the first byte of the \a data byte array.

    \note The byte array must contain at least one element.
*/
QKnxControlField::QKnxControlField(const QKnxByteArray &data)
{
    if (data.size() > 0)
        m_ctrl1 = data.at(0);
}

/*!
    Returns the priority that is used for the transmission or reception of
    the frame.
*/
QKnxControlField::Priority QKnxControlField::priority() const
{
    return static_cast<Priority> ((m_ctrl1 & gPriorityMask).to_ulong() >> 2);
}

/*!
    Sets the \a priority that is used for the transmission or reception of
    the frame.
*/
void QKnxControlField::setPriority(QKnxControlField::Priority priority)
{
    m_ctrl1 &= ~gPriorityMask; // clear
    m_ctrl1 |= (static_cast<quint8> (priority) << 2); // set
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxControlField::operator==(const QKnxControlField &other) const
{
    return m_ctrl1 == other.m_ctrl1;
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxControlField::operator!=(const QKnxControlField &other) const
{
    return !operator==(other);
}

/*!
    Returns a builder to create a KNX control field object.
*/
QKnxControlField::Builder QKnxControlField::builder()
{
    return Builder();
}

/*!
    \fn quint8 QKnxControlField::bytes() const

    Returns the control field as a range of bytes.
*/

/*!
    \relates QKnxControlField

    Writes the control field \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxControlField &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << Qt::hex << qSetFieldWidth(2) << qSetPadChar(QLatin1Char('0'))
        << field.bytes();
    return debug;
}


/*!
    \class QKnxControlField::Builder

    \inmodule QtKnx
    \inheaderfile QKnxControlField

    \brief The QKnxControlField::Builder class creates a KNX control field with
    some default values set.

    The control field of a KNX frame must specify at least the \l FrameFormat,
    the \l Repeat flag, and the frame \l Priority. In addition, it may specify
    how widely the frame is \l Broadcast, and whether acknowledgment
    (\l Acknowledge) or confirmation (\l Confirm) is requested for the
    transmission or reception of the frame.

    The default values produce control fields that are suitable for multicast
    frames that read or write group values.

    The control field selects the standard or extended frame format for the data
    link layer and includes information for the used extended frame type. By
    default, the frame format is set to \l Standard, which is the preferred
    format for short frames.

    The repeat flag determines whether the frame transmission is repeated if the
    status of an acknowledgment frame received from a bus device indicates that
    an error has occurred. By default, the repeat flag is set to \l DoNotRepeat.

    The broadcast flag selects the communication mode used for transmitting
    the frame. By default, it is set to \l Domain to transmit frames using
    broadcast communication mode.

    The priority of frames should be selected carefully to ensure fair bus
    access. By default, the priority of a frame is set to \l Low, which is
    mandatory for long frames. The maximum priority that is allowed for
    run-time communication is \l Normal.

    The acknowledge flag determines whether a Layer 2 acknowledge is requested
    for an \c {L_Data.req} frame that is used in client-to-server communiction
    on the KNX data link layer. By default, it is set to \l NotRequested.

    The confirm flag indicates whether there is an error in the transmitted
    frame. By default, it is set to \l NoError.

    The following sample code creates a control field using the default values:

    \code
        auto ctrl = QKnxControlField::builder.create();
    \endcode

    Some flags can be modified for more advanced use, such as setting up a
    control field used for unicast or broadcast with a higher priority:

    \code
        auto ctrl = QKnxControlField::builder
            .setPriority(QKnxControlField::Priority::System)
            .create();
    \endcode
*/

/*!
    Sets the frame type bit of the frame format parameter to \a type and returns
    a reference to the builder.
*/
QKnxControlField::Builder &
    QKnxControlField::Builder::setFrameFormat(QKnxControlField::FrameFormat type)
{
    m_frameFormat = type;
    return *this;
}

/*!
    Sets the repeat flag to \a repeat and returns a reference to the builder.
*/
QKnxControlField::Builder &QKnxControlField::Builder::setRepeat(QKnxControlField::Repeat repeat)
{
    m_repeat = repeat;
    return *this;
}

/*!
    Sets the broadcast flag to \a broadcast and returns a reference to the builder.
*/
QKnxControlField::Builder &
    QKnxControlField::Builder::setBroadcast(QKnxControlField::Broadcast broadcast)
{
    m_broad = broadcast;
    return *this;
}

/*!
    Sets the priority flag to \a priority and returns a reference to the builder.
*/
QKnxControlField::Builder &
    QKnxControlField::Builder::setPriority(QKnxControlField::Priority priority)
{
    m_priority = priority;
    return *this;
}

/*!
    Sets the acknowledge flag to \a acknowledge and returns a reference to the builder.
*/
QKnxControlField::Builder &
    QKnxControlField::Builder::setAcknowledge(QKnxControlField::Acknowledge acknowledge)
{
    m_acknowledge = acknowledge;
    return *this;
}

/*!
    Sets the confirm flag to \a errorStatus and returns a reference to the builder.
*/
QKnxControlField::Builder &
    QKnxControlField::Builder::setConfirm(QKnxControlField::Confirm errorStatus)
{
    m_errorStatus = errorStatus;
    return *this;
}

/*!
    Creates and returns a KNX control field.
*/
QKnxControlField QKnxControlField::Builder::create() const
{
    QKnxControlField ctrlField;
    ctrlField.setFrameFormat(m_frameFormat);
    ctrlField.setRepeat(m_repeat);
    ctrlField.setBroadcast(m_broad);
    ctrlField.setPriority(m_priority);
    ctrlField.setAcknowledge(m_acknowledge);
    ctrlField.setConfirm(m_errorStatus);
    return ctrlField;
}

QT_END_NAMESPACE
