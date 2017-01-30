/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxcontrolfield1.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gPriorityMask = 0x0c;

/*!
    Creates a new control field  from a 8 Bit \a data value. Hexadecimal, octal
    and decimal notation are supported.
*/
QKnxControlField1::QKnxControlField1(quint8 data)
    : m_ctrl1(data)
{
}

/*!
    Creates a new control field from the first byte of the \a data byte array.
    Hexadecimal, octal and decimal notation are supported.

    \note The byte array must contain at least one elements.
*/
QKnxControlField1::QKnxControlField1(const QByteArray &data)
{
    if (data.size() > 0)
        m_ctrl1 = quint8(data[0]);
}

/*!
    Creates a new control field from the first byte of the \a data vector.
    Hexadecimal, octal and decimal notation are supported.

    \note The vector must contain at least one elements.
*/
QKnxControlField1::QKnxControlField1(const QVector<quint8> &data)
{
    if (data.size() > 0)
        m_ctrl1 = quint8(data[0]);
}

QKnxControlField1::Priority QKnxControlField1::priority() const
{
    return static_cast<Priority> ((m_ctrl1 & gPriorityMask).to_ulong() >> 2);
}

void QKnxControlField1::setPriority(QKnxControlField1::Priority priority)
{
    m_ctrl1 &= ~gPriorityMask; // clear
    m_ctrl1 |= (static_cast<quint8> (priority) << 2); // set
}

/*!
    Returns the control field's \l Type, \l Repeat, \l Broadcast, \l Priority,
    \l Acknowledge and \l Confirm fields as string. All values are formatted in
    hexadecimal notation.
*/
QString QKnxControlField1::toString() const
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
    \fn quint8 QKnxControlField1::rawData() const

    Returns the first control field as \c quint8.
*/

/*!
    \fn auto QKnxControlField1::rawData() const

    Returns the first control field as vector of bytes.

    \note Only QByteArray and QVector<quint8> are supported as return type.
*/

/*!
    \relates QKnxControlField1

    Writes the KNX cEMI frame's first control \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxControlField1 &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0')
        << field.rawData();
    return debug;
}

/*!
    \relates QKnxControlField1

    Reads the KNX cEMI frame's first control \a field from the \a stream and
    returns a reference to the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxControlField1 &field)
{
    quint8 raw;
    stream >> raw;
    field = QKnxControlField1(raw);

    return stream;
}

/*!
    \relates QKnxControlField1

    Writes the KNX cEMI frame's first control \a field to the \a stream and
    returns a reference to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxControlField1 &field)
{
    return stream << field.rawData();
}

QT_END_NAMESPACE
