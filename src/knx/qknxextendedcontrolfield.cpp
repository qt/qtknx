/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxextendedcontrolfield.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gHopCountMask = 0x70;
const std::bitset<8> gExtFormatMask = 0x0f;

/*!
    \class QKnxExtendedControlField

    \inmodule QtKnx
    \brief The QKnxExtendedControlField class represents a 8 bit extended KNX
    control field.

    A KNX frame is made up out of several field, one might be the extended
    Control field. The extended Control field shall contain the Destination
    Address Type, Hop Count and Extended Frame Format parameter.
*/

/*!
    Creates a new extended control field from a 8 Bit \a data value.
    Hexadecimal, octal and decimal notation are supported.
*/
QKnxExtendedControlField::QKnxExtendedControlField(quint8 data)
    : m_ctrl2(data)
{}

/*!
    Creates a new extended control field from the first byte of the \a data
    byte array. Hexadecimal, octal and decimal notation are supported.

    \note The byte array must contain at least one elements.
*/
QKnxExtendedControlField::QKnxExtendedControlField(const QByteArray &data)
{
    if (data.size() > 0)
        m_ctrl2 = quint8(data[0]);
}

/*!
    Creates a new extended control field from the first byte of the \a data
    vector. Hexadecimal, octal and decimal notation are supported.

    \note The vector must contain at least one elements.
*/
QKnxExtendedControlField::QKnxExtendedControlField(const QVector<quint8> &data)
{
    if (data.size() > 0)
        m_ctrl2 = quint8(data[0]);
}

QKnxExtendedControlField::DestinationAddressType QKnxExtendedControlField::destinationAddressType() const
{
    return static_cast<QKnxExtendedControlField::DestinationAddressType> (m_ctrl2[7]);
}

void QKnxExtendedControlField::setDestinationAddressType(DestinationAddressType address)
{
    m_ctrl2[7] = static_cast<int> (address);
}

quint8 QKnxExtendedControlField::hopCount() const
{
    return static_cast<quint8> ((m_ctrl2 & gHopCountMask).to_ulong() >> 4);
}

void QKnxExtendedControlField::setHopCount(quint8 hopCount)
{
    m_ctrl2 &= ~gHopCountMask; // clear
    m_ctrl2 |= (hopCount << 4); // set
}

QKnxExtendedControlField::ExtendedFrameFormat QKnxExtendedControlField::format() const
{
    return static_cast<ExtendedFrameFormat> ((m_ctrl2 & gExtFormatMask).to_ulong());
}

void QKnxExtendedControlField::setFormat(QKnxExtendedControlField::ExtendedFrameFormat format)
{
    m_ctrl2 &= ~gExtFormatMask; // clear
    if (format == QKnxExtendedControlField::ExtendedFrameFormat::Lte)
        m_ctrl2 |= gExtFormatMask; // set
}

/*!
    Returns the extended control field's \l DestinationAddressType, \l hopCount
    and \l ExtendedFrameFormat fields as string. All values are formatted in
    hexadecimal notation.
*/
QString QKnxExtendedControlField::toString() const
{
    return QStringLiteral("Destination address { 0x%1 }, Hop count { 0x%2 }, "
        "Extended frame format { 0x%3 }")
        .arg(m_ctrl2[7], 2, 16, QLatin1Char('0'))
        .arg(hopCount(), 2, 16, QLatin1Char('0'))
        .arg(quint8(format()), 2, 16, QLatin1Char('0'));
}

/*!
    \fn auto QKnxExtendedControlField::rawData() const

    Returns the extended control field as byte or range of bytes.

    \note Only \c quint8, \c QByteArray and \c QVector<quint8> are supported as
    return type.

    \code
        QKnxExtendedControlField ectrl;

        quint8 byte = ectrl.rawData();
        auto byteArray = ectrl.rawData<QByteArray>();
        auto vector = ectrl.rawData<QVector<quint8>>();
    \endcode
*/

/*!
    \relates QKnxExtendedControlField

    Writes the extended control \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxExtendedControlField &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0')
        << field.rawData();
    return debug;
}

/*!
    \relates QKnxExtendedControlField

    Reads the extended control \a field from the \a stream and returns a
    reference to the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxExtendedControlField &field)
{
    quint8 raw;
    stream >> raw;
    field = QKnxExtendedControlField(raw);

    return stream;
}

/*!
    \relates QKnxExtendedControlField

    Writes the extended control \a field to the \a stream and returns a
    reference to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxExtendedControlField &field)
{
    return stream << field.rawData();
}

QT_END_NAMESPACE
