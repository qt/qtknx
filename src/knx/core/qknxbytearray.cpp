/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Copyright (C) 2016 Intel Corporation.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qknxbytearray.h"

#include <new>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxByteArray
    \inmodule QtKnx
    \ingroup qtknx-general-classes

    \brief The QKnxByteArray class provides an array of unsigned bytes.

    A KNX byte array can be used to store both raw bytes (including the string
    \c {\0} and any characters that might come after it) and traditional 8-bit
    \c {\0}-terminated strings. Using QKnxByteArray is much more convenient than
    using \c{const quint8 *}. Behind the scenes, it always ensures that the data
    is followed by a null-terminator (\c {\0}).

    QKnxByteArray makes a deep copy of the data given, so you
    can modify it later without experiencing side effects.

    Another approach is to set the size of the array using resize()
    and to initialize the data byte per byte. QKnxByteArray uses 0-based
    indexes, just like C++ arrays. To access the byte at a particular
    index position, you can use at(), set(), value() and setValue().

    To extract several bytes at a time, use left(), right(), or mid().

    A KNX byte array can embed \c {\0} bytes. The size() function always
    returns the size of the whole array, including the embedded \c {\0}
    bytes, but excluding the null-terminator added by QKnxByteArray.

    After a call to resize(), newly allocated bytes have undefined
    values. To set all the bytes to a particular value, call fill().

    To obtain a pointer to the actual character data, call data() or
    constData(). These functions return a pointer to the beginning of the data.
    The pointer is guaranteed to remain valid until a non-const function is
    called on the byte array.
*/

/*!
    \fn QKnxByteArray::QKnxByteArray()

    Constructs an empty byte array.
*/

/*!
    \fn QKnxByteArray::~QKnxByteArray()

    Destroys the byte array.
*/

/*!
    Constructs a byte array of the size \a size with every byte set to the
    character \a ch.
*/
QKnxByteArray::QKnxByteArray(int size, quint8 ch)
    : m_bytes(size, char(ch))
{}

/*!
    \internal

    Constructs a byte array of the size \a size with uninitialized contents.
*/
QKnxByteArray::QKnxByteArray(int size, Qt::Initialization)
    : m_bytes(size, Qt::Uninitialized)
{}

/*!
    Constructs a byte array from \a data containing the number of bytes
    specified by \a size starting from the beginning of the data.

    If \a data is 0, a null byte array is constructed.

    If \a size is negative, \a data is assumed to point to a null-terminated
    string and its length is determined dynamically. The terminating
    null-character is not considered part of the byte array.

    QKnxByteArray makes a deep copy of the string data.
*/
QKnxByteArray::QKnxByteArray(const char *data, int size)
    : m_bytes(data, size)
{}

/*!
    \overload QKnxByteArray()
*/
QKnxByteArray::QKnxByteArray(const quint8 *data, int size)
    : m_bytes(reinterpret_cast<const char*> (data), size)
{}

/*!
    Constructs a byte array from the \c {std::initializer_list} specified by
    \a args.
*/
QKnxByteArray::QKnxByteArray(std::initializer_list<quint8> args)
{
    if (args.size() > 0) {
        auto d = new (std::nothrow) quint8[args.size()];
        if (!d)
            return;
        memcpy(d, args.begin(), (args.end() - args.begin()) * sizeof(quint8));
        m_bytes = QByteArray(reinterpret_cast<const char*> (d), int(args.size()));
        delete []d;
    }
}

/*!
    \fn QKnxByteArray::QKnxByteArray(const QKnxByteArray &other)

    Constructs a copy of \a other.
*/

/*!
    Assigns \a other to this KNX byte array and returns a reference.
*/
QKnxByteArray &QKnxByteArray::operator=(const QKnxByteArray &other) Q_DECL_NOTHROW
{
    m_bytes.operator=(other.m_bytes);
    return *this;
}

/*!
    \fn QKnxByteArray::QKnxByteArray(QKnxByteArray &&other)

    Move-constructs a QKnxByteArray instance, making it point to the same
    object that \a other was pointing to.
*/

/*!
    \fn QKnxByteArray &QKnxByteArray::operator=(QKnxByteArray &&other)

    Move-assigns \a other to this QKnxByteArray instance.
*/

