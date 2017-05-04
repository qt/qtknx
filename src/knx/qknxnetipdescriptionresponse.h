/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPDESCRIPTIONRESPONSE_H
#define QKNXNETIPDESCRIPTIONRESPONSE_H

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
#include <QtKnx/qknxnetipstructref.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDescriptionResponse final : public QKnxNetIpFrame
{
public:
    QKnxNetIpDescriptionResponse() = default;
    ~QKnxNetIpDescriptionResponse() override = default;

    QKnxNetIpDescriptionResponse(const QKnxNetIpDeviceDIB &deviceHardware,
                                 const QKnxNetIpServiceFamiliesDIB &supportedFamilies);

    template <typename T>
        static QKnxNetIpDescriptionResponse fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::DescriptionResponse);
    }

    QKnxNetIpDeviceDIB deviceHardware() const;
    QKnxNetIpServiceFamiliesDIB supportedFamilies() const;

    QVector<QKnxNetIpStructRef> optionalDibs() const;
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

private:
    QKnxNetIpDescriptionResponse(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
