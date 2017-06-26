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

#ifndef QKNXNETIPMANAGEMENTCLIENT_H
#define QKNXNETIPMANAGEMENTCLIENT_H

#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipclient.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpManagementClientPrivate;
using QKnxDeviceManagementFrame = QKnxCemiFrame;

class Q_KNX_EXPORT QKnxNetIpManagementClient final : public QKnxNetIpClient
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpManagementClient)
    Q_DECLARE_PRIVATE(QKnxNetIpManagementClient)

public:
    QKnxNetIpManagementClient(QObject *parent = nullptr);
    ~QKnxNetIpManagementClient() override = default;

    QKnxNetIpManagementClient(const QHostAddress &localAddress, QObject *parent = nullptr);
    QKnxNetIpManagementClient(const QHostAddress &localAddress, quint16 localPort,
        QObject *parent = nullptr);

    void sendDeviceManagementFrame(const QKnxDeviceManagementFrame &frame);

Q_SIGNALS:
    void receivedDeviceManagementFrame(QKnxDeviceManagementFrame frame);
};

QT_END_NAMESPACE

#endif
