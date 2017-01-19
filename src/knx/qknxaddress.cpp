/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxaddress.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxAddress

    \inmodule QtKnx
    \brief The QKnxAddress class represents a 16 bit KNX address value.

    KNX is a fully distributed network, which accommodates up to 65,536 devices
    in a 16 bit individual address space. Concrete address types are \l Group
    and \l Individual.
*/

/*!
    \enum QKnxAddress::Type
    This enum describes the supported KNX address types.

    \value Group        The Group address shall be a 2 byte value and does not
                        need to be unique. A device may have more than one Group
                        address.
    \value Individual   The Individual address shall be a 2 byte value and
                        needs to be unique within a KNX installation.
*/

/*!
    \enum QKnxAddress::Notation
    This enum describes the supported KNX address human-readable representation.

    \value TwoLevel     The visual representation will be main group and sub
                        group, separated by a forward slash.
    \value ThreeLevel   The visual representation for \l Group addresses will
                        be main, middle and sub group separated by a forward
                        slash, while \l Individual addresses will be formated
                        as area, line and sequential number separated by a dot.

    \note Only \l Group addresses support 2-level notation as of now.
*/

/*!
    Returns the address type.
*/
QKnxAddress::Type QKnxAddress::type() const
{
    return m_type;
}

/*!
    Returns the address notation.
*/
QKnxAddress::Notation QKnxAddress::notation() const
{
    return m_notation;
}

/*!
    \fn QKnxAddress::QKnxAddress()

    Constructs an new, invalid KNX address object. The formatting is set to
    3-level notation.
*/

/*!
    Creates a KNX address from a 16 Bit \a address value. The type of the
    address is specified by \a type. The value of \a address should be in the
    range \c 0 to \c 65535. Hexadecimal, octal and decimal notation are
    supported. The formatting is set to 3-level notation.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, quint16 address)
{
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = address;
}

/*!
    Creates a KNX address from a string \a address representation. The type of
    the address is specified by \a type. Depending on parsed sections, the
    formatting is set to 2 or 3-level notation. If the address or parts begin
    with "0x", hexadecimal notation is assumed; if the address begins with "0",
    octal notation is assumed; otherwise decimal notation.

    QKnxAddress can be in one of the following formats:
    \list
        \li 2-level notation, like 1/2000, for \l Group addresses.
        \li 3-level notation, like 1/1/1 for \l Group or 1.1.1 for \l Individual
            addresses.
        \li Number in the range 0 to 65535 without separator for \l Group and
            \l Individual addresses.
    \endlist

    \note For \l Group address 2-level notation, the value of main group should
    be in the range \c 0 to \c 31 and the sub group value in the range \c 0
    to \c 2047.

    \note For \l Group address 3-level notation, the value of main group should
    be in the range \c 0 to \c 31, the value of middle group in the range \c 0
    to \c 7 and the sub group value in the range \c 0 to \c 255.

    \note For \l Individual address 3-level notation, the value of area should
    be in the range \c 0 to \c 15, the value of line in the range \c 0 to \c 15
    and the sequential number value in the range \c 0 to \c 255.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QString &address)
{
    const bool dots = address.contains(QLatin1Char('.'));
    const bool slashes = address.contains(QLatin1Char('/'));
    if (dots && slashes)
        return;

    QVector<quint16> sections;
    const auto convert = [&sections](const QVector<QStringRef> &in) -> bool {
        foreach (const QStringRef &section, in) {
            bool ok = false;
            const quint16 value = section.toUShort(&ok, 0);
            if (!ok)
                return false;
            sections.append(value);
        }
        return in.size() == sections.size();
    };

    if (slashes && type == QKnxAddress::Type::Group) {
        if (!convert(address.splitRef(QLatin1Char('/'))))
            return;
        if (sections.count() == 2)
            *this = QKnxAddress(type, sections[0], nullptr, sections[1]);
        if (sections.count() == 3)
            *this = QKnxAddress(type, sections[0], &sections[1], sections[2]);
    }
    if (dots && type == QKnxAddress::Type::Individual) {
        if (convert(address.splitRef(QLatin1Char('.'))) && sections.count() == 3)
            *this = QKnxAddress(type, sections[0], &sections[1], sections[2]);
    }
    if (type == QKnxAddress::Type::Group || type == QKnxAddress::Type::Individual) {
        if (convert({ address.midRef(0) }) && sections.count() == 1) {
            m_type = type;
            m_address = sections[0];
        }
    }
}

/*!
    Creates a KNX address from the first two bytes of the \a address byte
    array. The type of the address is specified by \a type. Hexadecimal, octal
    and decimal notation are supported.

    \note The byte array must contain at least two elements.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QByteArray &address)
{
    if (address.size() < 2)
        return;
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = quint16((quint8(address[0]) & 0xff) << 8 | quint8(address[1]) & 0xff);
}

/*!
    Creates a KNX address from the first two bytes of the \a address vector.
    The type of the address is specified by \a type. Hexadecimal, octal and
    decimal notation are supported.

    \note The vector must contain at least two elements.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QVector<quint8> &address)
{
    if (address.size() < 2)
        return;
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = quint16((address[0] & 0xff) << 8 | address[1] & 0xff);
}

/*!
    Creates a KNX \l Group address from the 2-level notation. The value of
    \a mainGroup should be in the range \c 0 to \c 31 and the \a subGroup
    value in the range \c 0 to \c 2047. Hexadecimal, octal and decimal
    notation are supported.
*/
QKnxAddress QKnxAddress::createGroup(quint8 mainGroup, quint16 subGroup)
{
    return QKnxAddress(QKnxAddress::Type::Group, mainGroup, nullptr, subGroup);
}

