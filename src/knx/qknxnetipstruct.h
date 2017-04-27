/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSTRUCT_H
#define QKNXNETIPSTRUCT_H

#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpStruct : public QKnxNetIpPackage<quint8, QKnxNetIpStructHeader>
{
    using Package = QKnxNetIpPackage<quint8, QKnxNetIpStructHeader>;

public:
    QKnxNetIpStruct() = default;
    ~QKnxNetIpStruct() override = default;

    QKnxNetIpStruct(const QKnxNetIpStruct &) = default;
    QKnxNetIpStruct &operator=(const QKnxNetIpStruct &) = default;

    using Package::bytes;
    template <typename T, std::size_t S = 0>
        static QKnxNetIpStruct fromBytes(const T &bytes, quint16 index)
    {
        auto header = QKnxNetIpStructHeader::fromBytes(bytes, index);
        if (!header.isValid())
            return {};

        return QKnxNetIpStruct(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
            header.payloadSize()));
    }

protected:
    using Package::Package;

    void setPayload(const QKnxNetIpPayload &payload) override
    {
        Package::setPayload(payload);
        setPayloadSize(payload.size());
    }
};

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpStruct &package);

QT_END_NAMESPACE

#endif
