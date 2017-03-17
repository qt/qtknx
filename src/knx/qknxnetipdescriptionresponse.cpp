/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdescriptionresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDescriptionResponse::QKnxNetIpDescriptionResponse(const QKnxNetIpDeviceDIB &deviceHardware,
        const QKnxNetIpServiceFamiliesDIB &supportedFamilies)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::DescriptionResponse))
{
    QKnxNetIpPayload payload;
    payload.setBytes(deviceHardware.bytes());
    payload.appendBytes(supportedFamilies.bytes());
    setPayload(payload);
}

QKnxNetIpDeviceDIB QKnxNetIpDescriptionResponse::deviceHardware() const
{
    return QKnxNetIpDeviceDIB::fromBytes(payload().bytes(0, 54), 0);
}

QKnxNetIpServiceFamiliesDIB QKnxNetIpDescriptionResponse::supportedFamilies() const
{
    return QKnxNetIpServiceFamiliesDIB::fromBytes(payload().bytes(), 54);
}

bool QKnxNetIpDescriptionResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 66;
}

QT_END_NAMESPACE
