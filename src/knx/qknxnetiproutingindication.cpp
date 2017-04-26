/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiproutingindication.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingIndication::QKnxNetIpRoutingIndication(const QKnxCemi &cemi)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::RoutingIndication))
{
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
}

QKnxCemi QKnxNetIpRoutingIndication::cemi() const
{
    return QKnxCemi::fromBytes(payloadRef(), 0, payloadRef().size());
}

void QKnxNetIpRoutingIndication::setCemi(const QKnxCemi &cemi)
{
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
}

QT_END_NAMESPACE
