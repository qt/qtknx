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
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::TunnelingAcknowledge)
{
    QKnxNetIpConnectionHeader header;
    header.setCommunicationChannelId(id);
    header.setSequenceCounter(sequenceCounter);
    header.setServiceTypeSpecificValue(quint8(status));
    setConnectionHeader(header);
}

QKnxNetIpTunnelingAcknowledge::QKnxNetIpTunnelingAcknowledge(
        const QKnxNetIpConnectionHeaderFrame &other)
    : QKnxNetIpConnectionHeaderFrame(other)
{}

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

bool QKnxNetIpTunnelingAcknowledge::isValid() const
{
    return QKnxNetIpConnectionHeaderFrame::isValid() && size() == 10
        && code() == QKnxNetIp::ServiceType::TunnelingAcknowledge;
}

QT_END_NAMESPACE
