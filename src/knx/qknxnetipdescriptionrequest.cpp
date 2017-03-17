/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdescriptionrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDescriptionRequest::QKnxNetIpDescriptionRequest(const QKnxNetIpHPAI &discoveryEndpoint)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::DescriptionRequest))
{
    QKnxNetIpPayload payload;
    payload.setBytes(discoveryEndpoint.bytes());
    setPayload(payload);
}

QKnxNetIpDescriptionRequest::QKnxNetIpDescriptionRequest(const QHostAddress &address, quint16 port)
    : QKnxNetIpDescriptionRequest(QKnxNetIpHPAI { address, port })
{
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
