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

#include "qknxnetipconnectresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectResponse::QKnxNetIpConnectResponse(QKnxNetIp::Error status)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::ConnectResponse)
{
    QKnxNetIpPayload payload;
    payload.setByte(1, quint8(status));
    setPayload(payload);
}

QKnxNetIpConnectResponse::QKnxNetIpConnectResponse(quint8 channelId, QKnxNetIp::Error status,
        const QKnxNetIpHpai &dataEndpoint, const QKnxNetIpCrd &responseData)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::ConnectResponse)
{
    QKnxNetIpPayload payload(channelId);
    payload.setByte(1, quint8(status));
    payload.appendBytes(dataEndpoint.bytes());
    payload.appendBytes(responseData.bytes());
    setPayload(payload);
}

QKnxNetIpConnectResponse::QKnxNetIpConnectResponse(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

quint8 QKnxNetIpConnectResponse::channelId() const
{
    return payloadRef().byte(0);
}

QKnxNetIp::Error QKnxNetIpConnectResponse::status() const
{
    return QKnxNetIp::Error(payloadRef().byte(1));
}

QKnxNetIpHpai QKnxNetIpConnectResponse::dataEndpoint() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpHpai::fromBytes(payloadRef().bytes(0), 2);
}

QKnxNetIpCrd QKnxNetIpConnectResponse::responseData() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpCrd::fromBytes(payloadRef().bytes(0), 10);
}

bool QKnxNetIpConnectResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && code() == QKnxNetIp::ServiceType::ConnectResponse
        && (status() == QKnxNetIp::Error::None ? size() >= 18 : true);
}

QT_END_NAMESPACE