/*!
    \fn void QKnxByteArray::swap(QKnxByteArray &other)

    Swaps the byte array \a other with this byte array. This operation is very
    fast and never fails.
*/

/*!
    Returns a copy of this byte array as \l QByteArray.
*/
const QByteArray &QKnxByteArray::toByteArray() const
{
    return m_bytes;
}

/*!
    Returns a byte array constructed from \a byteArray .
*/
QKnxByteArray QKnxByteArray::fromByteArray(const QByteArray &byteArray)
{
    QKnxByteArray ba(0, Qt::Uninitialized);
    ba.m_bytes = byteArray;
    return ba;
}

/*!
    Returns \c true if this byte array is null; otherwise returns \c false.
*/
bool QKnxByteArray::isNull() const
{
    return m_bytes.isNull();
}

/*!
    \fn bool QKnxByteArray::isEmpty() const

    Returns \c true if the byte array has the size 0; otherwise returns \c
    false.
*/

/*!
    \fn int QKnxByteArray::size() const

    Returns the number of bytes in this byte array. The last byte in the byte
    array is at the position returned by this function minus 1.
*/

/*!
    Clears the contents of the byte array and makes it null.
*/
void QKnxByteArray::clear()
{
    m_bytes.clear();
}

/*!
    Sets the size of the byte array to the number of bytes specified by \a size.

    If \a size is greater than the current size, the byte array is
    extended to \a size bytes by adding \l {default-constructed value}
    {default-constructed values} to the end of the array.

    If \a size is less than the current size, bytes are removed from the end of
    the array.
*/
void QKnxByteArray::resize(int size)
{
    if (size == m_bytes.size())
        return;

    if (size > m_bytes.size())
        m_bytes.append(size - m_bytes.size(), 0x00);
    else
        m_bytes.resize(size);
}

/*!
    \fn quint8 QKnxByteArray::at(int i) const

    Returns the character at the index position \a i in the byte array.

    \a i must be a valid index position in the byte array (that is, between
    0 and the value returned by size()).
*/

/*!
    \fn void QKnxByteArray::set(int i, quint8 value)

    Sets the character at the index position \a i in the byte array to \a value.

    \a i must be a valid index position in the byte array (that is, between
    0 and the value returned by size()).
*/

/*!
    \fn void QKnxByteArray::setValue(int i, quint8 value)

    Sets the value at the index position \a i in the byte array to \a value.

    If the index \a i is out of bounds, the function does nothing. If you
    are certain that \a i is within bounds, you can use set() instead, which
    is slightly faster.
*/

/*!
    \fn quint8 QKnxByteArray::value(int i, quint8 defaultValue = {}) const

    Returns the value at the index position \a i in the byte array.

    If the index \a i is out of bounds, the function returns \a defaultValue,
    which may be a \l{default-constructed value}. If you are certain that \a i
    is within bounds, you can use at() instead, which is slightly faster.
*/

/*!
    Returns a copy of this byte array repeated the specified number of \a times.

    If \a times is less than 1, an empty byte array is returned.
*/
QKnxByteArray QKnxByteArray::repeated(int times) const
{
    QKnxByteArray ba(0, Qt::Uninitialized);
    ba.m_bytes = m_bytes.repeated(times);
    return ba;
}

/*!
    Sets every byte in the byte array to the character \a ch and returns a
    reference to this byte array. If \a size is different from -1, the byte
    array is resized to \a size beforehand.
*/
QKnxByteArray &QKnxByteArray::fill(quint8 ch, int size)
{
    m_bytes.fill(ch, size);
    return *this;
}

/*!
    Returns a byte array that contains the number of leftmost bytes in the byte
    array specified by \a len.

    The entire byte array is returned if \a len is greater than the value
    returned by size().
*/

QKnxByteArray QKnxByteArray::left(int len)  const
{
    if (len >= size())
        return *this;
    if (len < 0)
        len = 0;
    return QKnxByteArray(constData(), len);
}

