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

#include "qknxnetipsearchresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpSearchResponse::QKnxNetIpSearchResponse(const QKnxNetIpHPAI &controlEndpoint,
    const QKnxNetIpDeviceDIB &deviceHardware, const QKnxNetIpServiceFamiliesDIB &supportedFamilies)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::SearchResponse)
{
    QKnxNetIpPayload payload;
    payload.setBytes(controlEndpoint.bytes());
    payload.appendBytes(deviceHardware.bytes());
    payload.appendBytes(supportedFamilies.bytes());
    setPayload(payload);
}

QKnxNetIpSearchResponse::QKnxNetIpSearchResponse(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

QKnxNetIpHPAI QKnxNetIpSearchResponse::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 0);
}

QKnxNetIpDeviceDIB QKnxNetIpSearchResponse::deviceHardware() const
{
    return QKnxNetIpDeviceDIB::fromBytes(payloadRef(), 8);
}

QKnxNetIpServiceFamiliesDIB QKnxNetIpSearchResponse::supportedFamilies() const
{
    return QKnxNetIpServiceFamiliesDIB::fromBytes(payloadRef(), 62);
}

bool QKnxNetIpSearchResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 70
        && code() == QKnxNetIp::ServiceType::SearchResponse;
}

QT_END_NAMESPACE