/*!
    Creates a KNX \l Group address from the 3-level notation. The value of
    \a mainGroup should be in the range \c 0 to \c 31, the value of
    \a middleGroup in the range \c 0 to \c 7 and the \a subGroup value in the
    range \c 0 to \c 255. Hexadecimal, octal and decimal notation are supported.
*/
QKnxAddress QKnxAddress::createGroup(quint8 mainGroup, quint8 middleGroup, quint8 subGroup)
{
    return QKnxAddress(QKnxAddress::Type::Group, mainGroup,
        reinterpret_cast<quint16*> (&middleGroup), subGroup);
}

/*!
    Creates a KNX \l Individual address from the 3-level notation. The value of
    \a area should be in the range \c 0 to \c 15, the value of \a line in the
    range \c 0 to \c 15 and the \a sequentialNumber value in the range \c 0 to
    \c 255. Hexadecimal, octal and decimal notation are supported.
*/
QKnxAddress QKnxAddress::createIndividual(quint8 area, quint8 line, quint8 sequentialNumber)
{
    return QKnxAddress(QKnxAddress::Type::Individual, area, reinterpret_cast<quint16*> (&line),
        sequentialNumber);
}

/*!
    Returns \c true if this is a valid \l Group KNX address object and the
    address is set to \c 0x0000; \c false otherwise.
*/
bool QKnxAddress::isBroadcast() const
{
    return (m_type == QKnxAddress::Type::Group) && (m_address == 0x0000);
}

/*!
    \relates QKnxAddress
    \variable QKnxAddress::Group::Broadcast

    Represents a single KNX \l Group address with value \c 0x0000 that is
    reserved for the broadcast address that shall be exclusively used in
    broadcast communication mode.
*/
QKnxAddress QKnxAddress::Group::Broadcast = { QKnxAddress::Type::Group, 0x0000 };

/*!
    \relates QKnxAddress
    \variable QKnxAddress::Individual::Null

    Represents a single KNX \l Individual address with value \c 0x0000.
*/
QKnxAddress QKnxAddress::Individual::Null = { QKnxAddress::Type::Individual, 0x0000 };

/*!
    Returns \c true if this is a valid \l Individual KNX address object and the
    devices sequential number is set to \c 0x00; \c false otherwise.

    Area and line couplers, as well as KNXnet/IP routers (routing) are given
    the sequential number \c 0x00.

    Example: Individual Address 1.5.0 - KNXnet/IP router acting as a line
    coupler, coupling the fifth line with the main line in the first area.
*/
bool QKnxAddress::isCouplerOrRouter() const
{
    return (m_type == QKnxAddress::Type::Individual) && (quint8(m_address) == 0x00);
}

/*!
    Returns \c true if this is a valid \l Individual KNX address object and the
    devices sequential number is set to \c 0xff; \c false otherwise.
*/
bool QKnxAddress::isUnregistered() const
{
    return (m_type == QKnxAddress::Type::Individual) && (quint8(m_address) == 0xff);
}

