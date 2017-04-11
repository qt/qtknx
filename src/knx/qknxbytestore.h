/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXBYTESTORE_H
#define QKNXBYTESTORE_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxbytestoreref.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxByteStore
{
public:
    quint16 size() const
    {
        return quint16(m_bytes.size());
    }

    void resize(quint16 size, const quint8 value = 0)
    {
        m_bytes.resize(size, value);
    }

    const quint8 *data() const
    {
        return m_bytes.data();
    }

    quint8 byte(quint16 index) const
    {
        if (index < m_bytes.size())
            return m_bytes[index];
        return {};
    }

    void setByte(quint16 index, quint8 byte)
    {
        if (m_bytes.size() <= index)
            resize(index + 1);
        m_bytes[index] = byte;
    }

    virtual QString toString() const
    {
        QString bytes;
        for (quint8 byte : m_bytes)
            bytes += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
        bytes.chop(2);
        return QStringLiteral("Bytes { %1 }").arg(bytes);
    }

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(m_bytes.size(), 0);
        std::copy(std::begin(m_bytes), std::end(m_bytes), std::begin(t));
        return t;
    }

    template <typename T = std::vector<quint8>>
        auto bytes(quint16 start, quint16 size) const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (quint16(m_bytes.size()) < start + size)
            return {};

        T t(size, 0);
        std::copy(std::next(std::begin(m_bytes), start), std::next(std::begin(m_bytes), start
            + size), std::begin(t));
        return t;
    }

    void setBytes(const quint8 *data);
    void setBytes(const quint8 *data, quint16 index, quint16 size);

    void setBytes(const QKnxByteStoreRef &storeRef);
    void setBytes(const QKnxByteStoreRef &storeRef, quint16 index, quint16 size);

    template <typename T, std::size_t S = 0> void setBytes(const T &bytes)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.clear();
        m_bytes.resize(bytes.size());
        std::copy(std::begin(bytes), std::end(bytes), std::begin(m_bytes));
    }

    // TODO: The next function signature is misleading, it suggest the function will change
    // m_bytes starting from 'index' with 'count' elements, though it sets m_bytes to the bytes
    // stored inside the argument 'bytes', starting from 'index' with 'count' elements.
    template <typename T, std::size_t S = 0> void setBytes(const T &bytes, quint16 index,
        quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.resize(0);
        if (quint16(bytes.size()) < index + size)
            return;

        m_bytes.resize(size);
        std::copy_n(std::next(std::begin(bytes), index), size, std::begin(m_bytes));
    }

    template <typename T, std::size_t S = 0> void appendBytes(const T &bytes)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (bytes.size() <= 0)
            return;

        m_bytes.resize(m_bytes.size() + quint16(bytes.size()));
        std::copy(std::begin(bytes), std::end(bytes), std::prev(std::end(m_bytes), bytes.size()));
    }

    QKnxByteStoreRef ref() const;

protected:
    QKnxByteStore();
    virtual ~QKnxByteStore() = default;

    QKnxByteStore(const QKnxByteStore &other);
    QKnxByteStore &operator=(const QKnxByteStore &other);

private:
    std::vector<quint8> m_bytes;
    QKnxByteStoreRef m_byteStoreRef;
};

QT_END_NAMESPACE

#endif
