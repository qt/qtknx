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
    \class QKnxByteStoreRef

    \inmodule QtKnx
    \brief The QKnxByteStoreRef class represents a pointer to a byte store.
*/

/*!
    Constructs a byte store reference using the pointer \a store to a byte store
    and the \a index at which the reference starts.
*/
QKnxByteStoreRef::QKnxByteStoreRef(QKnxByteStore *store, quint16 index)
{
    if (store && index < store->size()) {
        m_index = index;
        m_store = store;
    }
}

/*!
    Returns the size of the byte store referred to by the object.
*/
quint16 QKnxByteStoreRef::size() const
{
    if (m_store)
        return m_store->size() - m_index;
    return 0;
}

quint8 QKnxByteStoreRef::operator[](int i) const
{
    Q_ASSERT_X(i >= 0 && i < size(), "QKnxByteStoreRef::operator[]", "index out of range");
    if (const quint8 *tmp = bytes())
        return tmp[i];
    return 0;
}

/*!
    Returns the content of the byte store referred to as an array of \l quint8.
*/
const quint8 *QKnxByteStoreRef::bytes() const
{
    if (m_store)
        return &(*std::next(m_store->data(), m_index));
    return nullptr;
}

/*!
    \fn quint8 QKnxByteStoreRef::byte(quint16 index) const

    Returns the byte at \a index.
*/

QT_END_NAMESPACE
