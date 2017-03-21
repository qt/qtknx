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
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::DisconnectRequest))
{
    QKnxNetIpPayload payload;
    payload.setByte(0, channelId);
    payload.setByte(1, 0x00); // reserved
    payload.appendBytes(controlEndpoint.bytes());
    setPayload(payload);
}

quint8 QKnxNetIpDisconnectRequest::channelId() const
{
    return payload().byte(0);
}

QKnxNetIpHPAI QKnxNetIpDisconnectRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payload().bytes(), 1);
}

bool QKnxNetIpDisconnectRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 16;
}

QT_END_NAMESPACE
