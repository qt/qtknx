/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPFRAMEHEADER_H
#define QKNXNETIPFRAMEHEADER_H

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

class Q_KNX_EXPORT QKnxNetIpFrameHeader final : private QKnxByteStore
{
public:
    QKnxNetIpFrameHeader() = default;
    ~QKnxNetIpFrameHeader() override = default;

    explicit QKnxNetIpFrameHeader(quint16 code);
    QKnxNetIpFrameHeader(quint16 code, quint16 payloadSize);

    bool isValid() const;
    quint16 totalSize() const;

    quint16 payloadSize() const;
    void setPayloadSize(quint16 payloadSize);

    quint16 code() const;
    void setCode(quint16 code);

    QString toString() const override;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    static const quint8 HeaderSize10 = 0x06;
    static const quint8 KnxNetIpVersion = 0x10;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpFrameHeader fromBytes(const T &bytes, quint16 index)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        const qint32 availableSize = bytes.size() - index;
        if (availableSize < QKnxNetIpFrameHeader::HeaderSize10)
            return {};

        const quint16 code = QKnxUtils::QUint16::fromBytes(bytes, index + 2);
        const quint16 size = QKnxUtils::QUint16::fromBytes(bytes, index + 4);
        return QKnxNetIpFrameHeader(code, size - QKnxNetIpFrameHeader::HeaderSize10);
    }
};

QT_END_NAMESPACE

#endif
