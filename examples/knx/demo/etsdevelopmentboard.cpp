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

#include "etsdevelopmentboard.h"
#include "demodatapoint.h"

EtsDevelopmentBoard::EtsDevelopmentBoard()
{
    //e1-e6
    quint8 indexMapKey = 1;
    for (quint8 i = 1; i < 7 ; ++i) // small white leds
        addDevice(indexMapKey++, 0, 0, i, QSharedPointer<DemoSwitchDataPoint>::create());
    //e7-e8
    auto blindDataPoint = QSharedPointer<DemoBlindDataPoint>::create();
    addDevice(7, 1, 0, 0, blindDataPoint); // blind up/down
    //skip e8 (because e7 and e8 are the same)
    addDevice(8, 1, 0, 0, blindDataPoint); // blind up/down
    //indexMapKey++;
    //e9 red led
    addDevice(9, 0, 0, 7, QSharedPointer<DemoSwitchDataPoint>::create());
    //e10 red led
    addDevice(10, 0, 0, 8, QSharedPointer<DemoSwitchDataPoint>::create());
    //e11 red led
    addDevice(11, 0, 0, 9, QSharedPointer<DemoSwitchDataPoint>::create());
    //e12 red led
    addDevice(12, 0, 0, 10, QSharedPointer<DemoSwitchDataPoint>::create());
    // e13
    auto colorLed = QSharedPointer<DemoColorLed>::create();
    addDevice(13, 0, 0, 12, colorLed); // left Red
    addDevice(13, 0, 0, 13, colorLed); // left Green
    addDevice(13, 0, 0, 14, colorLed); // left Blue
    // e14
    addDevice(14, 0, 0, 11, QSharedPointer<DemoSwitchDataPoint>::create()); // right white
    addDevice(15, 2, 0, 0, QSharedPointer<DemoRockerDataPoint>::create()); // Rocker
    // e16 turn off blind up/down
    addDevice(16, 1, 1, 0, blindDataPoint);
}

DemoDataPoint *EtsDevelopmentBoard::getGroupObject(int idx) const
{
    return m_dataPoints.value(idx).dpt.data();
}

DemoDataPoint *EtsDevelopmentBoard::getGroupObject(const QKnxAddress &address) const
{
    return getGroupObject(getIndex(address));
}

int EtsDevelopmentBoard::getIndex(const QKnxAddress &address) const
{
    return m_addressToDataPoints.value(address);
}

QKnxAddress EtsDevelopmentBoard::getAddress(int idx) const
{
    return m_dataPoints.value(idx).address;
}

bool EtsDevelopmentBoard::exists(const QKnxAddress &address) const
{
    return m_addressToDataPoints.contains(address);
}

DemoRockerDataPoint *EtsDevelopmentBoard::getRocker() const
{
    return static_cast<DemoRockerDataPoint *>(getGroupObject(DeviceNumbers::ROCKER));
}

DemoBlindDataPoint *EtsDevelopmentBoard::getBlind() const
{
    return static_cast<DemoBlindDataPoint *>(getGroupObject(DeviceNumbers::E7));
}

DemoColorLed *EtsDevelopmentBoard::getColorLed() const
{
    return static_cast<DemoColorLed *>(getGroupObject(DeviceNumbers::COLORLED));
}

void EtsDevelopmentBoard::addDevice(quint8 key, quint8 mainGroup, quint16 middleGroup,
    quint8 subGroup, const QSharedPointer<DemoDataPoint> &dpt)
{
    auto address = QKnxAddress::createGroup(mainGroup, middleGroup, subGroup);

    if (!m_dataPoints.contains(key))
        m_dataPoints.insert(key, { address, dpt });

    if (!m_addressToDataPoints.contains(address))
        m_addressToDataPoints.insert(address, key);
}
