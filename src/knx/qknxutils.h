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

#ifndef QKNXUTILS_H
#define QKNXUTILS_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytestoreref.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

struct QKnxUtils final
{
    struct QUint8 final
    {
        static quint8 fromBytes(const QKnxByteStoreRef &data, quint16 index = 0)
        {
            if (data.size() - index < 1)
                return {};
            return data.bytes()[index];
        }

        template <typename T, std::size_t S = 0>
            static quint8 fromBytes(const T &data, quint16 index = 0)
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

            if (data.size() - index < 1)
                return {};
            return data[index];
        }
    };

    struct QUint16 final
    {
        template <typename T = std::vector<quint8>>
            static auto bytes(quint16 integer) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");

            T t(2, 0);
            t[0] = quint8(integer >> 8);
            t[1] = quint8(integer);
            return t;
        }

        static quint16 fromBytes(const QKnxByteStoreRef &data, quint16 index = 0)
        {
            if (data.size() - index < 2)
                return {};
            return quint16(quint16(data.bytes()[index]) << 8 | data.bytes()[index + 1]);
        }

        template <typename T, std::size_t S = 0>
            static quint16 fromBytes(const T &data, quint16 index = 0)
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

            if (data.size() - index < 2)
                return {};
            return quint16(quint16(data[index]) << 8 | data[index + 1]);
        }
    };

    struct HostAddress final
    {
        template <typename T = std::vector<quint8>>
            static auto bytes(const QHostAddress &address) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");

            T t(4, 0);
            auto addr = address.toIPv4Address();
            t[0] = quint8(addr >> 24);
            t[1] = quint8(addr >> 16);
            t[2] = quint8(addr >> 8);
            t[3] = quint8(addr);
            return t;
        }

        static QHostAddress fromBytes(const QKnxByteStoreRef &data, quint16 index = 0)
        {
            if (data.size() - index < 4)
                return {};

            const auto bytes = data.bytes();
            return QHostAddress(quint32(bytes[index] << 24 | bytes[index + 1] << 16
                | bytes[index + 2] << 8 | bytes[index + 3]));
        }

        template <typename T, std::size_t S = 0>
            static QHostAddress fromBytes(const T &data, quint16 index = 0)
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

            if (data.size() - index < 4)
                return {};
            return QHostAddress(quint32(data[index] << 24 | data[index + 1] << 16
                | data[index + 2] << 8 | data[index + 3]));
        }
    };
};

QT_END_NAMESPACE

#endif
