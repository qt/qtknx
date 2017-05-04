/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdeviceconfigurationacknowledge.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDeviceConfigurationAcknowledge::QKnxNetIpDeviceConfigurationAcknowledge(quint8 id,
        quint8 sequenceCounter, QKnxNetIp::Error status)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge)
{
    setConnectionHeader({ id, sequenceCounter, quint8(status) });
}

QKnxNetIpDeviceConfigurationAcknowledge::QKnxNetIpDeviceConfigurationAcknowledge(
        const QKnxNetIpConnectionHeaderFrame &other)
    : QKnxNetIpConnectionHeaderFrame(other)
{}

quint8 QKnxNetIpDeviceConfigurationAcknowledge::communicationChannelId() const
{
    return connectionHeader().communicationChannelId();
}

quint8 QKnxNetIpDeviceConfigurationAcknowledge::sequenceCounter() const
{
    return connectionHeader().sequenceCounter();
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
