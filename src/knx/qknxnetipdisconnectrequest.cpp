/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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
    return QKnxNetIpFrame::isValid() && size() == 16;
}

QT_END_NAMESPACE
