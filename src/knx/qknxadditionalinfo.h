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

#ifndef QKNXADDITIONAL_H
#define QKNXADDITIONAL_H

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxAdditionalInfo final : private QKnxByteStore
{
    Q_GADGET

public:
    enum class Type : quint8
    {
        Reserved = 0x00,
        PlMediumInformation = 0x01,
        RfMediumInformation = 0x02,
        BusmonitorStatusInfo = 0x03,
        TimestampRelative = 0x04,
        TimeDelayUntilSending = 0x05,
        ExtendedRelativeTimestamp = 0x06,
        BiBatInformation = 0x07,
        RfMultiInformation = 0x08,
        PreambleAndPostamble = 0x09,
        RfFastAckInformation = 0x0a,
        ManufactorSpecificData = 0xfe,
        EscCode = 0xff
    };
    Q_ENUM(Type)
    QKnxAdditionalInfo::Type type() const;

    QKnxAdditionalInfo() = default;
    ~QKnxAdditionalInfo() override = default;

    QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QKnxByteArray &data);

    bool isValid() const;
    QString toString() const override;

    quint8 dataSize() const;
    QKnxByteArray data() const
    {
        return ref(2).bytes(0);
    }
    static qint32 expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize = nullptr);

    static QKnxAdditionalInfo fromBytes(const QKnxByteArray &bytes, quint16 index)
    {
        const qint32 availableSize = bytes.size() - index;
        if (availableSize < 2)
            return {}; // size missing

        quint16 size = QKnxUtils::QUint8::fromBytes(bytes, index + 1) + 2; // type + size => 2
        if (availableSize < size)
            return {};

        QKnxAdditionalInfo info;
        info.setBytes(std::next(std::begin(bytes), index), std::next(std::begin(bytes), index + size));
        return info;
    }

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxAdditionalInfo &info);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxAdditionalInfo &info);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxAdditionalInfo &info);

Q_DECLARE_TYPEINFO(QKnxAdditionalInfo::Type, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
