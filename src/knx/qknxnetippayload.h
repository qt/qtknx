/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPPAYLOAD_H
#define QKNXNETIPPAYLOAD_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxbytestore.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpPayloadRef = QKnxByteStoreRef;

class Q_KNX_EXPORT QKnxNetIpPayload final : private QKnxByteStore
{
public:
    QKnxNetIpPayload() = default;
    ~QKnxNetIpPayload() override = default;

    explicit QKnxNetIpPayload(quint8 byte);
    QKnxNetIpPayload(const quint8 *data, quint16 size);

    QKnxNetIpPayloadRef ref(quint16 index = 0) const;

    using QKnxByteStore::size;
    using QKnxByteStore::resize;
    using QKnxByteStore::toString;

    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;
    using QKnxByteStore::setByte;
    using QKnxByteStore::setBytes;
    using QKnxByteStore::appendBytes;
    using QKnxByteStore::insertBytes;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpPayload fromBytes(const T &type, quint16 index, quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        QKnxNetIpPayload payload;
        auto begin = std::next(std::begin(type), index);
        payload.setBytes(begin, std::next(begin, size));
        return payload;
    }
};

QT_END_NAMESPACE

#endif
