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

    QKnxNetIpDeviceConfigurationAcknowledge(quint8 channelId, quint8 sequenceCounter,
        QKnxNetIp::Error status);

    template <typename T>
        static QKnxNetIpDeviceConfigurationAcknowledge fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpConnectionHeaderFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge);
    }

    quint8 channelId() const;
    quint8 sequenceCounter() const;
    QKnxNetIp::Error status() const;

    bool isValid() const override;

private:
    QKnxNetIpDeviceConfigurationAcknowledge(const QKnxNetIpConnectionHeaderFrame &other);
};

QT_END_NAMESPACE

#endif
