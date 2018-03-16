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

#include <QtCore/QTimer>
#include <QtGui/QColor>
#include <QtKnx/QKnx1Bit>
#include <QtKnx/QKnxTpdu>
#include <QtKnx/QKnxTpduFactory>
#include <QtKnx/QKnxLinkLayerFrame>

const QKnxByteArray DemoDataPoint::BytesOn { 0x01 };
const QKnxByteArray DemoDataPoint::BytesOff { 0x00 };

bool DemoSwitchDataPoint::updateDataPointState(const QKnxLinkLayerFrame &frame)
{
    m_state = frame.serviceInformation().endsWith(1);
    return true;
}

void DemoSwitchDataPoint::uiToggle()
{
    m_state = !m_state;
}

bool DemoSwitchDataPoint::isOn() const
{
    return m_state;
}

QKnxByteArray DemoSwitchDataPoint::bytes() const
{
    if (m_state)
        return DemoDataPoint::BytesOn;
    return DemoDataPoint::BytesOff;
}

bool DemoColorLed::updateDataPointState(const QKnxLinkLayerFrame &frame)
{
    bool notifyUI = false;
    numMsgConfirmation++;
    if (frame.messageCode() == QKnxLinkLayerFrame::MessageCode::DataConfirmation) {
        // response to request from UI
        // wait for three color confirmation to signal UI
        if (numMsgConfirmation == 3)
            notifyUI = true;
    } else if (frame.messageCode() == QKnxLinkLayerFrame::MessageCode::DataIndication) {
        // response to request from Board
        notifyUI = true;
    }

    QKnxSwitch dpt;
    dpt.setBytes(frame.tpdu().data(), 0, 1);
    int value = (dpt.value() == QKnxSwitch::State::On ? 255 : 0);

    auto dstAddress = frame.destinationAddress();
    if (dstAddress == QKnxAddress::createGroup(0, 0, 12))
        m_stateColor.setRed(value);

    else if (dstAddress == QKnxAddress::createGroup(0, 0, 13))
        m_stateColor.setGreen(value);

    else if (dstAddress == QKnxAddress::createGroup(0, 0, 14))
        m_stateColor.setBlue(value);

    if (notifyUI) {
        numMsgConfirmation = 0;
        emit colorChange(m_stateColor);
    }
    return false;
}

QKnxByteArray DemoColorLed::redBytes() const
{
    if (m_stateColor.red() > 0)
        return DemoDataPoint::BytesOn;
    return DemoDataPoint::BytesOff;
}

QKnxByteArray DemoColorLed::greenBytes() const
{
    if (m_stateColor.green() > 0)
        return DemoDataPoint::BytesOn;
    return DemoDataPoint::BytesOff;
}

QKnxByteArray DemoColorLed::blueBytes() const
{
    if (m_stateColor.blue() > 0)
        return DemoDataPoint::BytesOn;
    return DemoDataPoint::BytesOff;
}

QColor DemoColorLed::currentColor() const
{
    return m_stateColor;
}

void DemoColorLed::setCurrentColor(QColor color)
{
    m_stateColor = color;
}

bool DemoColorLed::isOn() const
{
    return (m_stateColor != Qt::black);
}

bool DemoRockerDataPoint::updateDataPointState(const QKnxLinkLayerFrame &frame)
{
    auto bytes = frame.serviceInformation();
    QString s = QByteArray((const char *) bytes.constData(), bytes.size()).mid(20, 4);
    bool ok;
    setPosition(s.toInt(&ok, 16));
    emit rockerChange(m_position);
    return false;
}

void DemoBlindDataPoint::chooseTimeoutHandle(bool blindUp)
{
    if (blindUp) {
        disconnect(&m_timer, &QTimer::timeout, this, &DemoBlindDataPoint::moveBlindDown);
        connect(&m_timer, &QTimer::timeout, this, &DemoBlindDataPoint::moveBlindUp);
    } else {
        disconnect(&m_timer, &QTimer::timeout, this, &DemoBlindDataPoint::moveBlindUp);
        connect(&m_timer, &QTimer::timeout, this, &DemoBlindDataPoint::moveBlindDown);
    }
}

bool DemoBlindDataPoint::updateDataPointState(const QKnxLinkLayerFrame &frame)
{
    auto tpdu = frame.tpdu();
    if (frame.destinationAddress().toString() == "1/0/0") {
        stopBlind();

        QKnx1Bit dpt;
        dpt.setBytes(frame.tpdu().data(), 0, 1);

        chooseTimeoutHandle(dpt.bit());
        reenableTimer();
    } else if (frame.destinationAddress().toString() == "1/1/0") {
        stopBlind();
    }
    return false;
}

void DemoBlindDataPoint::reenableTimer()
{
    if (!m_timer.isActive())
        m_timer.start(50);
}

void DemoBlindDataPoint::moveBlindUp()
{
    if (m_position == 70) {
        stopBlind();
        emit blindFullyUp();
        return;
    }
    ++m_position;
    emit blindChange(m_position);
}

void DemoBlindDataPoint::moveBlindDown()
{
    if (m_position > 0) {
        --m_position;
        emit blindChange(m_position);
        return;
    }
    stopBlind();
    emit blindFullyDown();
}

void DemoBlindDataPoint::stopBlind()
{
    if (m_timer.isActive())
        m_timer.stop();
}

QKnxByteArray DemoBlindDataPoint::moveUpBytes() const
{
    // send a 0 to move up
    return DemoDataPoint::BytesOff;
}

QKnxByteArray DemoBlindDataPoint::moveDownBytes() const
{
    // send a 1 to move down
    return DemoDataPoint::BytesOn;
}

QKnxByteArray DemoBlindDataPoint::stopBytes() const
{
    // send a 1 to stop
    return DemoDataPoint::BytesOn;
}
