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

#ifndef QKNXNETIPCONNECTIONHEADER_H
#define QKNXNETIPCONNECTIONHEADER_H

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpQKnxNetIpConnectionHeaderRef = QKnxByteStoreRef;

class Q_KNX_EXPORT QKnxNetIpConnectionHeader final : private QKnxByteStore
{
    using QKnxByteStore::QKnxByteStore;

public:
    QKnxNetIpConnectionHeader() = default;
    ~QKnxNetIpConnectionHeader() override = default;

    QKnxNetIpConnectionHeader(quint8 channelId, quint8 sequenceCount, quint8 serviceTypeSpecificValue = 0);

    bool isValid() const;

    quint8 channelId() const;
    void setChannelId(quint8 id);

    quint8 sequenceCount() const;
    void setSequenceCount(quint8 count);

    quint8 serviceTypeSpecificValue() const;
    void setServiceTypeSpecificValue(quint8 value);

    QKnxByteArray connectionTypeSpecificHeaderItems() const
    {
        return bytes(4, size() - 4);
    }

    void setConnectionTypeSpecificHeaderItems(const QKnxByteArray &items)
    {
        insertBytes(4, items);
        setByte(0, quint8(items.size()) + 4);
    }

    QString toString() const override;
    QKnxNetIpQKnxNetIpConnectionHeaderRef ref() const;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    static QKnxNetIpConnectionHeader fromBytes(const QKnxByteArray &bytes, quint16 index);

private:
    quint8 m_isValid = 0;
};

QT_END_NAMESPACE

#endif