/*!
    Returns a byte array that contains the number of rightmost bytes in the byte
    array specified by \a len.

    The entire byte array is returned if \a len is greater than the value
    returned by size().
*/
QKnxByteArray QKnxByteArray::right(int len) const
{
    if (len >= size())
        return *this;
    if (len < 0)
        len = 0;
    return QKnxByteArray(constData() + size() - len, len);
}

/*!
    Returns a byte array containing the number of bytes in the byte array
    specified by \a len, starting at the position \a pos.

    If \a len is -1 (the default), or \a pos added to \a len is equal to or
    larger than the value returned by size(), returns a byte array containing
    all bytes starting from the position \a pos to the end of the byte array.
*/
QKnxByteArray QKnxByteArray::mid(int pos, int len) const
{
    const auto m = m_bytes.mid(pos, len);
    if (m.isEmpty())
        return {};
    return QKnxByteArray(m.constData(), m.size());
}

/*!
    \fn QKnxByteArray::chop(int len) const

    Returns a byte array that contains the number of leftmost bytes in this
    byte array calculated by subtracting the amount of bytes specified by \a len
    from the amount of bytes of returned by size().

    \note The behavior is undefined if \a len is negative.
*/

/*!
    \overload prepend()

    Prepends the character \a ch to this byte array.
*/
QKnxByteArray &QKnxByteArray::prepend(quint8 ch)
{
    m_bytes.prepend(char(ch));
    return *this;
}

/*!
    Prepends the byte array \a ba to this byte array and returns a
    reference to this byte array.
*/
QKnxByteArray &QKnxByteArray::prepend(const QKnxByteArray &ba)
{
    m_bytes.prepend(reinterpret_cast<const char*> (ba.constData()), ba.size());
    return *this;
}

/*!
    \overload prepend()
    \fn QKnxByteArray &QKnxByteArray::prepend(int count, quint8 ch)

    Prepends the number of copies of the character \a ch specified by \a count
    to this byte array.
*/

/*!
    \overload insert()

    Inserts the character \a ch at the index position \a i in the byte array.
    If \a i is greater than the number of bytes returned by size(), the array
    is first extended using resize().
*/
QKnxByteArray &QKnxByteArray::insert(int i, quint8 ch)
{
    m_bytes.insert(i, char(ch));
    return *this;
}

/*!
    \overload insert()

    Inserts the number of copies of the character \a ch specified by \a count
    at the index position \a i in the byte array.

    If \a i is greater than the value returned by size(), the array is first
    extended using resize().
*/
QKnxByteArray &QKnxByteArray::insert(int i, int count, quint8 ch)
{
    m_bytes.insert(i, count, char(ch));
    return *this;
}

/*!
    Inserts the byte array \a ba at the index position \a i and returns a
    reference to this byte array.
*/
QKnxByteArray &QKnxByteArray::insert(int i, const QKnxByteArray &ba)
{
    m_bytes.insert(i, reinterpret_cast<const char*> (ba.constData()), ba.size());
    return *this;
}

/*!
    \overload append()

    Appends the character \a ch to this byte array.
*/
QKnxByteArray& QKnxByteArray::append(quint8 ch)
{
    m_bytes.append(char(ch));
    return *this;
}

/*!
    Appends the byte array \a ba to the end of this byte array.
*/
QKnxByteArray &QKnxByteArray::append(const QKnxByteArray &ba)
{
    m_bytes.append(reinterpret_cast<const char*> (ba.constData()), ba.size());
    return *this;
}

/*!
    \overload append()
    \fn QKnxByteArray &QKnxByteArray::append(int count, quint8 ch)

    Appends the number of copies of the character \a ch specified by \a count to
    this byte array and returns a reference to this byte array.

    If \a count is negative or zero, nothing is appended to the byte array.
*/

/*!
    Replaces the number of bytes specified by \a len beginning at the index
    position \a index with the byte array \a after, and returns a reference
    to this byte array.
*/
QKnxByteArray &QKnxByteArray::replace(int index, int len, const QKnxByteArray &after)
{
    m_bytes.replace(index, len, reinterpret_cast<const char*> (after.constData()), after.size());
    return *this;
}

