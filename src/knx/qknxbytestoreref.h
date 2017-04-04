/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXBYTESTOREREF_H
#define QKNXBYTESTOREREF_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxByteStoreRef final
{
public:
    QKnxByteStoreRef() = default;
    ~QKnxByteStoreRef() = default;

    explicit QKnxByteStoreRef(const QKnxByteStore *store)
        : m_store(store)
    {}

    QKnxByteStoreRef(const QKnxByteStoreRef &) = default;
    QKnxByteStoreRef &operator=(const QKnxByteStoreRef &) = default;

    quint16 size() const
    {
        if (m_store)
            return m_store->size();
        return 0;
    }

    quint8 byte(quint16 index) const
    {
        if (m_store)
            return m_store->byte(index);
        return {};
    }

    const quint8 *bytes() const
    {
        if (m_store)
            return m_store->data();
        return nullptr;
    }

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        if (m_store)
            return m_store->bytes<T>();
        return {};
    }

    template <typename T = std::vector<quint8>>
        auto bytes(quint16 start, quint16 size) const -> decltype(T())
    {
        if (m_store)
            return m_store->bytes<T>(start, size);
        return {};
    }

private:
    const QKnxByteStore *m_store = nullptr;
};

QT_END_NAMESPACE

#endif
