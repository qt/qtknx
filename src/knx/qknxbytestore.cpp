/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "QtKnx/qknxbytestore.h"
#include "QtKnx/qknxbytestoreref.h"

QT_BEGIN_NAMESPACE

QKnxByteStore::QKnxByteStore()
    : m_byteStoreRef(this)
{}

QKnxByteStore::QKnxByteStore(const QKnxByteStore &other)
    : m_bytes(other.m_bytes)
    , m_byteStoreRef(this)
{}

QKnxByteStore &QKnxByteStore::operator=(const QKnxByteStore &other)
{
    if (this != &other) {
        m_bytes = other.m_bytes;
        m_byteStoreRef = QKnxByteStoreRef(this);
    }
    return *this;
}

void QKnxByteStore::setBytes(const quint8 *data)
{
    return setBytes(data, 0, quint16(strlen(reinterpret_cast<const char*> (data))));
}

void QKnxByteStore::setBytes(const quint8 *data, quint16 index, quint16 size)
{
    m_bytes.resize(0);
    if (quint16(strlen(reinterpret_cast<const char*> (data))) < index + size)
        return;

    m_bytes.resize(size);
    std::copy_n(std::next(data, index), size, std::begin(m_bytes));
}

void QKnxByteStore::setBytes(const QKnxByteStoreRef &storeRef)
{
    return setBytes(storeRef, 0, storeRef.size());
}

void QKnxByteStore::setBytes(const QKnxByteStoreRef &storeRef, quint16 index, quint16 size)
{
    m_bytes.resize(0);
    if (quint16(storeRef.size()) < index + size)
        return;

    m_bytes.resize(size);
    std::copy_n(std::next(storeRef.bytes(), index), size, std::begin(m_bytes));
}

QKnxByteStoreRef QKnxByteStore::ref() const
{
    return m_byteStoreRef;
}

QT_END_NAMESPACE
