/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPDISCONNECTRESPONSE_H
#define QKNXNETIPDISCONNECTRESPONSE_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDisconnectResponse final : private QKnxNetIpFrame
{
public:
    QKnxNetIpDisconnectResponse() = default;
    ~QKnxNetIpDisconnectResponse() override = default;

    QKnxNetIpDisconnectResponse(quint8 channelId, QKnxNetIp::Error status);

    quint8 channelId() const;
    QKnxNetIp::Error status() const;

    bool isValid() const override;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE

#endif
