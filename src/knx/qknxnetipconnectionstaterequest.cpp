/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipconnectionstaterequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectionStateRequest::QKnxNetIpConnectionStateRequest(quint8 channelId,
        const QKnxNetIpHPAI &controlEndpoint)
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

QKnxNetIpHPAI QKnxNetIpConnectionStateRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 2);
}

bool QKnxNetIpConnectionStateRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 16
        && code() == QKnxNetIp::ServiceType::ConnectionStateRequest;
}

QT_END_NAMESPACE
