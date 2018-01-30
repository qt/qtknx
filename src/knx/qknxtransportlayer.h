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

#ifndef QKNXTRANSPORTLAYER_H
#define QKNXTRANSPORTLAYER_H

#include <QtCore/qobject.h>

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxlinklayerdevice.h>
#include <QtKnx/qknxtpdu.h>
#include <QtKnx/qknxconnectioninfo.h>
#include <QtKnx/qknxdeviceconfiguration.h>

QT_BEGIN_NAMESPACE

class QKnxTransportLayerPrivate;
class Q_KNX_EXPORT QKnxTransportLayer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxTransportLayer)
    Q_DECLARE_PRIVATE(QKnxTransportLayer)

public:
    enum State : quint8
    {
        Connecting,
        Closed,
        OpenIdle,
        OpenWait,
    };
    State state() const;

    enum class Error : quint8
    {
        None,
        LinkLayer
    };
    Error error() const;
    QString errorMessage() const;

    enum class Broadcast : quint8
    {
        System = 0x00,
        Domain = 0x01
    };

    enum class Priority : quint8
    {
        System = 0x00,
        Normal = 0x01,
        Urgent = 0x02,
        Low = 0x03
    };

    explicit QKnxTransportLayer(QObject *parent = nullptr);
    QKnxTransportLayer(QKnxLinkLayerDevice *lld, QObject *parent = nullptr);
    QKnxTransportLayer(const QKnxDeviceConfiguration &dc, const QKnxConnectionInfo
            &ci, QObject *parent = nullptr);

    bool connect(const QKnxAddress &dest);
    void disconnect(const QKnxAddress &dest);
    bool isConnected(const QKnxAddress &dest);

    QKnxLinkLayerDevice *linkLayerDevice() const;
    void setLinkLayerDevice(QKnxLinkLayerDevice *lld);

    bool send(const QKnxAddress &dest, const QKnxTpdu &tpdu);
    bool sendConnected(const QKnxAddress &dest, const QKnxTpdu &tpdu);

    bool send(Broadcast broadcast, Priority priority, const QKnxTpdu &tpdu);

private:
    void setState(State state);
    void setError(Error error, const QString &message);
};

QT_END_NAMESPACE

#endif