/*!
    Returns \c true if this is a valid KNX address object; \c false otherwise.
*/
bool QKnxAddress::isValid() const
{
    return (m_type == QKnxAddress::Type::Group || m_type == QKnxAddress::Type::Individual)
        && m_address >= 0;
}

/*!
    Returns the KNX address as a string formatted using the internal stored
    notation. A address of type \l Group is formatted using a forward slash
    as separator, while a address of type \l Individual is formatted using a
    dot as separator.

    \note A \l Individual address is always returned in 3-level notation, a
    \l Group address in 2-level or 3-level notation depending on the how the
    address was created. If the address is invalid, an empty string is returned.
*/
QString QKnxAddress::toString() const
{
    if (m_notation == QKnxAddress::Notation::ThreeLevel) {
        if (m_type == QKnxAddress::Type::Group) {
            return QStringLiteral("%1/%2/%3").arg((m_address >> 11) & 0x1f)
                .arg((m_address >> 8) & 0x07).arg(m_address & 0xff);
        }
        if (m_type == QKnxAddress::Type::Individual) {
            return QStringLiteral("%1.%2.%3").arg(m_address >> 12)
                .arg((m_address >> 8) & 0x0f).arg(m_address & 0xff);
        }
    }
    if (m_notation == QKnxAddress::Notation::TwoLevel && m_type == QKnxAddress::Type::Group)
        return QStringLiteral("%1/%2").arg((m_address >> 11) & 0x1f).arg(m_address & 0x07ff);
    return QString();
}

/*!
    \fn auto QKnxAddress::rawData() const

    Returns a KNX address as a range of bytes if the address is valid;
    otherwise the return value is empty. Only the first two bytes of the return
    value are part of the address.

    \note Only QByteArray and QVector<quint8> are supported as return type.
*/

/*!
    \relates QKnxAddress

    Writes the KNX \a address to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxAddress &address)
{
    QDebugStateSaver _(debug);
    if (address.isValid()) {
        const auto rawData = address.rawData<QVector<quint8>>();
        debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0')
            << rawData[0] << hex << rawData[1];
    } else {
         debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

/*!
    \relates QKnxAddress

    Writes a KNX \a address to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxAddress &address)
{
    if (!address.isValid())
        return out;
    const auto rawData = address.rawData<QByteArray>();
    return out << quint8(rawData[0]) << quint8(rawData[1]);
}


// -- private

/*!
    \internal

    Creates a KNX address from 2 or 3-level notation. The type of the address is
    specified by \a type. Hexadecimal, octal and decimal notation are supported.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, quint16 sec1, quint16 *sec2, quint16 sec3)
{
    const auto checkRange = [type, sec1, sec2, sec3](QKnxAddress::Notation notation) {
        if (type == QKnxAddress::Type::Group) {
            if ((sec1 < 0) && (sec1 > 31))
                return false;
            if (notation == QKnxAddress::Notation::TwoLevel)
                return (sec3 >= 0) && (sec3 <= 2047);
            if (sec2 && (notation == QKnxAddress::Notation::ThreeLevel))
                return (*sec2 >= 0) && (*sec2 <= 7) && (sec3 >= 0) && (sec3 <= 255);
        }
        if (sec2 && (type == QKnxAddress::Type::Individual)) {
            return (sec1 >= 0) && (sec1 <= 15) && (*sec2 >= 0) && (*sec2 <= 15)
                && (sec3 >= 0) && (sec3 <= 255);
        }
        return false;
    };

    if (type == QKnxAddress::Type::Group) {
        if (!sec2) {
            if (checkRange(QKnxAddress::Notation::TwoLevel)) {
                m_type = type;
                m_notation = QKnxAddress::Notation::TwoLevel;
                m_address = quint16(sec1 << 11 | sec3);
            }
        } else {
            if (checkRange(QKnxAddress::Notation::ThreeLevel)) {
                m_type = type;
                m_address = quint16(sec1 << 11 | (*sec2) << 8 | sec3);
            }
        }
    }
    if (type == QKnxAddress::Type::Individual) {
        if (checkRange(QKnxAddress::Notation::ThreeLevel)) {
            m_type = type;
            m_address = quint16(sec1 << 12 | (*sec2) << 8 | sec3);
        }
    }
}

QT_END_NAMESPACE
