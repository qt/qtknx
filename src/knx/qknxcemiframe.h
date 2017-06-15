/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXCEMI_H
#define QKNXCEMI_H

#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxCemi final : private QKnxByteStore
{
public:
    QKnxCemi() = default;
    ~QKnxCemi() override = default;

    using QKnxByteStore::size;
    using QKnxByteStore::toString;

    using QKnxByteStore::ref;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    template <typename T, std::size_t S = 0>
        static QKnxCemi fromBytes(const T &type, quint16 index, quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        QKnxCemi cemi;
        auto begin = std::next(std::begin(type), index);
        cemi.setBytes(begin, std::next(begin, size));
        return cemi;
    }
};

QT_END_NAMESPACE

#endif
