/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiproutingbusy.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingBusy::QKnxNetIpRoutingBusy(QKnxNetIp::DeviceState state)
    : QKnxNetIpRoutingBusy(state, 100, 0x0000)
{}

QKnxNetIpRoutingBusy::QKnxNetIpRoutingBusy(QKnxNetIp::DeviceState state,
        quint8 routingBusyWaitTime, quint16 routingBusyControl)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::RoutingBusy)
{
    QKnxNetIpPayload payload((quint8) state);
    payload.setByte(1, routingBusyWaitTime);
    payload.appendBytes(QKnxUtils::QUint16::bytes(routingBusyControl));
    setPayload(payload);
}

QKnxNetIp::DeviceState QKnxNetIpRoutingBusy::deviceState() const
{
    return QKnxNetIp::DeviceState(payloadRef().byte(0));
}

quint8 QKnxNetIpRoutingBusy::routingBusyWaitTime() const
{
    return quint8(payloadRef().byte(1));
}

quint16 QKnxNetIpRoutingBusy::routingBusyControl() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef(), 2);
}

bool QKnxNetIpRoutingBusy::isValid() const
{
    quint8 time = routingBusyWaitTime();
    if (time < 20 || time > 100)
        return false;
    return QKnxNetIpFrame::isValid() && size() == 12 && code() == QKnxNetIp::ServiceType::RoutingBusy;
}

QT_END_NAMESPACE
