/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipsearchrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpSearchRequest::QKnxNetIpSearchRequest(const QKnxNetIpHPAI &discoveryEndpoint)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::SearchRequest)
{
    QKnxNetIpPayload payload;
    payload.setBytes(discoveryEndpoint.bytes());
    setPayload(payload);
}

QKnxNetIpSearchRequest::QKnxNetIpSearchRequest(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

bool QKnxNetIpSearchRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 14
        && code() == QKnxNetIp::ServiceType::SearchRequest;
}

QKnxNetIpHPAI QKnxNetIpSearchRequest::discoveryEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 0);
}

QT_END_NAMESPACE
