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

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qtknxglobal.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

struct QKnxUtils final
{
    struct QUint8 final
    {
        static QKnxByteArray bytes(quint8 integer)
        {
            return { quint8(integer) };
        }

        static quint8 fromBytes(const QKnxByteArray &data, quint16 index = 0)
        {
            if (data.size() - index < 1)
                return {};
            return data.at(index);
        }
    };

    struct QUint16 final
    {
        static QKnxByteArray bytes(quint16 integer)
        {
            return { quint8(integer >> 8), quint8(integer) };
        }

        static quint16 fromBytes(const QKnxByteArray &data, quint16 index = 0)
        {
            if (data.size() - index < 2)
                return {};
            return quint16(quint16(data.at(index)) << 8 | data.at(index + 1));
        }
    };

    struct QUint32 final
    {
        static QKnxByteArray bytes(quint32 integer)
        {
            return { quint8(integer >> 24), quint8(integer >> 16), quint8(integer >> 8),
                quint8(integer) };
        }

        static quint32 fromBytes(const QKnxByteArray &data, quint16 index = 0)
        {
            if (data.size() - index < 4)
                return {};
            return quint32(quint32(data.at(index)) << 24
                | quint32(data.at(index + 1)) << 16
                | quint32(data.at(index + 2)) << 8
                | data.at(index + 3));
        }
    };

    struct QUint48 final
    {
        static QKnxByteArray bytes(quint48 integer)
        {
            return { quint8(integer >> 40), quint8(integer >> 32), quint8(integer >> 24),
                quint8(integer >> 16), quint8(integer >> 8), quint8(integer) };
        }

        static quint48 fromBytes(const QKnxByteArray &data, quint16 index = 0)
        {
            if (data.size() - index < 6)
                return {};
            return quint48(quint48(data.at(index + 0)) << 40
                | quint48(data.at(index + 1)) << 32
                | quint48(data.at(index + 2)) << 24
                | quint48(data.at(index + 3)) << 16
                | quint48(data.at(index + 4)) << 8
                | data.at(index + 5));
        }
    };

    struct QUint64 final
    {
        static QKnxByteArray bytes(quint64 integer)
        {
            return { quint8(integer >> 56), quint8(integer >> 48), quint8(integer >> 40)
                , quint8(integer >> 32), quint8(integer >> 24), quint8(integer >> 16)
                , quint8(integer >> 8), quint8(integer) };
        }

        static quint64 fromBytes(const QKnxByteArray &data, quint16 index = 0)
        {
            if (data.size() - index < 8)
                return {};
            return quint64(quint64(data.at(index)) << 56
                | quint64(data.at(index + 1)) << 48
                | quint64(data.at(index + 2)) << 40
                | quint64(data.at(index + 3)) << 32
                | quint64(data.at(index + 4)) << 24
                | quint64(data.at(index + 5)) << 16
                | quint64(data.at(index + 6)) << 8
                | data.at(index + 7));
        }
    };

    struct HostAddress final
    {
        static QKnxByteArray bytes(const QHostAddress &address)
        {
            return QUint32::bytes(address.toIPv4Address());
        }

        static QHostAddress fromBytes(const QKnxByteArray &data, quint16 index = 0)
        {
            if (data.size() - index < 4)
                return {};
            return QHostAddress(QUint32::fromBytes(data, index));
        }
    };
};

QT_END_NAMESPACE

#endif
