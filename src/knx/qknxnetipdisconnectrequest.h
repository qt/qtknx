/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDisconnectRequest final : private QKnxNetIpFrame
{
public:
    QKnxNetIpDisconnectRequest() = default;
    ~QKnxNetIpDisconnectRequest() override = default;

    QKnxNetIpDisconnectRequest(quint8 channelId, const QKnxNetIpHPAI &controlEndpoint);

    quint8 channelId() const;
    QKnxNetIpHPAI controlEndpoint() const;

    bool isValid() const override;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
