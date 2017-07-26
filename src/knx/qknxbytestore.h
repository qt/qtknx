/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

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
        if (index < size())
            return m_bytes[index];
        return {};
    }

    void setByte(quint16 index, quint8 byte)
    {
        if (size() <= index)
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

    template <typename T = QByteArray> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(size(), 0);
        std::copy(std::begin(m_bytes), std::end(m_bytes), std::begin(t));
        return t;
    }

    template <typename T = QByteArray> auto bytes(quint16 start, quint16 count) const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (size() < start + count)
            return {};

        T t(count, 0);
        std::copy(std::next(std::begin(m_bytes), start), std::next(std::begin(m_bytes), start
            + count), std::begin(t));
        return t;
    }

    void setBytes(const QKnxByteStoreRef &storeRef);

    template <typename T, std::size_t S = 0> void setBytes(const T &sourceBytes)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.resize(sourceBytes.size());
        std::copy(std::begin(sourceBytes), std::end(sourceBytes), std::begin(m_bytes));
    }

    void setBytes(QByteArray::const_iterator sourceBegin, QByteArray::const_iterator sourceEnd)
    {
        m_bytes.resize(std::distance(sourceBegin, sourceEnd));
        std::copy(sourceBegin, sourceEnd, std::begin(m_bytes));
    }

    template <typename Iterator> void setBytes(Iterator sourceBegin, Iterator sourceEnd)
    {
        static_assert(is_type<typename std::iterator_traits<Iterator>::value_type, quint8>::value,
            "Type not supported.");

        m_bytes.resize(std::distance(sourceBegin, sourceEnd));
        std::copy(sourceBegin, sourceEnd, std::begin(m_bytes));
    }

    template <typename T, std::size_t S = 0> void appendBytes(const T &bytesToAppend)
    {
        insertBytes<T, S>(size(), bytesToAppend);
    }

    template <typename T, std::size_t S = 0> void insertBytes(quint16 pos, const T &bytesToInsert)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (bytesToInsert.size() <= 0)
            return;

        quint16 tmpSize = size();
        if (pos < tmpSize) {
            m_bytes.resize(tmpSize + bytesToInsert.size());
            std::move_backward(std::next(std::begin(m_bytes), pos),
                std::prev(std::end(m_bytes), bytesToInsert.size()), std::end(m_bytes));
        } else {
            resize(pos + quint16(bytesToInsert.size()));
        }

        std::copy(std::begin(bytesToInsert), std::end(bytesToInsert),
            std::next(std::begin(m_bytes), pos));
    }

    template <typename T, std::size_t S = 0> void replaceBytes(quint16 pos, const T &replacement)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (replacement.size() <= 0)
            return;

        if ((pos + replacement.size()) >= size())
            resize(pos + quint16(replacement.size()));

        std::copy(std::begin(replacement), std::end(replacement),
            std::next(std::begin(m_bytes), pos));
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
