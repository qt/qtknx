/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
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
        template <typename T = QByteArray> static auto bytes(quint8 integer) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");

            T t(1, 0);
            t[0] = quint8(integer);
            return t;
        }

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
            return quint8(data[index]);
        }
    };

    struct QUint16 final
    {
        template <typename T = QByteArray> static auto bytes(quint16 integer) -> decltype(T())
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
            return quint16(quint16(quint8(data[index])) << 8 | quint8(data[index + 1]));
        }
    };

    struct QUint32 final
    {
        template <typename T = QByteArray> static auto bytes(quint32 integer) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");

            T t(4, 0);
            t[0] = quint8(integer >> 24);
            t[1] = quint8(integer >> 16);
            t[2] = quint8(integer >> 8);
            t[3] = quint8(integer);
            return t;
        }
        static quint32 fromBytes(const QKnxByteStoreRef &data, quint16 index = 0)
        {
            if (data.size() - index < 4)
                return {};
            const auto bytes = data.bytes();
            return quint32(bytes[index]) << 24 | quint32(bytes[index + 1]) << 16
                | quint32(bytes[index + 2]) << 8 | bytes[index + 3];
        }

        template <typename T, std::size_t S = 0>
            static quint32 fromBytes(const T &data, quint16 index = 0)
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

            if (data.size() - index < 4)
                return {};
            return quint32(quint32(quint8(data[index])) << 24
                | quint32(quint8(data[index + 1])) << 16
                | quint32(quint8(data[index + 2])) << 8
                | quint8(data[index + 3]));
        }
    };

    struct QUint64 final
    {
        template <typename T = QByteArray> static auto bytes(quint64 integer) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");

            T t(8, 0);
            t[0] = quint8(integer >> 56);
            t[1] = quint8(integer >> 48);
            t[2] = quint8(integer >> 40);
            t[3] = quint8(integer >> 32);
            t[4] = quint8(integer >> 24);
            t[5] = quint8(integer >> 16);
            t[6] = quint8(integer >> 8);
            t[7] = quint8(integer);
            return t;
        }
        static quint64 fromBytes(const QKnxByteStoreRef &data, quint16 index = 0)
        {
            if (data.size() - index < 4)
                return {};
            const auto bytes = data.bytes();
            return quint64(bytes[index]) << 56
                | quint64(bytes[index + 1]) << 48
                | quint64(bytes[index + 2]) << 40
                | quint64(bytes[index + 3]) << 32
                | quint64(bytes[index + 4]) << 24
                | quint64(bytes[index + 5]) << 16
                | quint64(bytes[index + 6]) << 8
                | bytes[index + 7];
        }

        template <typename T, std::size_t S = 0>
            static quint64 fromBytes(const T &data, quint16 index = 0)
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

            if (data.size() - index < 8)
                return {};
            return quint64(quint64(quint8(data[index])) << 56
                | quint64(quint8(data[index + 1])) << 48
                | quint64(quint8(data[index + 2])) << 40
                | quint64(quint8(data[index + 3])) << 32
                | quint64(quint8(data[index + 4])) << 24
                | quint64(quint8(data[index + 5])) << 16
                | quint64(quint8(data[index + 6])) << 8
                | quint8(data[index + 7]));
        }
    };

    struct HostAddress final
    {
        template <typename T = QByteArray>
            static auto bytes(const QHostAddress &address) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");
            return QUint32::bytes(address.toIPv4Address());
        }

        template <typename T, std::size_t S = 0>
            static QHostAddress fromBytes(const T &data, quint16 index = 0)
        {
            if (data.size() - index < 4)
                return {};
            return QHostAddress(QUint32::fromBytes(data, index));
        }
    };
};

QT_END_NAMESPACE

#endif
