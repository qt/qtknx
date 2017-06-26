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

#ifndef QKNXNETIPCLIENT_H
#define QKNXNETIPCLIENT_H

#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipcri.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetipconnectionheaderframe.h>
#include <QtNetwork/qudpsocket.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpClientPrivate;

class Q_KNX_EXPORT QKnxNetIpClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpClient)
    Q_DECLARE_PRIVATE(QKnxNetIpClient)

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
    QKnxNetIpClient::State state() const;

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
    QKnxNetIpClient::Error error() const;

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

    QKnxNetIpClient() = delete;
    virtual ~QKnxNetIpClient() = 0;

    quint16 localPort() const;
    void setLocalPort(quint16 port);

    QHostAddress localAddress() const;
    void setLocalAddress(const QHostAddress &address);

    bool natAware() const;
    void setNatAware(bool isAware);

    quint32 heartbeatTimeout() const;
    void setHeartbeatTimeout(quint32 msec);

    QVector<quint8> supportedProtocolVersions() const;
    void setSupportedProtocolVersions(const QVector<quint8> &versions);

    void connectToHost(const QKnxNetIpHpai &controlEndpoint);
    void connectToHost(const QHostAddress &address, quint16 port);

    void disconnectFromHost();

protected:
    QKnxNetIpClient(QKnxNetIpClientPrivate &dd, QObject *parent);

Q_SIGNALS:
    void connected();
    void disconnected();

    void stateChanged(QKnxNetIpClient::State state);
    void errorOccurred(QKnxNetIpClient::Error error, QString errorString);
};

QT_END_NAMESPACE

#endif
