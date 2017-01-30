/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxcontrolfield2.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gHopCountMask = 0x70;
const std::bitset<8> gExtFormatMask = 0x0f;

/*!
    Creates a new control field  from a 8 Bit \a data value. Hexadecimal, octal
    and decimal notation are supported.
*/
QKnxControlField2::QKnxControlField2(quint8 data)
    : m_ctrl2(data)
{
}

/*!
    Creates a new control field from the first byte of the \a data byte array.
    Hexadecimal, octal and decimal notation are supported.

    \note The byte array must contain at least one elements.
*/
QKnxControlField2::QKnxControlField2(const QByteArray &data)
{
    if (data.size() > 0)
        m_ctrl2 = quint8(data[0]);
}

/*!
    Creates a new control field from the first byte of the \a data vector.
    Hexadecimal, octal and decimal notation are supported.

    \note The vector must contain at least one elements.
*/
QKnxControlField2::QKnxControlField2(const QVector<quint8> &data)
{
    if (data.size() > 0)
        m_ctrl2 = quint8(data[0]);
}

QKnxControlField2::DestinationAddress QKnxControlField2::destinationAddress() const
{
    return static_cast<QKnxControlField2::DestinationAddress> (m_ctrl2[7]);
}

void QKnxControlField2::setDestinationAddress(QKnxControlField2::DestinationAddress address)
{
    m_ctrl2[7] = static_cast<int> (address);
}

quint8 QKnxControlField2::hopCount() const
{
    return static_cast<quint8> ((m_ctrl2 & gHopCountMask).to_ulong() >> 4);
}

void QKnxControlField2::setHopCount(quint8 hopCount)
{
    m_ctrl2 &= ~gHopCountMask; // clear
    m_ctrl2 |= (hopCount << 4); // set
}

QKnxControlField2::ExtendedFrameFormat QKnxControlField2::format() const
{
    return static_cast<ExtendedFrameFormat> ((m_ctrl2 & gExtFormatMask).to_ulong());
}

void QKnxControlField2::setFormat(QKnxControlField2::ExtendedFrameFormat format)
{
    m_ctrl2 &= ~gExtFormatMask; // clear
    if (format == QKnxControlField2::ExtendedFrameFormat::Lte)
        m_ctrl2 |= gExtFormatMask; // set
}

/*!
    Returns the control field's Destination address type, Hop count and
    Extended frame format fields as string. All values are formatted in
    hexadecimal notation.
*/
QString QKnxControlField2::toString() const
{
    return QStringLiteral("Destination address { 0x%1 }, Hop count { 0x%2 }, "
        "Extended frame format { 0x%3 }")
        .arg(m_ctrl2[7], 2, 16, QLatin1Char('0'))
        .arg(hopCount(), 2, 16, QLatin1Char('0'))
        .arg(quint8(format()), 2, 16, QLatin1Char('0'));
}

/*!
    \fn quint8 QKnxControlField2::rawData() const

    Returns the first control field as \c quint8.
*/

/*!
    \fn auto QKnxControlField2::rawData() const

    Returns the first control field as vector of bytes.

    \note Only \c QByteArray and \c QVector<quint8> are supported as return type.
*/

/*!
    \relates QKnxControlField2

    Writes the KNX cEMI frame's second control \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxControlField2 &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0')
        << field.rawData();
    return debug;
}

/*!
    \relates QKnxControlField2

    Reads the KNX cEMI frame's second control \a field from the \a stream and
    returns a reference to the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxControlField2 &field)
{
    quint8 raw;
    stream >> raw;
    field = QKnxControlField2(raw);

    return stream;
}

/*!
    \relates QKnxControlField2

    Writes the KNX cEMI frame's second control \a field to the \a stream and
    returns a reference to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxControlField2 &field)
{
    return stream << field.rawData();
}

QT_END_NAMESPACE
