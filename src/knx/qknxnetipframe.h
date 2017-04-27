/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPFRAME_H
#define QKNXNETIPFRAME_H

#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpFrame : public QKnxNetIpPackage<quint16, QKnxNetIpFrameHeader>
{
    using Package = QKnxNetIpPackage<quint16, QKnxNetIpFrameHeader>;

public:
    QKnxNetIpFrame() = default;
    ~QKnxNetIpFrame() override = default;

    QKnxNetIpFrame(const QKnxNetIpFrame &) = default;
    QKnxNetIpFrame &operator=(const QKnxNetIpFrame &) = default;

    using Package::bytes;
    template <typename T, std::size_t S = 0>
        static QKnxNetIpFrame fromBytes(const T &bytes, quint16 index)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid())
            return {};

        return QKnxNetIpFrame(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
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

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpFrame &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpFrame &package);

QT_END_NAMESPACE

#endif
