/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtKnx/qknxnetipknxaddressesdib.h>
#include <QtKnx/qknxnetipmanufacturerdib.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDescriptionResponse final : private QKnxNetIpFrame
{
public:
    QKnxNetIpDescriptionResponse() = default;

    QKnxNetIpDescriptionResponse(const QKnxNetIpDeviceDIB &deviceHardware,
                                 const QKnxNetIpServiceFamiliesDIB &supportedFamilies);

    QKnxNetIpDeviceDIB deviceHardware() const;
    QKnxNetIpServiceFamiliesDIB supportedFamilies() const;

    // TODO: add access functions for optional DIBs
    template <typename T> void addOptionalDib(const T &dib)
    {
        static_assert(is_type<T, QKnxNetIpDeviceDIB, QKnxNetIpConfigDIB, QKnxNetIpCurrentConfigDIB,
            QKnxNetIpKnxAddressesDIB, QKnxNetIpManufacturerDIB, QKnxNetIpServiceFamiliesDIB>::value,
            "Type not supported.");

        auto load = payload();
        load.appendBytes(dib.bytes());
        setPayload(load);
    }

    bool isValid() const override;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
