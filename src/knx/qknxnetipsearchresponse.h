/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpSearchResponse final : private QKnxNetIpFrame
{
public:
    QKnxNetIpSearchResponse() = default;
    ~QKnxNetIpSearchResponse() override = default;

    QKnxNetIpSearchResponse(const QKnxNetIpHPAI &controlEndpoint,
                            const QKnxNetIpDeviceDIB &deviceHardware,
                            const QKnxNetIpServiceFamiliesDIB &supportedFamilies);

    QKnxNetIpHPAI controlEndpoint() const;
    QKnxNetIpDeviceDIB deviceHardware() const;
    QKnxNetIpServiceFamiliesDIB supportedFamilies() const;

    bool isValid() const override;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