/*!
    \overload replace()

    Replaces every occurrence of the character \a before with the
    byte array \a after.
*/
QKnxByteArray &QKnxByteArray::replace(quint8 before, const QKnxByteArray &after)
{
    m_bytes.replace(char(before), after.toByteArray());
    return *this;
}

/*!
    \overload replace()

    Replaces every occurrence of the byte array \a before with the
    byte array \a after.
*/
QKnxByteArray &QKnxByteArray::replace(const QKnxByteArray &before, const QKnxByteArray &after)
{
    m_bytes.replace(before.toByteArray(), after.toByteArray());
    return *this;
}

/*!
    \overload replace()

    Replaces every occurrence of the character \a before with the
    character \a after.
*/
QKnxByteArray &QKnxByteArray::replace(quint8 before, quint8 after)
{
    m_bytes.replace(char(before), char(after));
    return *this;
}

/*!
    Removes the number of bytes specified by \a len from the array, starting at
    the index position \a pos, and returns a reference to the array.

    If \a pos is out of range, nothing happens. If \a pos is valid, but \a pos
    added to \a len is larger than the size of the array, the array is truncated
    at the position \a pos.
*/
QKnxByteArray &QKnxByteArray::remove(int pos, int len)
{
    m_bytes.remove(pos, len);
    return *this;
}

/*!
    \fn quint8 *QKnxByteArray::data()

    Returns a pointer to the data stored in the byte array. The
    pointer can be used to access and modify the bytes that compose
    the array. The data is null-terminated, which means that the number of
    bytes in the returned character string is the value returned by size() plus
    1 for the null-terminator.

    The pointer remains valid until the byte array is reallocated or destroyed.
*/

/*!
    \overload data()
    \fn const quint8 *QKnxByteArray::data() const
*/

/*!
    \fn const quint8 *QKnxByteArray::constData() const

    Returns a pointer to the data stored in the byte array. The pointer can be
    used to access the bytes that compose the array. The data is
    null-terminated.

    The pointer remains valid until the byte array is reallocated or destroyed.
*/

/*!
    \overload indexOf()

    Returns the index position of the first occurrence of the
    character \a ch in the byte array, searching forward from the index
    position \a from. Returns -1 if \a ch could not be found.
*/
int QKnxByteArray::indexOf(quint8 ch, int from) const
{
    return m_bytes.indexOf(char(ch), from);
}

/*!
    Returns the index position of the first occurrence of the byte
    array \a ba in this byte array, searching forward from the index position
    \a from. Returns -1 if \a ba could not be found.
*/
int QKnxByteArray::indexOf(const QKnxByteArray &ba, int from) const
{
    return m_bytes.indexOf(ba.toByteArray(), from);
}

/*!
    \overload lastIndexOf()

    Returns the index position of the last occurrence of the character \a ch in
    the byte array, searching backward from the index position \a from. If
    \a from is -1 (the default), the search starts at the last byte (that is,
    the value returned by size() minus 1). Returns -1 if \a ch cannot be found.
*/
int QKnxByteArray::lastIndexOf(quint8 ch, int from) const
{
    return m_bytes.lastIndexOf(char(ch), from);
}

/*!
    Returns the index position of the last occurrence of the byte
    array \a ba in this byte array, searching backward from the index
    position \a from. If \a from is -1 (the default), the search
    starts at the last byte. Returns -1 if \a ba could not be found.
*/
int QKnxByteArray::lastIndexOf(const QKnxByteArray &ba, int from) const
{
    return m_bytes.lastIndexOf(ba.toByteArray(), from);
}

/*!
    \overload contains()
    \fn bool QKnxByteArray::contains(quint8 ch) const

    Returns \c true if the byte array contains the character \a ch;
    otherwise returns \c false.
*/

/*!
    \fn bool QKnxByteArray::contains(const QKnxByteArray &ba) const

    Returns \c true if the byte array contains an occurrence of the byte
    array \a ba; otherwise returns \c false.
*/

/*! \overload startsWith()

    Returns \c true if this byte array starts with the character \a ch;
    otherwise returns \c false.
*/
bool QKnxByteArray::startsWith(quint8 ch) const
{
    return m_bytes.startsWith(char(ch));
}

