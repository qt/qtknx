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

#ifndef QKNXNPDU_H
#define QKNXNPDU_H

#include <QtCore/qbytearray.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxbytestore.h>


QT_BEGIN_NAMESPACE

class QKnxNpduFactory;

class Q_KNX_EXPORT QKnxNpdu final : private QKnxByteStore
{
    friend QKnxNpduFactory;

public:
    QKnxNpdu() = default;
    ~QKnxNpdu() override = default;

    explicit QKnxNpdu(quint8 byte);

    bool isValid();

    template <typename T, std::size_t S = 0>
        static QKnxNpdu fromBytes(const T &type, quint16 index, quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");
        QKnxNpdu npdu;
        auto begin = std::next(std::begin(type), index);
        npdu.setBytes(begin, std::next(begin, size));
        return npdu;
    }

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;
    using QKnxByteStore::toString;

protected:
    using QKnxByteStore::resize;
    using QKnxByteStore::setByte;
    using QKnxByteStore::setBytes;
    using QKnxByteStore::appendBytes;
    using QKnxByteStore::insertBytes;
    void appendAndUpdate(const QByteArray &data);
};

QT_END_NAMESPACE

#endif
