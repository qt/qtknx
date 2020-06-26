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

#ifndef QKNXNETIPSERVERDESCRIPTIONAGENT_H
#define QKNXNETIPSERVERDESCRIPTIONAGENT_H

#include <QtCore/qobject.h>
#include <QtCore/qlist.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxnetipserverinfo.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpServerDescriptionAgentPrivate;
class Q_KNX_EXPORT QKnxNetIpServerDescriptionAgent final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpServerDescriptionAgent)
    Q_DECLARE_PRIVATE(QKnxNetIpServerDescriptionAgent)

public:
    enum class State : quint8
    {
        NotRunning,
        Starting,
        Running,
        Stopping
    };
    Q_ENUM(State)

    enum class Error : quint8
    {
        None,
        Network,
        NotIPv4,
        Timeout,
        Unknown = 0x80
    };
    Q_ENUM(Error)

    QKnxNetIpServerDescriptionAgent(QObject *parent = nullptr);
    ~QKnxNetIpServerDescriptionAgent();

    explicit QKnxNetIpServerDescriptionAgent(const QHostAddress &localAddress,
        QObject *parent = nullptr);
    QKnxNetIpServerDescriptionAgent(const QHostAddress &localAddress, quint16 localPort,
        QObject *parent = nullptr);

    QKnxNetIpServerDescriptionAgent::State state() const;
    QKnxNetIpServerDescriptionAgent::Error error() const;

    QString errorString() const;
    QKnxNetIpServerInfo serverDescription() const;

    quint16 localPort() const;
    void setLocalPort(quint16 port);

    QHostAddress localAddress() const;
    void setLocalAddress(const QHostAddress &address);

    int timeout() const;
    void setTimeout(int msec);

    bool natAware() const;
    void setNatAware(bool useNat);

    quint8 multicastTtl() const;
    void setMulticastTtl(quint8 ttl);

public Q_SLOTS:
    void start(const QKnxNetIpHpai &server);
    void start(const QKnxNetIpServerInfo &server);
    void start(const QHostAddress &address, quint16 port);
    void stop();

Q_SIGNALS:
    void started();
    void finished();

    void descriptionReceived(QKnxNetIpServerInfo server);
    void stateChanged(QKnxNetIpServerDescriptionAgent::State state);
    void errorOccurred(QKnxNetIpServerDescriptionAgent::Error error, QString errorString);

private:
    QKnxNetIpServerDescriptionAgent(QKnxNetIpServerDescriptionAgentPrivate &dd, QObject *parent);
};

QT_END_NAMESPACE

#endif
