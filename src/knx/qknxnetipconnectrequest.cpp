/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipconnectrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectRequest::QKnxNetIpConnectRequest(const QKnxNetIpHPAI &controlEndpoint,
        const QKnxNetIpHPAI &dataEndpoint, const QKnxNetIpCRI &requestInformation)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::ConnectRequest))
{
    QKnxNetIpPayload payload;
    payload.setBytes(controlEndpoint.bytes());
    payload.appendBytes(dataEndpoint.bytes());
    payload.appendBytes(requestInformation.bytes());
    setPayload(payload);
}

QKnxNetIpHPAI QKnxNetIpConnectRequest::controlEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payload().bytes(0, 8), 0);
}

QKnxNetIpHPAI QKnxNetIpConnectRequest::dataEndpoint() const
{
    return QKnxNetIpHPAI::fromBytes(payload().bytes(8, 8), 0);
}

QKnxNetIpCRI QKnxNetIpConnectRequest::requestInformation() const
{
    return QKnxNetIpCRI::fromBytes(payload().bytes(), 16);
}

bool QKnxNetIpConnectRequest::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 28;
}

QT_END_NAMESPACE
