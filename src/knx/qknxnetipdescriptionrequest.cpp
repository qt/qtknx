/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdescriptionrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDescriptionRequest::QKnxNetIpDescriptionRequest(const QKnxNetIpHPAI & controlEndpoint)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::DescriptionRequest))
{
    QKnxNetIpPayload payload;
    payload.setBytes(controlEndpoint.bytes());
    setPayload(payload);
}

bool QKnxNetIpDescriptionRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 18;
}

QKnxNetIpHPAI QKnxNetIpDescriptionRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payload().bytes(), 0);
}

QT_END_NAMESPACE
