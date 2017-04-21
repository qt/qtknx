/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "QtKnx/qknxbytestore.h"
#include "QtKnx/qknxbytestoreref.h"

QT_BEGIN_NAMESPACE

QKnxByteStore::QKnxByteStore(quint8 byte)
{
    m_bytes.resize(1);
    m_bytes[0] = byte;
}

QKnxByteStore::QKnxByteStore(const quint8 *bytes, quint16 size)
{
    m_bytes.resize(size);
    std::copy_n(bytes, size, std::begin(m_bytes));
}

void QKnxByteStore::setBytes(const QKnxByteStoreRef &storeRef)
{
    m_bytes.resize(storeRef.size());
    std::copy(std::begin(storeRef), std::end(storeRef), std::begin(m_bytes));
}

QKnxByteStoreRef QKnxByteStore::ref(quint16 index) const
{
    return QKnxByteStoreRef(const_cast<QKnxByteStore*> (this), index);
}

QT_END_NAMESPACE
