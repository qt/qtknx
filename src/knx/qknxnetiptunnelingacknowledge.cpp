/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiptunnelingacknowledge.h"

QT_BEGIN_NAMESPACE

QKnxNetIpTunnelingAcknowledge::QKnxNetIpTunnelingAcknowledge(quint8 id,
        quint8 sequenceCounter, QKnxNetIp::Error status)
    : QKnxNetIpConnectionHeaderFrame(quint16(QKnxNetIp::ServiceType::TunnelingAcknowledge))
{
    QKnxNetIpConnectionHeader header;
    header.setCommunicationChannelId(id);
    header.setSequenceCounter(sequenceCounter);
    header.setServiceTypeSpecificValue(quint8(status));
    setConnectionHeader(header);
}

quint8 QKnxNetIpTunnelingAcknowledge::communicationChannelId() const
{
    return connectionHeader().communicationChannelId();
}

quint8 QKnxNetIpTunnelingAcknowledge::sequenceCounter() const
{
    return connectionHeader().sequenceCounter();
}

QKnxNetIp::Error QKnxNetIpTunnelingAcknowledge::status() const
{
    return QKnxNetIp::Error(connectionHeader().serviceTypeSpecificValue());
}

QT_END_NAMESPACE
