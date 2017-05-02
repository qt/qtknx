/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdeviceconfigurationrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDeviceConfigurationRequest::QKnxNetIpDeviceConfigurationRequest(quint8 id,
        quint8 sequenceCounter, const QKnxCemi &cemi)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::DeviceConfigurationRequest)
{
    setConnectionHeader({ id, sequenceCounter });
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
}

quint8 QKnxNetIpDeviceConfigurationRequest::communicationChannelId() const
{
    return connectionHeader().communicationChannelId();
}

quint8 QKnxNetIpDeviceConfigurationRequest::sequenceCounter() const
{
    return connectionHeader().sequenceCounter();
}

QKnxCemi QKnxNetIpDeviceConfigurationRequest::cemi() const
{
    return QKnxCemi::fromBytes(payloadRef(), 0, payloadRef().size());
}

QT_END_NAMESPACE
