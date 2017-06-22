/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdeviceconfigurationacknowledge.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDeviceConfigurationAcknowledge::QKnxNetIpDeviceConfigurationAcknowledge(quint8 id,
        quint8 sequenceCount, QKnxNetIp::Error status)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge)
{
    setConnectionHeader({ id, sequenceCount, quint8(status) });
}

QKnxNetIpDeviceConfigurationAcknowledge::QKnxNetIpDeviceConfigurationAcknowledge(
        const QKnxNetIpConnectionHeaderFrame &other)
    : QKnxNetIpConnectionHeaderFrame(other)
{}

quint8 QKnxNetIpDeviceConfigurationAcknowledge::channelId() const
{
    return connectionHeader().channelId();
}

quint8 QKnxNetIpDeviceConfigurationAcknowledge::sequenceCount() const
{
    return connectionHeader().sequenceCount();
}

QKnxNetIp::Error QKnxNetIpDeviceConfigurationAcknowledge::status() const
{
    return QKnxNetIp::Error(connectionHeader().serviceTypeSpecificValue());
}

bool QKnxNetIpDeviceConfigurationAcknowledge::isValid() const
{
    return QKnxNetIpConnectionHeaderFrame::isValid() && size() == 10
        && code() == QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge;
}

QT_END_NAMESPACE
