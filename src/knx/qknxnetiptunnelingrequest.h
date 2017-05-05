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

    template <typename T> static QKnxNetIpTunnelingRequest fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpConnectionHeaderFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::TunnelingRequest);
    }

    quint8 communicationChannelId() const;
    quint8 sequenceCounter() const;
    QKnxCemi cemi() const;

    bool isValid() const override;

private:
    QKnxNetIpTunnelingRequest(const QKnxNetIpConnectionHeaderFrame &other);
};

QT_END_NAMESPACE

#endif
