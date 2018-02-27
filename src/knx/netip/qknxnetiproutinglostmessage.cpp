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

QKnxNetIpRoutingLostMessage::QKnxNetIpRoutingLostMessage(QKnxNetIp::DeviceState state,
        quint16 lostMessageCount)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::RoutingLostMessage)
{
    QKnxNetIpPayload payload((quint8) state);
    payload.appendBytes(QKnxUtils::QUint16::bytes(lostMessageCount));
    setPayload(payload);
}

QKnxNetIpRoutingLostMessage::QKnxNetIpRoutingLostMessage(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

QKnxNetIp::DeviceState QKnxNetIpRoutingLostMessage::deviceState() const
{
    return QKnxNetIp::DeviceState(payloadRef().byte(0));
}

quint16 QKnxNetIpRoutingLostMessage::lostMessageCount() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef().bytes(0), 1);
}

bool QKnxNetIpRoutingLostMessage::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 10
        && code() == QKnxNetIp::ServiceType::RoutingLostMessage;
}

QT_END_NAMESPACE
