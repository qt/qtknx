/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdescriptionrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDescriptionRequest::QKnxNetIpDescriptionRequest(const QKnxNetIpHPAI & controlEndpoint)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::DescriptionRequest)
{
    QKnxNetIpPayload payload;
    payload.setBytes(controlEndpoint.bytes());
    setPayload(payload);
}

QKnxNetIpDescriptionRequest::QKnxNetIpDescriptionRequest(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

bool QKnxNetIpDescriptionRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 14
        && code() == QKnxNetIp::ServiceType::DescriptionRequest;
}

QKnxNetIpHPAI QKnxNetIpDescriptionRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 0);
}

QT_END_NAMESPACE
