/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipsearchresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpSearchResponse::QKnxNetIpSearchResponse(const QKnxNetIpHPAI &controlEndpoint,
    const QKnxNetIpDeviceDIB &deviceHardware, const QKnxNetIpServiceFamiliesDIB &supportedFamilies)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::SearchResponse))
{
    QKnxNetIpPayload payload;
    payload.setBytes(controlEndpoint.bytes());
    payload.appendBytes(deviceHardware.bytes());
    payload.appendBytes(supportedFamilies.bytes());
    setPayload(payload);
}

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
    return QKnxNetIpFrame::isValid() && size() >= 74;
}

QT_END_NAMESPACE
