/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCONNECTIONSTATEREQUEST_H
#define QKNXNETIPCONNECTIONSTATEREQUEST_H

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionStateRequest final : private QKnxNetIpFrame
{
public:
    QKnxNetIpConnectionStateRequest() = default;
    ~QKnxNetIpConnectionStateRequest() override = default;

    QKnxNetIpConnectionStateRequest(quint8 channelId, const QKnxNetIpHPAI &controlEndpoint);

    quint8 channelId() const;
    QKnxNetIpHPAI controlEndpoint() const;

    bool isValid() const override;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE

#endif