/*!
    Returns \c true if this byte array starts with the byte array \a ba;
    otherwise returns \c false.
*/
bool QKnxByteArray::startsWith(const QKnxByteArray &ba) const
{
    return m_bytes.startsWith(ba.toByteArray());
}

/*!
    \overload endsWith()

    Returns \c true if this byte array ends with the character \a ch;
    otherwise returns \c false.
*/
bool QKnxByteArray::endsWith(quint8 ch) const
{
    return m_bytes.endsWith(char(ch));
}

/*!
    Returns \c true if this byte array ends with the byte array \a ba;
    otherwise returns \c false.
*/
bool QKnxByteArray::endsWith(const QKnxByteArray &ba) const
{
    return m_bytes.endsWith(ba.toByteArray());
}

/*!
    Returns a hex encoded copy of the byte array. The hex encoding uses the
    numbers 0-9 and the letters a-f.

    If \a separator is not \c{\0}, the separator character is inserted between
    the hex bytes.

    Example:
    \code
        auto macAddress = QKnxByteArray::fromHex("123456abcdef");
        macAddress.toHex(':'); // returns "12:34:56:ab:cd:ef"
        macAddress.toHex(0);   // returns "123456abcdef"
    \endcode
*/
QKnxByteArray QKnxByteArray::toHex(quint8 separator) const
{
    if (!size())
        return {};

    QKnxByteArray hex(0, Qt::Uninitialized);
    hex.m_bytes = m_bytes.toHex(separator);
    return hex;
}

/*!
    \overload fromHex()
*/
QKnxByteArray QKnxByteArray::fromHex(const QByteArray &hexEncoded)
{
    QKnxByteArray hex(0, Qt::Uninitialized);
    hex.m_bytes = QByteArray::fromHex(hexEncoded);
    return hex;
}

/*!
    Returns a decoded copy of the hex encoded array \a hexEncoded. Input is not
    checked for validity; invalid characters in the input are skipped, enabling
    the decoding process to continue with subsequent characters.
*/
QKnxByteArray QKnxByteArray::fromHex(const QKnxByteArray &hexEncoded)
{
    return QKnxByteArray::fromHex(hexEncoded.m_bytes);
}

/*!
    \typedef QKnxByteArray::iterator

    This typedef provides an STL-style non-const iterator for QKnxByteArray.
*/

/*!
    \typedef QKnxByteArray::const_iterator

    This typedef provides an STL-style const iterator for QKnxByteArray.
*/

/*!
    \internal
    \typedef QKnxByteArray::Iterator
*/

/*!
    \internal
    \typedef QKnxByteArray::ConstIterator
*/

/*!
    \typedef QKnxByteArray::reverse_iterator

    This typedef provides an STL-style non-const reverse iterator for QKnxByteArray.
*/

/*!
    \typedef QKnxByteArray::const_reverse_iterator

    This typedef provides an STL-style const reverse iterator for QKnxByteArray.
*/

/*!
    \fn QKnxByteArray::iterator QKnxByteArray::begin()

    Returns an \l{STL-style iterators}{STL-style iterator} pointing to the first character in
    the byte array.
*/

/*!
    \overload begin()
    \fn QKnxByteArray::const_iterator QKnxByteArray::begin() const
*/

/*!
    \fn QKnxByteArray::const_iterator QKnxByteArray::cbegin() const

    Returns a const \l{STL-style iterators}{STL-style iterator} pointing to the first character
    in the byte array.
*/

/*!
    \fn QKnxByteArray::const_iterator QKnxByteArray::constBegin() const

    Returns a const \l{STL-style iterators}{STL-style iterator} pointing to the first character
    in the byte array.
*/

/*!
    \fn QKnxByteArray::iterator QKnxByteArray::end()

    Returns an \l{STL-style iterators}{STL-style iterator} pointing to the imaginary character
    after the last character in the byte array.
*/

/*!
    \overload end()
    \fn QKnxByteArray::const_iterator QKnxByteArray::end() const

*/

/*!
    \fn QKnxByteArray::const_iterator QKnxByteArray::cend() const

    Returns a const \l{STL-style iterators}{STL-style iterator} pointing to the imaginary
    character after the last character in the list.
*/

