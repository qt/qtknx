/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXNETIPDESCRIPTIONRESPONSE_H
#define QKNXNETIPDESCRIPTIONRESPONSE_H

#include <QtCore/qvector.h>

#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtKnx/qknxnetipknxaddressesdib.h>
#include <QtKnx/qknxnetipmanufacturerdib.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxnetipstructref.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDescriptionResponse final
{
public:
    QKnxNetIpDescriptionResponse() = delete;
    ~QKnxNetIpDescriptionResponse() = default;

    QKnxNetIpDescriptionResponse(const QKnxNetIpFrame &&) = delete;
    explicit QKnxNetIpDescriptionResponse(const QKnxNetIpFrame &frame);

    bool isValid() const;

    QKnxNetIpDeviceDib deviceHardware() const;
    QKnxNetIpServiceFamiliesDib supportedFamilies() const;

    QVector<QKnxNetIpStructRef> optionalDibs() const
    {
        const auto codeToType = [] (QKnxNetIp::DescriptionType code) -> QKnxNetIpStructRef::Type
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
            case QKnxNetIp::DescriptionType::ManufacturerData:
                return QKnxNetIpStructRef::Type::QKnxNetIpManufacturerDib;
            default:
                break;
            }
            return QKnxNetIpStructRef::Type::Null;
        };

        const auto &data = m_frame.constData();
        auto header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, 0);
        quint16 index = header.totalSize(); // total size of device DIB

        header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, index);
        index += header.totalSize(); // advance of total size of families DIB

        QVector<QKnxNetIpStructRef> dibs;
        while (index < data.size()) {
            header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, index);
            dibs.push_back(QKnxNetIpStructRef(data.mid(index, header.totalSize()),
                codeToType(header.code())));
            index += header.totalSize(); // advance of total size of last read DIB
        }
        return dibs;
    }

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder &setDeviceHardware(const QKnxNetIpDeviceDib &ddib);
        Builder &setSupportedFamilies(const QKnxNetIpServiceFamiliesDib &sdib);

        template <typename T> void addOptionalDib(const T &dib)
        {
            static_assert(is_type<T, QKnxNetIpDeviceDib, QKnxNetIpConfigDib,
                QKnxNetIpCurrentConfigDib, QKnxNetIpKnxAddressesDib, QKnxNetIpManufacturerDib,
                QKnxNetIpServiceFamiliesDib>::value,
                "QKnxNetIpDescriptionResponse::Builder::addOptionalDib - Type not supported.");
            m_optionalDibs += dib.bytes();
        }

        QKnxNetIpFrame create() const;

    private:
        QKnxNetIpDeviceDib m_ddib;
        QKnxNetIpServiceFamiliesDib m_sdib;
        QKnxByteArray m_optionalDibs;
    };
    static QKnxNetIpDescriptionResponse::Builder builder();

private:
    const QKnxNetIpFrame &m_frame;
};

QT_END_NAMESPACE

#endif
