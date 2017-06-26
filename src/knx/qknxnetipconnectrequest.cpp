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

#include "qknxnetipconnectrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectRequest::QKnxNetIpConnectRequest(const QKnxNetIpHPAI &controlEndpoint,
        const QKnxNetIpHPAI &dataEndpoint, const QKnxNetIpCRI &requestInformation)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::ConnectRequest)
{
    QKnxNetIpPayload payload;
    payload.setBytes(controlEndpoint.bytes());
    payload.appendBytes(dataEndpoint.bytes());
    payload.appendBytes(requestInformation.bytes());
    setPayload(payload);
}

QKnxNetIpConnectRequest::QKnxNetIpConnectRequest(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

QKnxNetIpHPAI QKnxNetIpConnectRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 0);
}

QKnxNetIpHPAI QKnxNetIpConnectRequest::dataEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 8);
}

QKnxNetIpCRI QKnxNetIpConnectRequest::requestInformation() const
{
    return QKnxNetIpCRI::fromBytes(payloadRef(), 16);
}

bool QKnxNetIpConnectRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 24
        && code() == QKnxNetIp::ServiceType::ConnectRequest;
}

QT_END_NAMESPACE
