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

#include "qknxnetiproutingindication.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingIndication::QKnxNetIpRoutingIndication(const QKnxCemiFrame &cemi)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::RoutingIndication)
{
    setCemi(cemi);
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
    setPayload(QKnxNetIpPayload::fromBytes(cemi.bytes(), 0, cemi.size()));
}

bool QKnxNetIpRoutingIndication::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return QKnxNetIpFrame::isValid() && size() >= 8
        && code() == QKnxNetIp::ServiceType::RoutingIndication;
}

QT_END_NAMESPACE
