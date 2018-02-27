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

#include "demodatapoint.h"
#include "qmlknxdemo.h"
#include "utils.h"

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtKnx/QKnxAddress>
#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QNetworkInterface>

namespace {
    const double sliderSectionSize = 1 / 7.0;
    using DeviceNumbers = EtsDevelopmentBoard::DeviceNumbers;
}

QmlKnxDemo::QmlKnxDemo()
    : m_tunnel(this)
{
    checkLightStatusOn();

    if (auto blind = m_etsBoard.getBlind()) {
        connect(blind, &DemoBlindDataPoint::blindChange, this, &QmlKnxDemo::blindPositionChange);
        connect(blind, &DemoBlindDataPoint::blindFullyUp, this, &QmlKnxDemo::blindFullyUp);
        connect(blind, &DemoBlindDataPoint::blindFullyDown, this, &QmlKnxDemo::blindFullyDown);
    }

    if (auto rocker = m_etsBoard.getRocker())
        connect(rocker, &DemoRockerDataPoint::rockerChange, this, &QmlKnxDemo::rockerChange);

    if (auto colorLed = m_etsBoard.getColorLed())
        connect(colorLed, &DemoColorLed::colorChange, this, &QmlKnxDemo::colorLedChange);
}

void QmlKnxDemo::toggleLight(int light)
{
    if (auto dpt = static_cast<DemoSwitchDataPoint *>(m_etsBoard.getGroupObject(light))) {
        dpt->uiToggle();
        sendGroupValueWrite(m_tunnel, m_etsBoard.getAddress(light), dpt->bytes());
    }
}

void QmlKnxDemo::colorSwitch(double value)
{
    QColor colorAtStart = colorState;
    if (value < (sliderSectionSize / 2)
            && colorState != Qt::black)
        colorState = Qt::black;

    else if ((value > (sliderSectionSize / 2))
             && value < (3 * sliderSectionSize / 2)
             && (colorState != Qt::white))
        colorState = Qt::white;

    else if ((value > (3 * sliderSectionSize / 2))
             && value < (5 * sliderSectionSize / 2)
             && (colorState != Qt::red))
        colorState = Qt::red;

    else if ((value > (5 * sliderSectionSize / 2))
             && value < (7 * sliderSectionSize / 2)
             && (colorState != Qt::magenta))
        colorState = Qt::magenta;

    else if ((value > (7 * sliderSectionSize / 2))
             && value < (9 * sliderSectionSize / 2)
             && (colorState != Qt::blue))
        colorState = Qt::blue;

    else if ((value > (9 * sliderSectionSize / 2))
             && value < (11 * sliderSectionSize / 2)
             && (colorState != Qt::cyan))
        colorState = Qt::cyan;

    else if (value > (11 * sliderSectionSize / 2)
             && value < (13 * sliderSectionSize / 2)
             && (colorState != Qt::green))
        colorState = Qt::green;

    else if (value > ((6 * sliderSectionSize) + (sliderSectionSize / 2))
             && (colorState != Qt::yellow))
        colorState = Qt::yellow;

    if (colorAtStart != colorState) {
        if (auto led = m_etsBoard.getColorLed()) {
            led->setCurrentColor(colorState);
            sendColorLedGroupValueWriteFrames(m_tunnel, led);
        }
    }
}

void QmlKnxDemo::toggleMiddleRight()
{
    auto light = DeviceNumbers::MIDDLE_RIGHT;
    if (auto dpt = static_cast<DemoSwitchDataPoint *>(m_etsBoard.getGroupObject(light))) {
        dpt->uiToggle();
        sendGroupValueWrite(m_tunnel, m_etsBoard.getAddress(light), dpt->bytes());
    }
}

void QmlKnxDemo::toggleMiddleLeft()
{
    auto led = m_etsBoard.getColorLed();
    if (!led)
        return;

    if (!led->isOn()) {
        // set green color in dpt by default...
        led->setCurrentColor(QColor(0, 255, 0));
        // send request to ets board
        sendColorLedGroupValueWriteFrames(m_tunnel, led);
    } else {
        led->setCurrentColor(QColor(0, 0, 0));
        sendColorLedGroupValueWriteFrames(m_tunnel, led);
    }
}

QString QmlKnxDemo::getColor() const
{
    if (auto obj = m_etsBoard.getColorLed())
        return obj->currentColor().name();
    return {};
}

int QmlKnxDemo::getRockerPosition() const
{
    if (auto rocker = m_etsBoard.getRocker())
        return rocker->position();
    return 0;
}

bool QmlKnxDemo::getLightState(int lightNum) const
{
    if (auto dpt = static_cast<DemoSwitchDataPoint *>(m_etsBoard.getGroupObject(lightNum)))
        return dpt->isOn();
    return false;
}

int QmlKnxDemo::getBlindPosition() const
{
    if (auto blind = m_etsBoard.getBlind())
        return blind->position();
    return 0;
}

void QmlKnxDemo::moveBlindUp()
{
    if (auto dpt = m_etsBoard.getBlind()) {
        sendGroupValueWrite(m_tunnel, m_etsBoard.getAddress(DeviceNumbers::E7), dpt->moveUpBytes(),
            0xac);
    }
}

