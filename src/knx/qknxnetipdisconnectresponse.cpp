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

#include "qknxnetipdisconnectresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDisconnectResponse::QKnxNetIpDisconnectResponse(quint8 channelId, QKnxNetIp::Error status)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::DisconnectResponse)
{
    QKnxNetIpPayload payload(channelId);
    payload.setByte(1, quint8(status));
    setPayload(payload);
}

QKnxNetIpDisconnectResponse::QKnxNetIpDisconnectResponse(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

quint8 QKnxNetIpDisconnectResponse::channelId() const
{
    return payloadRef().byte(0);
}

QKnxNetIp::Error QKnxNetIpDisconnectResponse::status() const
{
    return QKnxNetIp::Error(payloadRef().byte(1));
}

bool QKnxNetIpDisconnectResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 8
        && code() == QKnxNetIp::ServiceType::DisconnectResponse;
}

QT_END_NAMESPACE
