/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

struct QKnxNetIpByteStore
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

    template <typename T> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(m_bytes.size(), Qt::Uninitialized);
        std::copy(std::begin(m_bytes), std::end(m_bytes), std::begin(t));
        return t;
    }

    template <typename T> auto bytes(quint16 start, quint16 size) const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (quint16(m_bytes.size()) < start + size)
            return {};

        T t(size, Qt::Uninitialized);
        std::copy(std::next(std::begin(m_bytes), start), std::next(std::begin(m_bytes), start
            + size), std::begin(t));
        return t;
    }

    template <typename T, std::size_t S = 0> void setBytes(const T &bytes)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.clear();
        m_bytes.resize(bytes.size());
        std::copy(std::begin(bytes), std::end(bytes), std::begin(m_bytes));
    }

    template <typename T, std::size_t S = 0> void setBytes(const T &bytes, quint16 index, int size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_bytes.resize(0);
        if (bytes.size() < (index + size))
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

protected:
    QKnxNetIpByteStore() = default;
    virtual ~QKnxNetIpByteStore() = default;

private:
    std::vector<quint8> m_bytes;
};

QT_END_NAMESPACE
