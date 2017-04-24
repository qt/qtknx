/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCONNECTREQUEST_H
#define QKNXNETIPCONNECTREQUEST_H

#include <QtKnx/qknxnetipcri.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectRequest final : public QKnxNetIpFrame
{
public:
    QKnxNetIpConnectRequest() = default;
    ~QKnxNetIpConnectRequest() override = default;

    QKnxNetIpConnectRequest(const QKnxNetIpHPAI &controlEndpoint,
                            const QKnxNetIpHPAI &dataEndpoint,
                            const QKnxNetIpCRI &requestInformation);

    QKnxNetIpHPAI controlEndpoint() const;
    QKnxNetIpHPAI dataEndpoint() const;
    QKnxNetIpCRI requestInformation() const;

    bool isValid() const override;
};

QT_END_NAMESPACE

#endif
