/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPTUNNELINGREQUEST_H
#define QKNXNETIPTUNNELINGREQUEST_H

#include <QtKnx/qknxcemi.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheaderframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpTunnelingRequest final : public QKnxNetIpConnectionHeaderFrame
{
public:
    QKnxNetIpTunnelingRequest() = default;
    ~QKnxNetIpTunnelingRequest() override = default;

    QKnxNetIpTunnelingRequest(quint8 communicationChannelId, quint8 sequenceCounter,
        const QKnxCemi &cemi);

    quint8 communicationChannelId() const;
    quint8 sequenceCounter() const;
    QKnxCemi cemi() const;

    bool isValid() const override;
};

QT_END_NAMESPACE

#endif
