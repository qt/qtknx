/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCONNECTIONSTATERESPONSE_H
#define QKNXNETIPCONNECTIONSTATERESPONSE_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionStateResponse final : public QKnxNetIpFrame
{
public:
    QKnxNetIpConnectionStateResponse() = default;
    ~QKnxNetIpConnectionStateResponse() override = default;

    QKnxNetIpConnectionStateResponse(quint8 channelId, QKnxNetIp::Error status);

    quint8 channelId() const;
    QKnxNetIp::Error status() const;

    bool isValid() const override;
};

QT_END_NAMESPACE

#endif
