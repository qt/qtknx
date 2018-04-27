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

#include "qknxaddress.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxAddress

    \inmodule QtKnx
    \brief The QKnxAddress class represents a 16-bit KNX address value.

    KNX is a fully distributed network, which accommodates up to 65,536 devices
    in a 16-bit individual address space. Concrete address \l {Type}{types}
    are \e {group address} and \e {individual address}.
*/

/*!
    \enum QKnxAddress::Type
    This enum describes the supported KNX address types.

    \value Group        A group address is a 2-byte value that does not need to
                        be unique. A device may have more than one group address.
    \value Individual   An individual address is a 2-byte value that needs to be
                        unique within a KNX installation.
*/

/*!
    \enum QKnxAddress::Notation
    This enum describes the supported human-readable representation of KNX
    addresses.

    \value TwoLevel
        The visual representation will be main group and sub group, separated
        by a forward slash.
    \value ThreeLevel
        The visual representation for \l {Type}{group addresses} will be main,
        middle, and sub group separated by a forward slash, while \l {Type}
        {individual addresses} will be formatted as area, line, and sequential
        number, separated by a dot.

    \note Only group addresses support 2-level notation.
*/

/*!
    \fn QKnxAddress::size() const

    Returns the size of a KNX address.
*/

/*!
    Returns the address type.
*/
QKnxAddress::Type QKnxAddress::type() const
{
    return m_type;
}

/*!
    \fn QKnxAddress::QKnxAddress()

    Constructs an new, invalid KNX address object. The formatting is set to
    3-level notation.
*/

/*!
    Creates a KNX address from a 16-bit \a address value. The type of the
    address is specified by \a type. The value of \a address should be in the
    range \c 0 to \c 65535. The formatting is set to 3-level notation.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, quint16 address)
{
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = address;
}

/*!
    Creates a KNX address from the string \a address representation. The type of
    the address is specified by \a type. Depending on the parsed sections, the
    formatting is set to 2 or 3-level notation.

    A KNX address can be in one of the following formats:
    \list
        \li 2-level notation, such as 1/2000 for \l {Type}{group addresses}.
        \li 3-level notation, such as 1/1/1 for group addresses or 1.1.1
            for \l {Type}{individual addresses}.
        \li Number in the range 0 to 65535 without a separator for group and
            individual addresses.
    \endlist

    \note For group address 2-level notation, the value of the main group
    should be in the range \c 0 to \c 31 and the sub group value in the range
    \c 0 to \c 2047.

    \note For group address 3-level notation, the value of the main group
    should be in the range \c 0 to \c 31, that of the middle group in the range
    \c 0 to \c 7, and that of the sub group in the range \c 0 to \c 255.

    \note For individual address 3-level notation, the value of area should
    be in the range \c 0 to \c 15, the value of line in the range \c 0 to \c 15,
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
        for (const QStringRef &section : qAsConst(in)) {
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
    Creates a KNX address from the first two bytes of the \a address byte array.
    The type of the address is specified by \a type.

    \note The byte array must contain at least two elements.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QKnxByteArray &address)
{
    if (address.size() < 2)
        return;
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = QKnxUtils::QUint16::fromBytes(address);
}

/*!
    Creates a KNX \l {Type}{group address} from the 2-level
    notation. The value of \a mainGroup should be in the range \c 0 to \c 31
    and that of \a subGroup in the range \c 0 to \c 2047.
*/
QKnxAddress QKnxAddress::createGroup(quint8 mainGroup, quint16 subGroup)
{
    return QKnxAddress(QKnxAddress::Type::Group, mainGroup, nullptr, subGroup);
}

/*!
    Creates a KNX \l {Type}{group address} from the 3-level
    notation. The value of \a mainGroup should be in the range \c 0 to \c 31,
    that of \a middleGroup in the range \c 0 to \c 7, and that of \a subGroup
    in the range \c 0 to \c 255.
*/
QKnxAddress QKnxAddress::createGroup(quint8 mainGroup, quint16 middleGroup, quint8 subGroup)
{
    return QKnxAddress(QKnxAddress::Type::Group, mainGroup, &middleGroup, subGroup);
}

/*!
    Creates a KNX \l {Type}{individual address} from the 3-level
    notation. The value of \a area should be in the range \c 0 to \c 15, that
    of \a line in the range \c 0 to \c 15, and that of \a sequentialNumber in
    the range \c 0 to \c 255.
*/
QKnxAddress QKnxAddress::createIndividual(quint8 area, quint16 line, quint8 sequentialNumber)
{
    return QKnxAddress(QKnxAddress::Type::Individual, area, &line, sequentialNumber);
}

/*!
    Returns \c true if this is a valid \l {Type}{group address} object and the
    address is set to \c 0x0000; \c false otherwise.
*/
bool QKnxAddress::isBroadcast() const
{
    return (m_type == QKnxAddress::Type::Group) && (m_address == 0x0000);
}

/*!
    \class QKnxAddress::Group
    \inmodule QtKnx

    \brief The QKnxAddress::Group class represents a KNX group address to be
    used as broadcast address.

    A group address is a 2-byte value that does not need to be unique. A device
    may have more than one group address.

    \sa QKnxAddress::Individual, QKnxAddress::Notation
*/

