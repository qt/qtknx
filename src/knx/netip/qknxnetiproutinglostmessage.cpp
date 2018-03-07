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

#include "qknxnetiproutinglostmessage.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingLostMessage::QKnxNetIpRoutingLostMessage(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

QKnxNetIp::DeviceState QKnxNetIpRoutingLostMessage::deviceState() const
{
    return QKnxNetIp::DeviceState(m_frame.constData().value(1));
}

quint16 QKnxNetIpRoutingLostMessage::lostMessageCount() const
{
    return QKnxUtils::QUint16::fromBytes(m_frame.constData(), 2);
}

bool QKnxNetIpRoutingLostMessage::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 10
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingLostMessage;
}

QKnxNetIpRoutingLostMessage::Builder QKnxNetIpRoutingLostMessage::builder()
{
    return QKnxNetIpRoutingLostMessage::Builder();
}


// -- QKnxNetIpRoutingLostMessage::Builder

QKnxNetIpRoutingLostMessage::Builder &
    QKnxNetIpRoutingLostMessage::Builder::setDeviceState(QKnxNetIp::DeviceState state)
{
    m_state = state;
    return *this;
}

QKnxNetIpRoutingLostMessage::Builder &
    QKnxNetIpRoutingLostMessage::Builder::setLostMessageCount(quint16 messageCount)
{
    m_lostMessageCount = messageCount;
    return *this;
}

QKnxNetIpFrame QKnxNetIpRoutingLostMessage::Builder::create() const
{
    return { QKnxNetIp::ServiceType::RoutingLostMessage, QKnxByteArray { 0x04, quint8(m_state) }
        + QKnxUtils::QUint16::bytes(m_lostMessageCount) };
}

QT_END_NAMESPACE
