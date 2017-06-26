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

#include "qknxnetipdisconnectrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDisconnectRequest::QKnxNetIpDisconnectRequest(quint8 channelId,
        const QKnxNetIpHPAI &controlEndpoint)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::DisconnectRequest)
{
    QKnxNetIpPayload payload(channelId);
    payload.setByte(1, 0x00); // reserved
    payload.appendBytes(controlEndpoint.bytes());
    setPayload(payload);
}

QKnxNetIpDisconnectRequest::QKnxNetIpDisconnectRequest(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

quint8 QKnxNetIpDisconnectRequest::channelId() const
{
    return payloadRef().byte(0);
}

QKnxNetIpHPAI QKnxNetIpDisconnectRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 2);
}

bool QKnxNetIpDisconnectRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 16
        && code() == QKnxNetIp::ServiceType::DisconnectRequest;
}

QT_END_NAMESPACE
