/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipconnectresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConnectResponse::QKnxNetIpConnectResponse(QKnxNetIp::Error status)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::ConnectResponse))
{
    QKnxNetIpPayload payload;
    payload.setByte(1, quint8(status));
    setPayload(payload);
}

QKnxNetIpConnectResponse::QKnxNetIpConnectResponse(quint8 channelId, QKnxNetIp::Error status,
        const QKnxNetIpHPAI &dataEndpoint, const QKnxNetIpCRD &responseData)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::ConnectResponse))
{
    QKnxNetIpPayload payload(channelId);
    payload.setByte(1, quint8(status));
    payload.appendBytes(dataEndpoint.bytes());
    payload.appendBytes(responseData.bytes());
    setPayload(payload);
}

quint8 QKnxNetIpConnectResponse::channelId() const
{
    return payloadRef().byte(0);
}

QKnxNetIp::Error QKnxNetIpConnectResponse::status() const
{
    return QKnxNetIp::Error(payloadRef().byte(1));
}

QKnxNetIpHPAI QKnxNetIpConnectResponse::dataEndpoint() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpHPAI::fromBytes(payloadRef(), 2);
}

QKnxNetIpCRD QKnxNetIpConnectResponse::responseData() const
{
    if (status() != QKnxNetIp::Error::None)
        return {};
    return QKnxNetIpCRD::fromBytes(payloadRef(), 10);
}

bool QKnxNetIpConnectResponse::isValid() const
{
    if (status() == QKnxNetIp::Error::None)
        return QKnxNetIpFrame::isValid() && size() >= 22;
    return QKnxNetIpFrame::isValid();
}

QT_END_NAMESPACE
