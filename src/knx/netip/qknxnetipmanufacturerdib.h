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

#ifndef QKNXNETIPMANUFACTURERDIB_H
#define QKNXNETIPMANUFACTURERDIB_H

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpManufacturerDib final : public QKnxNetIpDescriptionTypeStruct
{
public:
    QKnxNetIpManufacturerDib() = default;
    ~QKnxNetIpManufacturerDib() override = default;

    explicit QKnxNetIpManufacturerDib(quint16 manufacturerId);
    QKnxNetIpManufacturerDib(quint16 manufacturerId, const QKnxByteArray &manufacturerData);

    static QKnxNetIpManufacturerDib fromBytes(const QKnxByteArray &bytes, quint16 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::ManufactorData);
    }

    QKnxNetIp::DescriptionType descriptionType() const;

    quint16 manufacturerId() const;
    QKnxByteArray manufacturerData() const
    {
        const auto &ref = payloadRef();
        return ref.bytes(2, ref.size() - 2);
    }

    bool isValid() const override;

private:
    QKnxNetIpManufacturerDib(const QKnxNetIpDescriptionTypeStruct &other);
};

QT_END_NAMESPACE

#endif
