/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiproutingindication.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingIndication::QKnxNetIpRoutingIndication(const QKnxCemiFrame &cemi)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::RoutingIndication)
{
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
}

QKnxNetIpRoutingIndication::QKnxNetIpRoutingIndication(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

QKnxCemiFrame QKnxNetIpRoutingIndication::cemi() const
{
    return QKnxCemiFrame::fromBytes(payloadRef(), 0, payloadRef().size());
}

void QKnxNetIpRoutingIndication::setCemi(const QKnxCemiFrame &cemi)
{
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
}

bool QKnxNetIpRoutingIndication::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return QKnxNetIpFrame::isValid() && size() >= 8
        && code() == QKnxNetIp::ServiceType::RoutingIndication;
}

QT_END_NAMESPACE
