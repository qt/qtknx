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

#include "qknxnetiproutingbusy.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingBusy::QKnxNetIpRoutingBusy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

QKnxNetIp::DeviceState QKnxNetIpRoutingBusy::deviceState() const
{
    return QKnxNetIp::DeviceState(m_frame.constData().value(1));
}

quint16 QKnxNetIpRoutingBusy::routingBusyWaitTime() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData(), 2);
}

quint16 QKnxNetIpRoutingBusy::routingBusyControl() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData(), 4);
}

bool QKnxNetIpRoutingBusy::isValid() const
{
    quint16 time = routingBusyWaitTime();
    if (time < 20 || time > 100)
        return false;
    return m_frame.isValid() && m_frame.size() == 12
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingBusy;
}

QKnxNetIpRoutingBusy::Builder QKnxNetIpRoutingBusy::builder()
{
    return QKnxNetIpRoutingBusy::Builder();
}


// -- QKnxNetIpRoutingBusy::Builder

QKnxNetIpRoutingBusy::Builder &
    QKnxNetIpRoutingBusy::Builder::setDeviceState(QKnxNetIp::DeviceState state)
{
    m_state = state;
    return *this;
}

QKnxNetIpRoutingBusy::Builder &
    QKnxNetIpRoutingBusy::Builder::setRoutingBusyWaitTime(quint16 waitTime)
{
    m_waitTime = (waitTime > 100 ? 100 : (waitTime < 20 ? 20 : waitTime));
    return *this;
}

QKnxNetIpRoutingBusy::Builder &
    QKnxNetIpRoutingBusy::Builder::setRoutingBusyControl(quint16 busyControl)
{
    m_busyControl = busyControl;
    return *this;
}

QKnxNetIpFrame QKnxNetIpRoutingBusy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::RoutingBusy, QKnxByteArray { 0x06, quint8(m_state) }
        + QKnxUtils::QUint16::bytes(m_waitTime) + QKnxUtils::QUint16::bytes(m_busyControl) };
}

QT_END_NAMESPACE
