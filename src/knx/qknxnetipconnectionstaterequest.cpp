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

#include "qknxnetipconnectionstaterequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectionStateRequest::QKnxNetIpConnectionStateRequest(quint8 channelId,
        const QKnxNetIpHpai &controlEndpoint)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::ConnectionStateRequest)
{
    QKnxNetIpPayload payload(channelId);
    payload.setByte(1, 0x00); // reserved
    payload.appendBytes(controlEndpoint.bytes());
    setPayload(payload);
}

QKnxNetIpConnectionStateRequest::QKnxNetIpConnectionStateRequest(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

quint8 QKnxNetIpConnectionStateRequest::channelId() const
{
    return payloadRef().byte(0);
}

QKnxNetIpHpai QKnxNetIpConnectionStateRequest::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(payloadRef(), 2);
}

bool QKnxNetIpConnectionStateRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 16
        && code() == QKnxNetIp::ServiceType::ConnectionStateRequest;
}

QT_END_NAMESPACE