void QmlKnxDemo::moveBlindDown()
{
    if (auto dpt = m_etsBoard.getBlind()) {
        sendGroupValueWrite(m_tunnel, m_etsBoard.getAddress(DeviceNumbers::E7), dpt->moveDownBytes(),
            0xac);
    }
}

void QmlKnxDemo::stopBlind()
{
    auto index = DeviceNumbers::E16;
    if (auto dpt = static_cast<DemoBlindDataPoint *>(m_etsBoard.getGroupObject(index)))
        sendGroupValueWrite(m_tunnel, m_etsBoard.getAddress(index), dpt->stopBytes(), 0xac);
}

void QmlKnxDemo::handleFrame(const QKnxLinkLayerFrame &frame)
{
    auto dstAdd = frame.destinationAddress().toString();

    if (QKnxLinkLayerFrame::MessageCode::DataConfirmation == frame.messageCode()
        || QKnxLinkLayerFrame::MessageCode::DataIndication == frame.messageCode()) {
        // data indication come from pushing buttons on the Board
        // data confirmation are responses to App UI requests
        bool boardStateChanged = false;
        const QKnxAddress dstAddress = frame.destinationAddress();
        if (m_etsBoard.exists(dstAddress))
            boardStateChanged = m_etsBoard.getGroupObject(dstAddress)->updateDataPointState(frame);

        if (boardStateChanged)
            emit boardUpdate(m_etsBoard.getIndex(dstAddress));
    }
}

static QHostAddress obtainLocalAddress()
{
    auto addresses = QNetworkInterface::allAddresses();
    for (auto address : qAsConst(addresses)) {
        if (address.isLoopback() || address.protocol() != QAbstractSocket::IPv4Protocol)
            continue;
        return address;
    }
    return QHostAddress::LocalHost; // no local address found!
}

QmlKnxTunnel::QmlKnxTunnel(QmlKnxDemo *demo)
{
    connect(&m_connection, &QKnxNetIpTunnelConnection::stateChanged,
            this, &QmlKnxTunnel::handleConnection);
    connect(&m_connection, &QKnxNetIpTunnelConnection::receivedTunnelFrame,
            demo, &QmlKnxDemo::handleFrame);
    connect(&m_timer, &QTimer::timeout, this, &QmlKnxTunnel::send);

    auto newAddress = obtainLocalAddress();
    m_connection.setLocalAddress(newAddress);
    m_discoveryAgent.setLocalAddress(newAddress);

    // Automatic KNXnet/Ip router discovery. Connects to first router found
    static auto handleDiscoveredServer = [&](const QKnxNetIpServerInfo &info) {
        if (!m_connected) {
            m_ip = info.controlEndpointAddress();
            m_port = info.controlEndpointPort();
            // establish the knx tunnel
            reconnect();
            m_connected = true;
        }
    };
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered, this,
            handleDiscoveredServer);
    autoDiscoverKnxServers();
}

void QmlKnxTunnel::sendTunnelFrame(const QKnxLinkLayerFrame &frame)
{
    if (frame.size() == 0)
        return;

    if (m_frames.size() == 0)
        m_timer.start(200);
    m_frames.enqueue(frame);
}

void QmlKnxTunnel::reconnect()
{
    m_connection.connectToHost(m_ip, m_port);
}

void QmlKnxTunnel::autoDiscoverKnxServers()
{
    m_discoveryAgent.stop();
    m_discoveryAgent.start();
}

void QmlKnxTunnel::changeIp(const QString &ip, quint16 port)
{
    m_ip = ip;
    m_port = port;
    m_connection.disconnectFromHost();
}

void QmlKnxTunnel::send()
{
    m_connection.sendTunnelFrame(m_frames.dequeue());
    if (m_frames.size() == 0)
        m_timer.stop();
}

void QmlKnxTunnel::handleConnection(QKnxNetIpEndpointConnection::State state)
{
    switch (state) {
    case QKnxNetIpEndpointConnection::State::Disconnected:
        autoDiscoverKnxServers();
        break;
    default:
        break;
    }
}

void QmlKnxDemo::changeTunnelIp(const QString &ipStr, quint16 port)
{
    m_tunnel.changeIp(ipStr, port);
}

void QmlKnxDemo::checkLightStatusOn()
{
    initBoard(m_etsBoard, m_tunnel);
}

void QmlKnxDemo::test()
{
    // red
    QKnxByteArray data { 00, 0xbc, 0xe0, 11, 05, 00, 0x0c, 01, 00, 81 };
    handleFrame(QKnxLinkLayerFrame::fromBytes(data, 0, data.size()));

    // blue
    data = { 00, 0xbc, 0xe0, 11, 05, 00, 0x0e, 01, 00, 81 };
    handleFrame(QKnxLinkLayerFrame::fromBytes(data, 0, data.size()));

    // green
    data = { 00, 0xbc, 0xe0, 11, 05, 00, 0x0d, 01, 00, 81 };
    handleFrame(QKnxLinkLayerFrame::fromBytes(data, 0, data.size()));
}
