/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPTUNNELINGACKNOWLEDGE_H
#define QKNXNETIPTUNNELINGACKNOWLEDGE_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheaderframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpTunnelingAcknowledge final : public QKnxNetIpConnectionHeaderFrame
{
public:
    QKnxNetIpTunnelingAcknowledge() = default;
    ~QKnxNetIpTunnelingAcknowledge() override = default;

    QKnxNetIpTunnelingAcknowledge(quint8 communicationChannelId, quint8 sequenceCounter,
        QKnxNetIp::Error status);

    quint8 communicationChannelId() const;
    quint8 sequenceCounter() const;
    QKnxNetIp::Error status() const;

    bool isValid() const override;
};

QT_END_NAMESPACE

#endif
