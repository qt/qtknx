/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiproutinglostmessage.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingLostMessage::QKnxNetIpRoutingLostMessage(QKnxNetIp::DeviceState state,
        quint16 lostMessageCount)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::RoutingLostMessage)
{
    QKnxNetIpPayload payload((quint8) state);
    payload.appendBytes(QKnxUtils::QUint16::bytes(lostMessageCount));
    setPayload(payload);
}

QKnxNetIp::DeviceState QKnxNetIpRoutingLostMessage::deviceState() const
{
    return QKnxNetIp::DeviceState(payloadRef().byte(0));
}

quint16 QKnxNetIpRoutingLostMessage::lostMessageCount() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef(), 1);
}

bool QKnxNetIpRoutingLostMessage::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 10
        && code() == QKnxNetIp::ServiceType::RoutingLostMessage;
}

QT_END_NAMESPACE
