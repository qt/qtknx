/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxbytestore.h"
#include "qknxbytestoreref.h"

QT_BEGIN_NAMESPACE

QKnxByteStoreRef::QKnxByteStoreRef(QKnxByteStore *store, quint16 index)
{
    if (store && index < store->size()) {
        m_index = index;
        m_store = store;
    }
}

quint16 QKnxByteStoreRef::size() const
{
    if (m_store)
        return m_store->size() - m_index;
    return 0;
}

const quint8 *QKnxByteStoreRef::bytes() const
{
    if (m_store)
        return &(*std::next(m_store->data(), m_index));
    return nullptr;
}

QT_END_NAMESPACE
