/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPFRAME_H
#define QKNXNETIPFRAME_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpFrame = QKnxNetIpPackage<QKnxNetIp::ServiceType, QKnxNetIpFrameHeader>;

struct QKnxNetIpFrameHelper
{
    template <typename T, std::size_t S = 0>
        static QKnxNetIpFrame fromBytes(const T &bytes, quint16 index, QKnxNetIp::ServiceType sType)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid() || header.code() != sType)
            return {};

        return QKnxNetIpFrame(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
            header.payloadSize()));
    }
};

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpFrame &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpFrame &package);

QT_END_NAMESPACE

#endif
