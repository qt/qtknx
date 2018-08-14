/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#ifndef QKNXNETIPENDPOINTCONNECTION_H
#define QKNXNETIPENDPOINTCONNECTION_H

#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxnetipcri.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtNetwork/qudpsocket.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpEndpointConnectionPrivate;
class Q_KNX_EXPORT QKnxNetIpEndpointConnection : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpEndpointConnection)
    Q_DECLARE_PRIVATE(QKnxNetIpEndpointConnection)

public:
    enum State : quint8
    {
        Disconnected,
        Starting,
        Bound,
        Connecting,
        Connected,
        Disconnecting
    };
    Q_ENUM(State)
    QKnxNetIpEndpointConnection::State state() const;

    enum class Error : quint8
    {
        None,
        State,
        Network,
        NotIPv4,
        Acknowledge,
        Heartbeat,
        Cemi,
        Unknown = 0x80
    };
    Q_ENUM(Error)

    QString errorString() const;
    QKnxNetIpEndpointConnection::Error error() const;

    enum SequenceType : quint8
    {
        Send,
        Receive,
    };
    Q_ENUM(SequenceType)
    int sequenceCount(SequenceType type) const;

    enum class EndpointType : quint8
    {
        Data,
        Control
    };
    quint8 netIpHeaderVersion(EndpointType endpoint) const;

    QKnxNetIpEndpointConnection() = delete;
    virtual ~QKnxNetIpEndpointConnection() = 0;

    quint16 localPort() const;
    void setLocalPort(quint16 port);

    QHostAddress localAddress() const;
    void setLocalAddress(const QHostAddress &address);

    bool natAware() const;
    void setNatAware(bool isAware);

    quint32 heartbeatTimeout() const;
    void setHeartbeatTimeout(quint32 msec);

    QKnxByteArray supportedProtocolVersions() const;
    void setSupportedProtocolVersions(const QKnxByteArray &versions);

    void connectToHost(const QKnxNetIpHpai &controlEndpoint);
    void connectToHost(const QHostAddress &address, quint16 port);

    void disconnectFromHost();

protected:
    QKnxNetIpEndpointConnection(QKnxNetIpEndpointConnectionPrivate &dd, QObject *parent);

Q_SIGNALS:
    void connected();
    void disconnected();

    void stateChanged(QKnxNetIpEndpointConnection::State state);
    void errorOccurred(QKnxNetIpEndpointConnection::Error error, QString errorString);
};

QT_END_NAMESPACE

#endif
