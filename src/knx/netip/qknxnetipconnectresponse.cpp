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

QKnxNetIpConnectResponse::QKnxNetIpConnectResponse(const QKnxNetIpFrameEx &frame)
    : m_frame(frame)
{}

bool QKnxNetIpConnectResponse::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::ConnectResponse
        && (status() == QKnxNetIp::Error::None ? m_frame.size() >= 18 : true);
}

quint8 QKnxNetIpConnectResponse::channelId() const
{
    return m_frame.constData().value(0);
}

QKnxNetIp::Error QKnxNetIpConnectResponse::status() const
{
    return QKnxNetIp::Error(m_frame.constData().value(1));
}

QKnxNetIpHpai QKnxNetIpConnectResponse::dataEndpoint() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 2);
}

QKnxNetIpCrd QKnxNetIpConnectResponse::responseData() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpCrd::fromBytes(m_frame.constData(), 10);
}

QKnxNetIpConnectResponse::Builder QKnxNetIpConnectResponse::builder()
{
    return QKnxNetIpConnectResponse::Builder();
}


// -- QKnxNetIpConnectResponse::Builder

QKnxNetIpConnectResponse::Builder &
    QKnxNetIpConnectResponse::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

QKnxNetIpConnectResponse::Builder &
    QKnxNetIpConnectResponse::Builder::setStatus(QKnxNetIp::Error status)
{
    m_status = status;
    return *this;
}

QKnxNetIpConnectResponse::Builder &
    QKnxNetIpConnectResponse::Builder::setDataEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

QKnxNetIpConnectResponse::Builder &
    QKnxNetIpConnectResponse::Builder::setResponseData(const QKnxNetIpCrd &crd)
{
    m_crd = crd;
    return *this;
}

QKnxNetIpFrameEx QKnxNetIpConnectResponse::Builder::create() const
{
    QKnxByteArray data { m_channelId, quint8(m_status) };
    if (m_status == QKnxNetIp::Error::None)
        data += m_hpai.bytes() + m_crd.bytes();
    return { QKnxNetIp::ServiceType::ConnectResponse, data };
}

QT_END_NAMESPACE
