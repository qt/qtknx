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

#ifndef QKNXNETIPPAYLOAD_H
#define QKNXNETIPPAYLOAD_H

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
    explicit QKnxNetIpPayload(const QKnxByteArray &ba);
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
    using QKnxByteStore::replaceBytes;

    static QKnxNetIpPayload fromBytes(const QKnxByteArray &type, quint16 index, quint16 size)
    {
        QKnxNetIpPayload payload;
        auto begin = std::next(std::begin(type), index);
        payload.setBytes(begin, std::next(begin, size));
        return payload;
    }
};

QT_END_NAMESPACE

#endif
