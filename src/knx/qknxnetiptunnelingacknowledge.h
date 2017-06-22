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

    QKnxNetIpTunnelingAcknowledge(quint8 channelId, quint8 sequenceCounter,
        QKnxNetIp::Error status);

    template <typename T>
        static QKnxNetIpTunnelingAcknowledge fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpConnectionHeaderFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::TunnelingAcknowledge);
    }

    quint8 channelId() const;
    quint8 sequenceCounter() const;
    QKnxNetIp::Error status() const;

    bool isValid() const override;

private:
    QKnxNetIpTunnelingAcknowledge(const QKnxNetIpConnectionHeaderFrame &other);
};

QT_END_NAMESPACE

#endif
