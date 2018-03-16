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

#include "qknxnetipconnectionstateresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectionStateResponse::QKnxNetIpConnectionStateResponse(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

quint8 QKnxNetIpConnectionStateResponse::channelId() const
{
    return m_frame.constData().value(0);
}

QKnxNetIp::Error QKnxNetIpConnectionStateResponse::status() const
{
    return QKnxNetIp::Error(m_frame.constData().value(1));
}

bool QKnxNetIpConnectionStateResponse::isValid() const
{
    return m_frame.isValid() && m_frame.size() == 8
        && m_frame.serviceType() == QKnxNetIp::ServiceType::ConnectionStateResponse;
}

QKnxNetIpConnectionStateResponse::Builder QKnxNetIpConnectionStateResponse::builder()
{
    return QKnxNetIpConnectionStateResponse::Builder();
}


// -- QKnxNetIpConnectionStateResponse::Builder

QKnxNetIpConnectionStateResponse::Builder &
    QKnxNetIpConnectionStateResponse::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

QKnxNetIpConnectionStateResponse::Builder &
    QKnxNetIpConnectionStateResponse::Builder::setStatus(QKnxNetIp::Error status)
{
    m_status = status;
    return *this;
}

QKnxNetIpFrame QKnxNetIpConnectionStateResponse::Builder::create() const
{
    QKnxByteArray data;
    switch (m_status) {
    case QKnxNetIp::Error::None:
    case QKnxNetIp::Error::ConnectionId:
    case QKnxNetIp::Error::DataConnection:
    case QKnxNetIp::Error::KnxConnection:
        data = { m_channelId, quint8(m_status) };
        break;
    default:
        break;
    };
    return { QKnxNetIp::ServiceType::ConnectionStateResponse, data };
}

QT_END_NAMESPACE
