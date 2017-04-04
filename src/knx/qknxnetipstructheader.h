/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSTRUCTHEADER_H
#define QKNXNETIPSTRUCTHEADER_H

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

class Q_KNX_EXPORT QKnxNetIpStructHeader final : private QKnxByteStore
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

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

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

    static QKnxNetIpStructHeader fromBytes(const quint8 *bytes, quint16 index)
    {
        const qint32 availableSize = strlen(reinterpret_cast<const char*> (bytes)) - index;
        if (availableSize < 1)
            return {}; // total size missing

        quint16 totalSize = quint8(bytes[0]);
        const quint8 headerSize = totalSize == 0xff ? 4 : 2;
        if (availableSize < headerSize)
            return {}; // total size and code missing

        if (headerSize == 4)
            totalSize = QKnxUtils::QUint16::fromBytes(bytes, index + 1);
        return QKnxNetIpStructHeader(bytes[headerSize - 1], totalSize - headerSize);
    }

    static QKnxNetIpStructHeader fromBytes(const QKnxByteStoreRef &store, quint16 index)
    {
        const qint32 availableSize = store.size() - index;
        if (availableSize < 1)
            return {}; // total size missing

        quint16 totalSize = quint8(store.bytes()[0]);
        const quint8 headerSize = totalSize == 0xff ? 4 : 2;
        if (availableSize < headerSize)
            return {}; // total size and code missing

        if (headerSize == 4)
            totalSize = QKnxUtils::QUint16::fromBytes(store, index + 1);
        return QKnxNetIpStructHeader(store.bytes()[headerSize - 1], totalSize - headerSize);
    }
};

QT_END_NAMESPACE

#endif
