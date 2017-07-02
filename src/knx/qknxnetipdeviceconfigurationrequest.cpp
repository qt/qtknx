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

#include "qknxnetipdeviceconfigurationrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDeviceConfigurationRequest::QKnxNetIpDeviceConfigurationRequest(quint8 id,
        quint8 sequenceCount, const QKnxCemiFrame &cemi)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::DeviceConfigurationRequest)
{
    setConnectionHeader({ id, sequenceCount });
    setPayload(QKnxNetIpPayload::fromBytes(cemi.bytes(), 0, cemi.size()));
}

QKnxNetIpDeviceConfigurationRequest::QKnxNetIpDeviceConfigurationRequest(
        const QKnxNetIpConnectionHeaderFrame &other)
    : QKnxNetIpConnectionHeaderFrame(other)
{}

quint8 QKnxNetIpDeviceConfigurationRequest::channelId() const
{
    return connectionHeader().channelId();
}

quint8 QKnxNetIpDeviceConfigurationRequest::sequenceCount() const
{
    return connectionHeader().sequenceCount();
}

QKnxCemiFrame QKnxNetIpDeviceConfigurationRequest::cemi() const
{
    auto ref = payloadRef(connectionHeaderSize());
    return QKnxCemiFrame::fromBytes(ref.bytes<QByteArray>(), 0, ref.size());
}

bool QKnxNetIpDeviceConfigurationRequest::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return QKnxNetIpConnectionHeaderFrame::isValid() && size() >= 12
        && code() == QKnxNetIp::ServiceType::DeviceConfigurationRequest;
}

QT_END_NAMESPACE
