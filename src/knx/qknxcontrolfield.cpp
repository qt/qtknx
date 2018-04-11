/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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
    \brief The QKnxControlField class represents an 8-bit KNX control field.

    A KNX frame contains several fields, one of which might be the control
    field. The control field must specify at least the \l FrameFormat, the
    \l Repeat flag, and the frame \l Priority. In addition, it may specify
    how widely the frame is \l Broadcast, and whether acknowledgment
    (\l Acknowledge) or confirmation (\l Confirm) is requested for the
    transmission or reception of the frame.

    \sa builder()
*/

/*!
    \enum QKnxControlField::FrameFormat

    This enum type specifies the frame format that is used for transmission or
    reception of a frame.

    \value Extended
           The frame has extended format.
    \value Standard
           The frame has standard format.
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
           System priority is used.
    \value Normal
           Normal priority is used.
    \value Urgent
           High priority is used.
    \value Low
           Low priority is used.
*/

/*!
    \enum QKnxControlField::Acknowledge

    This enum type holds whether a Layer 2 acknowledge is requested for an
    \c {L_Data.req} frame. This is not valid for all media.

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

    Sets the frame format to \a type.
*/

/*!
    \fn QKnxControlField::setRepeat(QKnxControlField::Repeat repeat)

    Sets the repetition flag to \a repeat.
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
    \fn QKnxControlField::setConfirm(QKnxControlField::Confirm confirm)

    Sets the confirm flag to \a confirm.
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
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar(QLatin1Char('0'))
        << field.bytes();
    return debug;
}


/*!
    \class QKnxControlField::Builder

    \inmodule QtKnx
    \brief The QKnxControlField::Builder class creates a KNX control field with
    some default values set.

    TODO: Write some useful text with the default values
        QKnxControlField::FrameFormat::Standard
        QKnxControlField::Repeat::DoNotRepeat
        QKnxControlField::Broadcast::Domain
        QKnxControlField::Priority::Low
        QKnxControlField::Acknowledge::NotRequested
        QKnxControlField::Confirm::NoError

    Most of the time the following code will produce the right control field
    for group value read or group value write frames.

    Example:
    \code
        auto ctrl = QKnxControlField::builder.create();
    \endcode

    For more advanced usages some flags can be modified:
    \code
        // setup a control field for used for unicast or broadcast with
        // higher priority

        auto ctrl = QKnxControlField::builder
            .setPriority(QKnxControlField::Priority::System)
            .create();
    \endcode
*/

/*!
    Sets the frame format to \a type and returns a reference to the builder.
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
    Creates and returns a QKnxControlField.
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
