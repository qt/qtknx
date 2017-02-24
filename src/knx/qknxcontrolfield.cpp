/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxcontrolfield.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gPriorityMask = 0x0c;

/*!
    \class QKnxControlField

    \inmodule QtKnx
    \brief The QKnxControlField class represents a 8 bit KNX control field.

    A KNX frame is made up out of several field, one might be the Control
    field. The Control field shall contain at least the Frame Type, the Repeat
    flag and the frame Priority.
*/

/*!
    Creates a new control field  from a 8 Bit \a data value. Hexadecimal, octal
    and decimal notation are supported.
*/
QKnxControlField::QKnxControlField(quint8 data)
    : m_ctrl1(data)
{}

/*!
    Creates a new control field from the first byte of the \a data byte array.
    Hexadecimal, octal and decimal notation are supported.

    \note The byte array must contain at least one elements.
*/
QKnxControlField::QKnxControlField(const QByteArray &data)
{
    if (data.size() > 0)
        m_ctrl1 = quint8(data[0]);
}

/*!
    Creates a new control field from the first byte of the \a data vector.
    Hexadecimal, octal and decimal notation are supported.

    \note The vector must contain at least one elements.
*/
QKnxControlField::QKnxControlField(const QVector<quint8> &data)
{
    if (data.size() > 0)
        m_ctrl1 = quint8(data[0]);
}

/*!
    Returns the priority that shall be used for transmission or reception of
    the frame.
*/
QKnxControlField::Priority QKnxControlField::priority() const
{
    return static_cast<Priority> ((m_ctrl1 & gPriorityMask).to_ulong() >> 2);
}

/*!
    Sets the priority that shall be used for transmission or reception of the
    frame.
*/
void QKnxControlField::setPriority(QKnxControlField::Priority priority)
{
    m_ctrl1 &= ~gPriorityMask; // clear
    m_ctrl1 |= (static_cast<quint8> (priority) << 2); // set
}

/*!
    Returns the control field's \l FrameType, \l Repeat, \l Broadcast,
    \l Priority, \l Acknowledge and \l Confirm fields as string. All values are
    formatted in hexadecimal notation.
*/
QString QKnxControlField::toString() const
{
    return QStringLiteral("Type { 0x%1 }, Repeat { 0x%2 }, Broadcast { 0x%3 }, Priority { 0x%4 }, "
            "Acknowledge { 0x%5 }, Confirm { 0x%6 }")
        .arg(m_ctrl1[7], 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[6], 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[5], 2, 16, QLatin1Char('0'))
        .arg(quint8(priority()), 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[1], 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[0], 2, 16, QLatin1Char('0'));
}

/*!
    \fn auto QKnxControlField::rawData() const

    Returns the control field as byte or range of bytes.

    \note Only \c quint8, \c QByteArray and \c QVector<quint8> are supported as
    return type.

    \code
        QKnxControlField ctrl;

        quint8 byte = ctrl.rawData();
        auto byteArray = ctrl.rawData<QByteArray>();
        auto vector = ctrl.rawData<QVector<quint8>>();
    \endcode
*/

/*!
    \relates QKnxControlField

    Writes the control \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxControlField &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0')
        << field.rawData();
    return debug;
}

/*!
    \relates QKnxControlField

    Reads the control \a field from the \a stream and returns a reference to
    the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxControlField &field)
{
    quint8 raw;
    stream >> raw;
    field = QKnxControlField(raw);

    return stream;
}

/*!
    \relates QKnxControlField

    Writes the control \a field to the \a stream and returns a reference to the
    \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxControlField &field)
{
    return stream << field.rawData();
}

QT_END_NAMESPACE
