/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXLINKLAYERDEVICE_H
#define QKNXLINKLAYERDEVICE_H

#include <QtCore/qstring.h>
#include <QtCore/qiodevice.h>

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxconnectioninfo.h>
#include <QtKnx/qknxdeviceconfiguration.h>
#include <QtKnx/qknxtpdu.h>
#include <QtKnx/qknxlinklayerframe.h>

using QKnxLinkLayerFrame = QKnxTunnelFrame;

QT_BEGIN_NAMESPACE

class QKnxLinkLayerDevicePrivate;
class Q_KNX_EXPORT QKnxLinkLayerDevice : public QIODevice
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxLinkLayerDevice)
    Q_DECLARE_PRIVATE(QKnxLinkLayerDevice)

public:
    enum class State : quint8
    {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    };
    State state() const;

    enum class Error : quint8
    {
        NoError
    };
    Error error() const;
    QString errorMessage() const;

    QKnxLinkLayerDevice(QObject *parent = nullptr);
    ~QKnxLinkLayerDevice() = default;

    QKnxLinkLayerDevice(const QKnxDeviceConfiguration &dc, QObject *parent = nullptr);
    QKnxLinkLayerDevice(const QKnxDeviceConfiguration &dc, const QKnxConnectionInfo &ci,
            QObject *parent = nullptr);

    bool connect();
    void disconnect();

    QKnxDeviceConfiguration deviceConfig() const;
    void setDeviceConfig(const QKnxDeviceConfiguration &dc);

    QKnxConnectionInfo connectionInfo() const;
    void setConnectionInfo(const QKnxConnectionInfo &ci);

    bool send(const QKnxLinkLayerFrame &frame, bool waitForConfirmation = true);
    void send(const QKnxAddress &dest, const QKnxTpdu &tpdu, bool waitForConfirmation = true);

    bool isSequential() const override;

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    void setState(State state);
    void setError(Error error, const QString &message);
};


QT_END_NAMESPACE

#endif
