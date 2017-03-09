/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpSearchRequest final : private QKnxNetIpFrame
{
public:
    QKnxNetIpSearchRequest() = default;
    QKnxNetIpSearchRequest(const QHostAddress &address, quint16 port);
    explicit QKnxNetIpSearchRequest(const QKnxNetIpHPAI &discoveryEndpoint);

    bool isValid() const override;
    QKnxNetIpHPAI discoveryEndpoint() const;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
