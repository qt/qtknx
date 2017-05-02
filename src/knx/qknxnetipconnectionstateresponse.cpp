/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipconnectionstateresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectionStateResponse::QKnxNetIpConnectionStateResponse(quint8 channelId,
        QKnxNetIp::Error status)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::ConnectionStateResponse)
{
    switch (status) {
    case QKnxNetIp::Error::None:
    case QKnxNetIp::Error::ConnectionId:
    case QKnxNetIp::Error::DataConnection:
    case QKnxNetIp::Error::KnxConnection:
    {
        QKnxNetIpPayload payload(channelId);
        payload.setByte(1, quint8(status));
        setPayload(payload);
    };
    default: break;
    };
}

quint8 QKnxNetIpConnectionStateResponse::channelId() const
{
    return payloadRef().byte(0);
}

QKnxNetIp::Error QKnxNetIpConnectionStateResponse::status() const
{
    return QKnxNetIp::Error(payloadRef().byte(1));
}

bool QKnxNetIpConnectionStateResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 8;
}

QT_END_NAMESPACE
