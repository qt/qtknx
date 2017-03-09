/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipsearchrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpSearchRequest::QKnxNetIpSearchRequest(const QKnxNetIpHPAI &discoveryEndpoint)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::SearchRequest))
{
    QKnxNetIpPayload payload;
    payload.setBytes(discoveryEndpoint.bytes());
    setPayload(payload);
}

QKnxNetIpSearchRequest::QKnxNetIpSearchRequest(const QHostAddress &address, quint16 port)
    : QKnxNetIpSearchRequest(QKnxNetIpHPAI { address, port })
{
}

bool QKnxNetIpSearchRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 18;
}

QKnxNetIpHPAI QKnxNetIpSearchRequest::discoveryEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payload().bytes(), 0);
}

QT_END_NAMESPACE
