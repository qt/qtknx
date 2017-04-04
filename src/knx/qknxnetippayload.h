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

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpPayload final : private QKnxByteStore
{
public:
    QKnxNetIpPayload() = default;
    ~QKnxNetIpPayload() override = default;

    using QKnxByteStore::size;
    using QKnxByteStore::resize;
    using QKnxByteStore::toString;

    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;
    using QKnxByteStore::setByte;
    using QKnxByteStore::setBytes;
    using QKnxByteStore::appendBytes;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpPayload fromBytes(const T &bytes, quint16 index, quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>, const quint8 *>::value, "Type not supported.");

        QKnxNetIpPayload payload;
        payload.setBytes(bytes, index, size);
        return payload;
    }
};

QT_END_NAMESPACE

#endif
