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

#include "qknxnetipclient_p.h"
#include "qknxnetipdeviceconfigurationrequest.h"
#include "qknxnetipmanagementclient.h"

QT_BEGIN_NAMESPACE

class QKnxNetIpManagementClientPrivate : public QKnxNetIpClientPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpManagementClient)

public:
    QKnxNetIpManagementClientPrivate(const QHostAddress &a, quint16 p)
        : QKnxNetIpClientPrivate(a, p, QKnxNetIpCri(QKnxNetIp::ConnectionType::DeviceManagement),
            3, QKnxNetIp::DeviceConfigurationRequestTimeout)
    {}

    void process(const QKnxDeviceManagementFrame &frame) override
    {
        Q_Q(QKnxNetIpManagementClient);
        emit q->receivedDeviceManagementFrame(frame);
    }
};

QKnxNetIpManagementClient::QKnxNetIpManagementClient(QObject *parent)
    : QKnxNetIpManagementClient({ QHostAddress::LocalHost }, 0, parent)
{}

QKnxNetIpManagementClient::QKnxNetIpManagementClient(const QHostAddress &localAddress, QObject *o)
    : QKnxNetIpManagementClient(localAddress, 0, o)
{}

QKnxNetIpManagementClient::QKnxNetIpManagementClient(const QHostAddress &a, quint16 p, QObject *o)
    : QKnxNetIpClient(*new QKnxNetIpManagementClientPrivate(a, p), o)
{}

void QKnxNetIpManagementClient::sendDeviceManagementFrame(const QKnxDeviceManagementFrame &frame)
{
    if (state() != State::Connected)
        return;

    Q_D(QKnxNetIpManagementClient);
    d->sendDeviceConfigurationRequest(frame);
}

QT_END_NAMESPACE
