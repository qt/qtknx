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
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpFrameHeader final : private QKnxByteStore
{
    friend struct QKnxNetIpFrameHelper;

public:
    QKnxNetIpFrameHeader() = default;
    ~QKnxNetIpFrameHeader() override = default;

    explicit QKnxNetIpFrameHeader(QKnxNetIp::ServiceType code);
    QKnxNetIpFrameHeader(QKnxNetIp::ServiceType code, quint16 payloadSize);

    bool isValid() const;
    quint16 totalSize() const;

    quint8 protocolVersion() const;

    quint16 payloadSize() const;
    void setPayloadSize(quint16 payloadSize);

    QKnxNetIp::ServiceType code() const;
    void setCode(QKnxNetIp::ServiceType code);

    QString toString() const override;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    static constexpr const quint8 HeaderSize10 = 0x06;
    static constexpr const quint8 KnxNetIpVersion10 = 0x10;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpFrameHeader fromBytes(const T &bytes, quint16 index)
    {
        static_assert(is_type<T, QByteArray, QKnxByteStoreRef, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        const qint32 availableSize = bytes.size() - index;
        if (availableSize < QKnxNetIpFrameHeader::HeaderSize10)
            return {};

        if (QKnxUtils::QUint8::fromBytes(bytes, index + 1) != QKnxNetIpFrameHeader::KnxNetIpVersion10)
            return {};

        const quint16 code = QKnxUtils::QUint16::fromBytes(bytes, index + 2);
        if (!QKnxNetIp::isFrameType(QKnxNetIp::ServiceType(code)))
            return {};
        return QKnxNetIpFrameHeader(QKnxNetIp::ServiceType(code),
            QKnxUtils::QUint16::fromBytes(bytes, index + 4) - QKnxNetIpFrameHeader::HeaderSize10);
    }
};

QT_END_NAMESPACE

#endif
