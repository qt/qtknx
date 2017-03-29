/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipcri.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectRequest final : private QKnxNetIpFrame
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

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