/*!
    \fn QKnxByteArray::const_iterator QKnxByteArray::constEnd() const

    Returns a const \l{STL-style iterators}{STL-style iterator} pointing to the imaginary
    character after the last character in the list.
*/

/*!
    \fn QKnxByteArray::reverse_iterator QKnxByteArray::rbegin()

    Returns a \l{STL-style iterators}{STL-style} reverse iterator pointing to the first
    character in the byte array, in reverse order.
*/

/*!
    \fn QKnxByteArray::reverse_iterator QKnxByteArray::rend()

    Returns a \l{STL-style iterators}{STL-style} reverse iterator pointing to one past
    the last character in the byte array, in reverse order.
*/

/*!
    \overload rbegin()
    \fn QKnxByteArray::const_reverse_iterator QKnxByteArray::rbegin() const
*/

/*!
    \overload rend()
    \fn QKnxByteArray::const_reverse_iterator QKnxByteArray::rend() const
*/

/*!
    \fn QKnxByteArray::const_reverse_iterator QKnxByteArray::crbegin() const

    Returns a const \l{STL-style iterators}{STL-style} reverse iterator pointing to the first
    character in the byte array, in reverse order.
*/

/*!
    \fn QKnxByteArray::const_reverse_iterator QKnxByteArray::crend() const

    Returns a const \l{STL-style iterators}{STL-style} reverse iterator pointing to one
    past the last character in the byte array, in reverse order.
*/

/*! \typedef QKnxByteArray::size_type
    \internal
*/

/*! \typedef QKnxByteArray::difference_type
    \internal
*/

/*! \typedef QKnxByteArray::const_reference
    \internal
*/

/*! \typedef QKnxByteArray::const_pointer
    \internal
*/

/*! \typedef QKnxByteArray::reference
    \internal
*/

/*! \typedef QKnxByteArray::pointer
    \internal
*/

/*! \typedef QKnxByteArray::value_type
  \internal
 */

/*!
    \overload operator+=()
    \fn QKnxByteArray &QKnxByteArray::operator+=(quint8 ch)

    Appends the character \a ch onto the end of this byte array and
    returns a reference to this byte array.
*/

/*!
    \fn QKnxByteArray &QKnxByteArray::operator+=(const QKnxByteArray &ba)

    Appends the byte array \a ba onto the end of this byte array and
    returns a reference to this byte array.
*/

/*!
    \relates QKnxByteArray
    \fn bool operator==(const QKnxByteArray &a1, const QKnxByteArray &a2)

    Returns \c true if the byte array \a a1 is equal to the byte array \a a2;
    otherwise returns \c false.
*/

/*!
    \relates QKnxByteArray
    \fn bool operator!=(const QKnxByteArray &a1, const QKnxByteArray &a2)

    Returns \c true if the byte array \a a1 is not equal to the byte array \a a2;
    otherwise returns \c false.
*/

/*!
    \relates QKnxByteArray
    \fn const QKnxByteArray operator+(const QKnxByteArray &a1, const QKnxByteArray &a2)

    Returns a byte array that is the result of concatenating the byte
    array \a a1 and byte array \a a2.
*/

/*!
    \overload operator+()
    \relates QKnxByteArray
    \fn const QKnxByteArray operator+(const QKnxByteArray &ba, quint8 ch)

    Returns a byte array that is the result of concatenating the byte
    array \a ba and character \a ch.
*/

/*!
    \overload operator+()
    \relates QKnxByteArray
    \fn const QKnxByteArray operator+(quint8 ch, const QKnxByteArray &ba)

    Returns a byte array that is the result of concatenating the character
    \a ch and byte array \a ba.
*/

/*!
    \relates QKnxByteArray

    Writes the byte array \a byteArray to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxByteArray &byteArray)
{
    debug << byteArray.toByteArray(); return debug;
}

/*!
    \relates QKnxByteArray
    \fn uint qHash(const QKnxByteArray &key, uint seed = 0)

    Returns the hash value for the \a key, using \a seed to seed the calculation.
*/
uint qHash(const QKnxByteArray &ba, uint seed) Q_DECL_NOTHROW
{
    return qHash(ba.toByteArray(), seed);
}

QT_END_NAMESPACE
