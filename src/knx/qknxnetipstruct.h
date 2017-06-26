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

#ifndef QKNXNETIPSTRUCT_H
#define QKNXNETIPSTRUCT_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

template <typename T>
using QKnxNetIpStruct = QKnxNetIpPackage<T, QKnxNetIpStructHeader<T>>;

struct QKnxNetIpStructHelper
{
    template <typename NetIpType, typename T, std::size_t S = 0>
        static QKnxNetIpStruct<NetIpType> fromBytes(const T &bytes, quint16 index, NetIpType nType)
    {
        auto header = QKnxNetIpStructHeader<NetIpType>::fromBytes(bytes, index);
        if (!header.isValid() || header.code() != nType)
            return {};

        return QKnxNetIpStruct<NetIpType>(header, QKnxNetIpPayload::fromBytes(bytes,
            index + header.size(), header.payloadSize()));
    }
};

using QKnxNetIpHostProtocolStruct = QKnxNetIpStruct<QKnxNetIp::HostProtocol>;
using QKnxNetIpConnectionTypeStruct = QKnxNetIpStruct<QKnxNetIp::ConnectionType>;
using QKnxNetIpDescriptionTypeStruct = QKnxNetIpStruct<QKnxNetIp::DescriptionType>;

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpHostProtocolStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpHostProtocolStruct &package);

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpConnectionTypeStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpConnectionTypeStruct &package);

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpDescriptionTypeStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpDescriptionTypeStruct &package);

QT_END_NAMESPACE

#endif
