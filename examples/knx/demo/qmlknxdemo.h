/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMLKNXDEMO_H
#define QMLKNXDEMO_H

#include "etsdevelopmentboard.h"

#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtGui/QColor>
#include <QtKnx/QKnxNetIpServerDiscoveryAgent>
#include <QtKnx/QKnxNetIpTunnelConnection>
#include <QtKnx/QKnxTunnelFrame>

class QmlKnxDemo;
class QmlKnxTunnel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QmlKnxTunnel)

public:
    QmlKnxTunnel(QmlKnxDemo *demo);
    void sendTunnelFrame(const QKnxCemiFrame &frame);
    void changeIp(const QString &ip, quint16 port = 3671);

public slots:
    void handleConnection(QKnxNetIpEndpointConnection::State state);
    void send();

private:
    void reconnect();
    void autoDiscoverKnxServers();

private:
    QQueue<QKnxCemiFrame> m_frames;
    QKnxNetIpTunnelConnection m_connection;
    QKnxNetIpServerDiscoveryAgent m_discoveryAgent;

    QTimer m_timer;
    QHostAddress m_ip;
    quint16 m_port { 3671 };
    bool m_connected { false };
};

class QmlKnxDemo : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QmlKnxDemo)

public:
    QmlKnxDemo();

    Q_INVOKABLE void toggleMiddleRight();
    Q_INVOKABLE void toggleMiddleLeft();
    Q_INVOKABLE QString getColor() const;
    Q_INVOKABLE int getRockerPosition() const;
    Q_INVOKABLE int getBlindPosition() const;
    Q_INVOKABLE bool getLightState(int lightNum) const;

    Q_INVOKABLE void moveBlindUp();
    Q_INVOKABLE void moveBlindDown();
    Q_INVOKABLE void stopBlind();

    Q_INVOKABLE void colorSwitch(double value);
    Q_INVOKABLE void toggleLight(int light);

    Q_INVOKABLE void test();
    Q_INVOKABLE void changeTunnelIp(const QString &ipStr, quint16 port = 3671);

Q_SIGNALS:
    void boardUpdate(quint32 lightNum);
    void blindPositionChange(int position);
    void blindFullyDown();
    void blindFullyUp();
    void rockerChange(int position);
    void colorLedChange(QColor color);

public slots:
    void handleFrame(const QKnxTunnelFrame &frame);

private:
    void checkLightStatusOn();

private:
    QColor colorState;
    QmlKnxTunnel m_tunnel;
    EtsDevelopmentBoard m_etsBoard;
};

#endif
