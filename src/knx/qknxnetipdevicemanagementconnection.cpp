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

#include "qknxnetipendpointconnection_p.h"
#include "qknxnetipdeviceconfigurationrequest.h"
#include "qknxnetipdevicemanagementconnection.h"

QT_BEGIN_NAMESPACE

class QKnxNetIpDeviceManagementConnectionPrivate : public QKnxNetIpEndpointConnectionPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpDeviceManagementConnection)

public:
    QKnxNetIpDeviceManagementConnectionPrivate(const QHostAddress &address, quint16 port)
        : QKnxNetIpEndpointConnectionPrivate(address, port,
            QKnxNetIpCri(QKnxNetIp::ConnectionType::DeviceManagement), 3,
            QKnxNetIp::DeviceConfigurationRequestTimeout)
    {}

    void process(const QKnxCemiFrame &frame) override
    {
        Q_Q(QKnxNetIpDeviceManagementConnection);
        emit q->receivedDeviceManagementFrame(frame);
    }
};

QKnxNetIpDeviceManagementConnection::QKnxNetIpDeviceManagementConnection(QObject *parent)
    : QKnxNetIpDeviceManagementConnection({ QHostAddress::LocalHost }, 0, parent)
{}

QKnxNetIpDeviceManagementConnection::QKnxNetIpDeviceManagementConnection(const QHostAddress &addr,
        QObject *o)
    : QKnxNetIpDeviceManagementConnection(addr, 0, o)
{}

QKnxNetIpDeviceManagementConnection::QKnxNetIpDeviceManagementConnection(const QHostAddress &addr,
        quint16 port, QObject *obj)
    : QKnxNetIpEndpointConnection(*new QKnxNetIpDeviceManagementConnectionPrivate(addr, port), obj)
{}

bool QKnxNetIpDeviceManagementConnection::sendDeviceManagementFrame(const QKnxDeviceManagementFrame &frame)
{
    if (state() != State::Connected)
        return false;

    Q_D(QKnxNetIpDeviceManagementConnection);
    return d->sendDeviceConfigurationRequest(frame);
}

QT_END_NAMESPACE
