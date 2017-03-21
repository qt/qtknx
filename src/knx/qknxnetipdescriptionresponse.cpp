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

QVector<QKnxNetIpPayload> QKnxNetIpDescriptionResponse::optionalDibs() const
{
    quint16 index = 54;
    auto bytes = payload().bytes();
    auto header = QKnxNetIpStructHeader::fromBytes(bytes, index);

    QVector<QKnxNetIpPayload> dibs;
    while (index < bytes.size()) {
        header = QKnxNetIpStructHeader::fromBytes(bytes, index += header.totalSize());
        dibs.push_back(QKnxNetIpPayload::fromBytes(bytes, index, header.totalSize()));
    }
    return dibs;
}

bool QKnxNetIpDescriptionResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 64;
}

QT_END_NAMESPACE