/*!
    \relates QKnxAddress
    \variable QKnxAddress::Group::Broadcast

    Represents a single KNX group address with the value \c 0x0000 that is
    reserved for the broadcast address that shall be exclusively used in the
    broadcast communication mode.
*/
QKnxAddress QKnxAddress::Group::Broadcast = { QKnxAddress::Type::Group, 0x0000 };

/*!
    \class QKnxAddress::Individual
    \inmodule QtKnx

    \brief The QKnxAddress::Individual class represents a KNX individual
    address for an unregistered KNX device.

    An individual address is a 2-byte value that needs to be unique within a KNX
    installation.

    \sa QKnxAddress::Group, QKnxAddress::Notation
*/

/*!
    \relates QKnxAddress
    \variable QKnxAddress::Individual::Unregistered

    Represents a single KNX individual address with the value \c 0xffff.
*/
QKnxAddress QKnxAddress::Individual::Unregistered = { QKnxAddress::Type::Individual, 0xffff };

/*!
    Returns \c true if this is a valid \l {Type}{individual address}
    object and the device's sequential number is set to \c 0x00;
    \c false otherwise.

    Area and line couplers, as well as KNXnet/IP routers (routing) are given
    the sequential number \c 0x00.

    For example, the individual address \c 1.5.0 identifies a KNXnet/IP router
    acting as a line coupler that couples the fifth line with the main line in
    the first area.
*/
bool QKnxAddress::isCouplerOrRouter() const
{
    return (m_type == QKnxAddress::Type::Individual) && (quint8(m_address) == 0x00);
}

/*!
    Returns \c true if this is a valid \l {Type}{individual address} object and
    the device's sequential number is set to \c 0xff; \c false otherwise.
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
    \a notation. An address of the type \l {Type}{Group} is formatted using a
    forward slash as a separator, while an address of the type \l {Type}
    {Individual} is formatted using a dot as separator.

    \note Individual addresses support only 3-level notation, whereas group
    addresses support 2-level or 3-level notation.
    If the address is invalid or 2-level notation is requested for individual
    addresses, an empty string is returned.
*/
QString QKnxAddress::toString(Notation notation) const
{
    if (notation == QKnxAddress::Notation::ThreeLevel) {
        if (m_type == QKnxAddress::Type::Group) {
            return QStringLiteral("%1/%2/%3").arg((m_address >> 11) & 0x1f)
                .arg((m_address >> 8) & 0x07).arg(m_address & 0xff);
        }
        if (m_type == QKnxAddress::Type::Individual) {
            return QStringLiteral("%1.%2.%3").arg(m_address >> 12)
                .arg((m_address >> 8) & 0x0f).arg(m_address & 0xff);
        }
    }
    if (notation == QKnxAddress::Notation::TwoLevel && m_type == QKnxAddress::Type::Group)
        return QStringLiteral("%1/%2").arg((m_address >> 11) & 0x1f).arg(m_address & 0x07ff);
    return QString();
}

/*!
    Returns \c true if this object is equal with \a other; otherwise
    returns \c false.
*/

bool QKnxAddress::operator==(const QKnxAddress &other) const
{
    return m_address == other.m_address && m_type == other.m_type;
}

/*!
    Returns \c true if this object is not equal with \a other; otherwise
    returns \c false.
*/

bool QKnxAddress::operator!=(const QKnxAddress &other) const
{
    return !operator==(other);
}

/*!
    \fn QKnxByteArray QKnxAddress::bytes() const

    Returns a KNX address as a range of bytes if the address is valid;
    otherwise the return value is empty. Only the first two bytes of the return
    value are part of the address.
*/

/*!
    \relates QKnxAddress

    Writes the KNX address \a address to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxAddress &address)
{
    QDebugStateSaver _(debug);
    if (address.isValid()) {
        const auto rawData = address.bytes();
        debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar(QLatin1Char('0'))
            << rawData.at(0) << hex << rawData.at(1);
    } else {
         debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}


// -- private

/*!
    \internal

    Creates a KNX address from 2 or 3-level notation. The type of the address is
    specified by \a type.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, quint16 sec1, quint16 *sec2, quint16 sec3)
{
    const auto checkRange = [type, sec1, sec2, sec3](QKnxAddress::Notation notation) {
        if (type == QKnxAddress::Type::Group) {
            if (sec1 > 31)
                return false;
            if (notation == QKnxAddress::Notation::TwoLevel)
                return (sec3 <= 2047);
            if (sec2 && (notation == QKnxAddress::Notation::ThreeLevel))
                return (*sec2 <= 7) && (sec3 <= 255);
        }
        if (sec2 && (type == QKnxAddress::Type::Individual))
            return (sec1 <= 15) && ( *sec2 <= 15) && (sec3 <= 255);
        return false;
    };

    if (type == QKnxAddress::Type::Group) {
        if (!sec2) {
            if (checkRange(QKnxAddress::Notation::TwoLevel)) {
                m_type = type;
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

uint qHash(const QKnxAddress &key, uint seed) Q_DECL_NOTHROW
{
    return qHash(key.bytes(), seed);
}

QT_END_NAMESPACE
