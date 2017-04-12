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
    friend class QKnxByteStoreRef;

public:
    quint16 size() const
    {
        return quint16(m_bytes.size());
    }

    void resize(quint16 size, const quint8 value = 0)
    {
        m_bytes.resize(size, value);
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
        QString bytesString;
        for (quint8 byte : m_bytes)
            bytesString += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
        bytesString.chop(2);
        return QStringLiteral("Bytes { %1 }").arg(bytesString);
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

    void setBytes(const QKnxByteStoreRef &storeRef);
    void setBytes(const QKnxByteStoreRef &storeRef, quint16 index, quint16 size);

    template <typename T, std::size_t S = 0> void setBytes(const T &sourceBytes)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.clear();
        m_bytes.resize(sourceBytes.size());
        std::copy(std::begin(sourceBytes), std::end(sourceBytes), std::begin(m_bytes));
    }

    // TODO: The next function signature is misleading, it suggest the function will change
    // m_bytes starting from 'index' with 'count' elements, though it sets m_bytes to the bytes
    // stored inside the argument 'bytes', starting from 'index' with 'count' elements.
    template <typename T, std::size_t S = 0> void setBytes(const T &bytesToCopy, quint16 index,
        quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.resize(0);
        if (quint16(bytesToCopy.size()) < index + size)
            return;

        m_bytes.resize(size);
        std::copy_n(std::next(std::begin(bytesToCopy), index), size, std::begin(m_bytes));
    }

    template <typename T, std::size_t S = 0> void appendBytes(const T &bytesToAppend)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (bytesToAppend.size() <= 0)
            return;

        m_bytes.resize(m_bytes.size() + quint16(bytesToAppend.size()));
        std::copy(std::begin(bytesToAppend), std::end(bytesToAppend), std::prev(std::end(m_bytes),
            bytesToAppend.size()));
    }

    QKnxByteStoreRef ref(quint16 index = 0) const;

protected:
    QKnxByteStore() = default;
    virtual ~QKnxByteStore() = default;

    explicit QKnxByteStore(quint8 byte);
    QKnxByteStore(const quint8 *bytes, quint16 size);

    const quint8 *data() const { return m_bytes.data(); }

private:
    std::vector<quint8> m_bytes;
};

QT_END_NAMESPACE

#endif
