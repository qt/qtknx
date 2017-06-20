/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipclient_p.h"
#include "qknxnetipdeviceconfigurationrequest.h"
#include "qknxnetipmanagementclient.h"

QT_BEGIN_NAMESPACE

class QKnxNetIpManagementClientPrivate : public QKnxNetIpClientPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpManagementClient)

public:
    QKnxNetIpManagementClientPrivate(const QHostAddress &a, quint16 p)
        : QKnxNetIpClientPrivate(a, p, QKnxNetIpCRI(QKnxNetIp::ConnectionType::DeviceManagement),
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
