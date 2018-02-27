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

#include "qknxbytestore.h"
#include "qknxbytestoreref.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxByteStore

    \inmodule QtKnx
    \brief The QKnxByteStore class is a byte store used when building frames.

    To communicate with the KNX bus, frames needs to be exchanged. The byte
    store is useful when building the frames and serves as a base to classes
    representing the frames.
*/

/*!
    Constructs a new byte store equal to \a byte.
*/
QKnxByteStore::QKnxByteStore(quint8 byte)
{
    m_bytes.resize(1);
    m_bytes[0] = byte;
}

/*!
    Constructs a new byte store of the size \a size from the \a bytes array.
*/
QKnxByteStore::QKnxByteStore(const quint8 *bytes, quint16 size)
{
    m_bytes.resize(size);
    std::copy_n(bytes, size, std::begin(m_bytes));
}

/*!
    \fn quint16 QKnxByteStore::size() const

    Returns the number of bytes.
*/

/*!
    \fn void QKnxByteStore::resize(quint16 size, const quint8 value = 0)

    Resizes the range of bytes to the given \a size and \a value.
*/

/*!
    \fn quint8 QKnxByteStore::byte(quint16 index) const

    Returns the byte of the current byte store at \a index.
*/

/*!
    \fn virtual QString QKnxByteStore::toString() const

    Returns a \l QString object representing the bytes.
*/

/*!
    \fn template <typename T> auto QKnxByteStore::bytes() const

    Returns the bytes of the byte store in the chosen format.
*/

/*!
    \fn template <typename T> auto QKnxByteStore::bytes(quint16 start, quint16 count) const

    Returns the bytes of the byte store in the chosen format. Starting at the
    byte at the index \a start until the byte at the index \a start plus
    \a count.
*/

/*!
    Resizes the current byte store to the size of one passed as
    \l QKnxByteStoreRef with \a storeRef. The \a storeRef bytes are copied to
    the current object.
*/
void QKnxByteStore::setBytes(const QKnxByteStoreRef &storeRef)
{
    m_bytes.resize(storeRef.size());
    std::copy(std::begin(storeRef), std::end(storeRef), std::begin(m_bytes));
}

/*!
    \fn template <typename T, std::size_t S> void QKnxByteStore::setBytes(const T &sourceBytes)

    Replaces the current byte store with the provided \a sourceBytes.
*/

/*!
    \fn void QKnxByteStore::setBytes(QByteArray::const_iterator sourceBegin, QByteArray::const_iterator sourceEnd)

     Replaces the current byte store using the STL-style const iterators
    \a sourceBegin and \a sourceEnd.
*/

/*!
    \fn template <typename Iterator> void sQKnxByteStore::setBytes(Iterator sourceBegin, Iterator sourceEnd)

    Replaces the current byte store by using the iterators \a sourceBegin
    and \a sourceEnd.
*/

/*!
    \fn template <typename T, std::size_t S> void QKnxByteStore::appendBytes(const T &bytesToAppend)

    Adds the \a bytesToAppend to the current byte store.
*/

/*!
    \fn template <typename T, std::size_t S> void QKnxByteStore::insertBytes(quint16 pos, const T &bytesToInsert)

    Inserts the bytes specified by \a bytesToInsert at the position \a pos.
*/

/*!
    \fn template <typename T, std::size_t S = 0> void QKnxByteStore::replaceBytes(quint16 pos, const T &replacement)

    Replaces the bytes situated at the position \a pos by the ones specified by
    \a replacement.
*/

void QKnxByteStore::replaceBytes(quint16 pos, const QKnxByteArray & replacement)
{
    if (replacement.size() <= 0)
        return;

    if ((pos + replacement.size()) >= size())
        resize(pos + quint16(replacement.size()));
    m_bytes.replace(pos, replacement.size(), replacement);
}

/*!
    Creates a \l QKnxByteStoreRef object pointing to the \a index of the object.
*/
QKnxByteStoreRef QKnxByteStore::ref(quint16 index) const
{
    return QKnxByteStoreRef(const_cast<QKnxByteStore*> (this), index);
}

QT_END_NAMESPACE
