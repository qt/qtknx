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
#include <QtKnx/qknxnetipbytestore.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpPayload final : private QKnxNetIpByteStore
{
public:
    QKnxNetIpPayload() = default;
    ~QKnxNetIpPayload() = default;

    using QKnxNetIpByteStore::size;
    using QKnxNetIpByteStore::resize;
    using QKnxNetIpByteStore::toString;

    using QKnxNetIpByteStore::byte;
    using QKnxNetIpByteStore::bytes;
    using QKnxNetIpByteStore::setByte;
    using QKnxNetIpByteStore::setBytes;
    using QKnxNetIpByteStore::appendBytes;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpPayload fromBytes(const T &bytes, quint16 index, quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        QKnxNetIpPayload payload;
        payload.setBytes(bytes, index, size);
        return payload;
    }
};

QT_END_NAMESPACE
