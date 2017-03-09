/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipcrd.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectResponse final : private QKnxNetIpFrame
{
public:
    QKnxNetIpConnectResponse() = default;

    QKnxNetIpConnectResponse(quint8 channelId,
                             QKnxNetIp::Error status,
                             const QKnxNetIpHPAI &dataEndpoint,
                             const QKnxNetIpCRD &responseData);

    explicit QKnxNetIpConnectResponse(QKnxNetIp::Error status);

    quint8 channelId() const;
    QKnxNetIp::Error status() const;
    QKnxNetIpHPAI dataEndpoint() const;
    QKnxNetIpCRD responseData() const;

    bool isValid() const override;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
