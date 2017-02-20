/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxtypecheck.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

struct QKnxUtils
{
    struct Integer
    {
        template <typename T, std::size_t S = 0> static quint16 quint16FromArray(const T &data)
        {
            QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>();

            if (data.size() < 2)
                return {};
            return quint16(quint16(data[0]) << 8 | data[1]);
        }

        template <typename T> static auto quint16ToArray(const quint16 integer) -> decltype(T())
        {
            QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>();

            T t(2, Qt::Uninitialized);
            t[0] = quint8(integer >> 8);
            t[1] = quint8(integer);
            return t;
        }
    };

    struct HostAddress
    {
        template <typename T, std::size_t S = 0> static QHostAddress fromArray(const T &data)
        {
            QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>();

            if (data.size() < 4)
                return {};
            return QHostAddress(quint32(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]));
        }

        template <typename T> static auto toArray(const QHostAddress &address) -> decltype(T())
        {
            QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>();

            T t(4, Qt::Uninitialized);
            auto addr = address.toIPv4Address();
            t[0] = quint8(addr >> 24);
            t[1] = quint8(addr >> 16);
            t[2] = quint8(addr >> 8);
            t[3] = quint8(addr);
            return t;
        }
    };
};
