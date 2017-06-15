/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPROUTINGINDICATION_H
#define QKNXNETIPROUTINGINDICATION_H

#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpRoutingIndication final : public QKnxNetIpFrame
{
public:
    QKnxNetIpRoutingIndication() = default;
    ~QKnxNetIpRoutingIndication() override = default;

    explicit QKnxNetIpRoutingIndication(const QKnxCemiFrame &cemi);

    template <typename T>
        static QKnxNetIpRoutingIndication fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::RoutingIndication);
    }

    QKnxCemiFrame cemi() const;
    void setCemi(const QKnxCemiFrame &cemi);

    bool isValid() const override;

private:
    QKnxNetIpRoutingIndication(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
