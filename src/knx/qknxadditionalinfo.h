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

#ifndef QKNXADDITIONAL_H
#define QKNXADDITIONAL_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
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

    QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QByteArray &data);
    QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QVector<quint8> &data);

    bool isValid() const;
    QString toString() const override;

    quint8 dataSize() const;
    template <typename T = QByteArray> auto data() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        const auto &store = ref(2);
        T t(store.size(), 0);
        std::copy(std::begin(store), std::end(store), std::begin(t));
        return t;
    }
    static qint32 expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize = nullptr);

    template <typename T, std::size_t S = 0>
        static QKnxAdditionalInfo fromBytes(const T &bytes, quint16 index)
    {
        static_assert(is_type<T, QByteArray, QKnxByteStoreRef, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

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
