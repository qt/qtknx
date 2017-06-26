/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

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

namespace QKnxPrivate
{
    static QKnxNetIpStructRef::Type codeToType(QKnxNetIp::DescriptionType code)
    {
        switch (QKnxNetIp::DescriptionType(code)) {
        case QKnxNetIp::DescriptionType::DeviceInfo:
            return QKnxNetIpStructRef::Type::QKnxNetIpDeviceDib;
        case QKnxNetIp::DescriptionType::SupportedServiceFamilies:
            return QKnxNetIpStructRef::Type::QKnxNetIpServiceFamiliesDib;
        case QKnxNetIp::DescriptionType::IpConfiguration:
            return QKnxNetIpStructRef::Type::QKnxNetIpConfigDib;
        case QKnxNetIp::DescriptionType::CurrentIpConfiguration:
            return QKnxNetIpStructRef::Type::QKnxNetIpCurrentConfigDib;
        case QKnxNetIp::DescriptionType::KnxAddresses:
            return QKnxNetIpStructRef::Type::QKnxNetIpKnxAddressesDib;
        case QKnxNetIp::DescriptionType::ManufactorData:
            return QKnxNetIpStructRef::Type::QKnxNetIpManufacturerDib;
        default:
            break;
        }
        return QKnxNetIpStructRef::Type::Null;
    }
}

class Q_KNX_EXPORT QKnxNetIpDescriptionResponse final : public QKnxNetIpFrame
{
public:
    QKnxNetIpDescriptionResponse() = default;
    ~QKnxNetIpDescriptionResponse() override = default;

    QKnxNetIpDescriptionResponse(const QKnxNetIpDeviceDib &deviceHardware,
                                 const QKnxNetIpServiceFamiliesDib &supportedFamilies);

    template <typename T>
        static QKnxNetIpDescriptionResponse fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::DescriptionResponse);
    }

    QKnxNetIpDeviceDib deviceHardware() const;
    QKnxNetIpServiceFamiliesDib supportedFamilies() const;

    template <typename T = std::vector<QKnxNetIpStructRef>>
        auto optionalDibs() const -> decltype(T())
    {
        static_assert(is_type<T, QVector<QKnxNetIpStructRef>, std::deque<QKnxNetIpStructRef>,
            std::vector<QKnxNetIpStructRef>>::value, "Type not supported.");

        const auto &ref = payloadRef();
        auto header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(ref, 0);
        quint16 index = header.totalSize(); // total size of device DIB

        header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(ref, index);
        index += header.totalSize(); // advance of total size of families DIB

        T dibs;
        while (index < ref.size()) {
            header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(ref, index);
            dibs.push_back(QKnxNetIpStructRef(payloadRef(index), QKnxPrivate::codeToType(header.code())));
            index += header.totalSize(); // advance of total size of last read DIB
        }
        return dibs;
    }

    template <typename T> void addOptionalDib(const T &dib)
    {
        static_assert(is_type<T, QKnxNetIpDeviceDib, QKnxNetIpConfigDib, QKnxNetIpCurrentConfigDib,
            QKnxNetIpKnxAddressesDib, QKnxNetIpManufacturerDib, QKnxNetIpServiceFamiliesDib>::value,
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
