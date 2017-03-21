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
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpStructHeader final : private QKnxNetIpByteStore
{
public:
    QKnxNetIpStructHeader() = default;
    ~QKnxNetIpStructHeader() override = default;

    explicit QKnxNetIpStructHeader(quint8 code);
    QKnxNetIpStructHeader(quint8 code, quint16 payloadSize);

    bool isValid() const;
    quint16 totalSize() const;

    quint16 payloadSize() const;
    void setPayloadSize(quint16 payloadSize);

    quint8 code() const;
    void setCode(quint8 code);

    QString toString() const override;

    using QKnxNetIpByteStore::size;
    using QKnxNetIpByteStore::byte;
    using QKnxNetIpByteStore::bytes;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpStructHeader fromBytes(const T &bytes, quint16 index)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        const qint32 availableSize = bytes.size() - index;
        if (availableSize < 1)
            return {}; // total size missing

        quint16 totalSize = quint8(bytes[index]);
        const quint8 headerSize = totalSize == 0xff ? 4 : 2;
        if (availableSize < headerSize)
            return {}; // total size and code missing

        if (headerSize == 4)
            totalSize = QKnxUtils::QUint16::fromBytes(bytes, index + 1);
        return QKnxNetIpStructHeader(quint8(bytes[index + headerSize - 1]), totalSize - headerSize);
    }
};

QT_END_NAMESPACE
