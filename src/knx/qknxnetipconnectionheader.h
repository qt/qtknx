/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCONNECTIONHEADER_H
#define QKNXNETIPCONNECTIONHEADER_H

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

using QKnxNetIpQKnxNetIpConnectionHeaderRef = QKnxByteStoreRef;

class Q_KNX_EXPORT QKnxNetIpConnectionHeader final : private QKnxByteStore
{
    using QKnxByteStore::QKnxByteStore;

public:
    QKnxNetIpConnectionHeader() = default;
    ~QKnxNetIpConnectionHeader() override = default;

    QKnxNetIpConnectionHeader(quint8 channelId, quint8 sequenceCounter,
                              quint8 serviceTypeSpecificValue = 0);

    bool isValid() const;

    quint8 channelId() const;
    void setChannelId(quint8 id);

    quint8 sequenceCounter() const;
    void setSequenceCounter(quint8 counter);

    quint8 serviceTypeSpecificValue() const;
    void setServiceTypeSpecificValue(quint8 value);

    template <typename T = std::vector<quint8>>
        auto connectionTypeSpecificHeaderItems() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");
        return bytes<T>(4, size() - 4);
    }
    template <typename T, std::size_t S = 0> void setConnectionTypeSpecificHeaderItems(const T &items)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");
        insertBytes(4, items);
        setByte(0, quint8(items.size()) + 4);
    }

    QString toString() const override;
    QKnxNetIpQKnxNetIpConnectionHeaderRef ref() const;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpConnectionHeader fromBytes(const T &bytes, quint16 index)
    {
        static_assert(is_type<T, QByteArray, QKnxByteStoreRef, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        const qint32 availableSize = bytes.size() - index;
        if (availableSize < 1)
            return {}; // total size missing

        const quint8 totalSize = QKnxUtils::QUint8::fromBytes(bytes, index);
        if (availableSize < totalSize)
            return {}; // header might be coruppted

        const quint8 channelId = QKnxUtils::QUint8::fromBytes(bytes, index + 1);
        const quint8 sequenceCounter = QKnxUtils::QUint8::fromBytes(bytes, index + 2);
        const quint8 serviceTypeSpecificValue = QKnxUtils::QUint8::fromBytes(bytes, index + 3);
        QKnxNetIpConnectionHeader header(channelId, sequenceCounter, serviceTypeSpecificValue);
        if (totalSize > 4) {
            auto begin = std::next(std::begin(bytes), index + 4);
            header.setConnectionTypeSpecificHeaderItems(std::vector<quint8>(begin, std::next(begin,
                totalSize - 4)));
        }
        return header;
    }

private:
    quint8 m_isValid = 0;
};

QT_END_NAMESPACE

#endif
