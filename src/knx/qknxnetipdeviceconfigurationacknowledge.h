/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPDEVICECONFIGURATIONACKNOWLEDGE_H
#define QKNXNETIPDEVICECONFIGURATIONACKNOWLEDGE_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheaderframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDeviceConfigurationAcknowledge final : public QKnxNetIpConnectionHeaderFrame
{
public:
    QKnxNetIpDeviceConfigurationAcknowledge() = default;
    ~QKnxNetIpDeviceConfigurationAcknowledge() override = default;

    QKnxNetIpDeviceConfigurationAcknowledge(quint8 communicationChannelId, quint8 sequenceCounter,
        QKnxNetIp::Error status);

    quint8 communicationChannelId() const;
    quint8 sequenceCounter() const;
    QKnxNetIp::Error status() const;
};

QT_END_NAMESPACE

#endif
