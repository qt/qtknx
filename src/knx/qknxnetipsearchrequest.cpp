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

#include "qknxnetipsearchrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpSearchRequest::QKnxNetIpSearchRequest(const QKnxNetIpHpai &discoveryEndpoint)
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

QKnxNetIpHpai QKnxNetIpSearchRequest::discoveryEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(payloadRef(), 0);
}

QT_END_NAMESPACE
