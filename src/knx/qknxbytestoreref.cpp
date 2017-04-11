/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxbytestore.h"
#include "qknxbytestoreref.h"

QT_BEGIN_NAMESPACE
QKnxByteStoreRef::QKnxByteStoreRef(const QKnxByteStore *store)
    : m_store(store)
{}

quint16 QKnxByteStoreRef::size() const
{
    if (m_store)
        return m_store->size();
    return 0;
}

quint8 QKnxByteStoreRef::byte(quint16 index) const
{
    if (m_store)
        return m_store->byte(index);
    return {};
}

const quint8 *QKnxByteStoreRef::bytes() const
{
    if (m_store)
        return m_store->data();
    return nullptr;
}

QT_END_NAMESPACE
