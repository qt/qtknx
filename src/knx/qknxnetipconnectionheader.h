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

class Q_KNX_EXPORT QKnxNetIpConnectionHeader final : private QKnxByteStore
{
    using QKnxByteStore::QKnxByteStore;

public:
    QKnxNetIpConnectionHeader() = default;
    ~QKnxNetIpConnectionHeader() override = default;

    QKnxNetIpConnectionHeader(quint8 communicationChannelId, quint8 sequenceCounter,
                              quint8 serviceTypeSpecificValue = 0);

    bool isValid() const;

    quint8 communicationChannelId() const;
    void setCommunicationChannelId(quint8 id);

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
    template <typename T, std::size_t S = 0> void setConnectionTypeSpecificHeaderItems(const T &bytes)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");
        insertBytes(4, bytes);
        setByte(0, quint8(bytes.size()) + 4);
    }

    QString toString() const override;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    template <typename T, std::size_t S = 0>
        static QKnxNetIpConnectionHeader fromBytes(const T &bytes, quint16 index)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        const qint32 availableSize = bytes.size() - index;
        if (availableSize < 1)
            return {}; // total size missing

        quint8 totalSize = bytes[index];
        if (availableSize < totalSize)
            return {}; // header might be coruppted

        QKnxNetIpConnectionHeader header(bytes[index + 1], bytes[index + 2], bytes[index + 3]);
        if (totalSize > 4) {
            T t(totalSize - 4, 0);
            std::copy_n(std::next(bytes.begin(), index + 4), totalSize - 4, std::begin(t));
            header.setConnectionTypeSpecificHeaderItems(t);
        }
        return header;
    }

    static QKnxNetIpConnectionHeader fromBytes(const QKnxByteStoreRef &store, quint16 index)
    {
        const qint32 availableSize = store.size() - index;
        if (availableSize < 1)
            return {}; // total size missing

        quint8 totalSize = store.bytes()[index];
        if (availableSize < totalSize)
            return {}; // header might be coruppted

        return QKnxNetIpConnectionHeader(&(*std::next(store.bytes(), index)), totalSize);
    }

private:
    quint8 m_isValid = 0;
};

QT_END_NAMESPACE

#endif
