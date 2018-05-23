/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#ifndef QKNXNAMESPACE_H
#define QKNXNAMESPACE_H

#include <QtCore/qmetaobject.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

namespace QKnx
{
    Q_KNX_EXPORT Q_NAMESPACE

    enum class MediumType : quint8
    {
        Unknown = 0x00,
        TP = 0x01,
        PL = 0x04,
        RF = 0x10,
        NetIP = 0x20
    };
    Q_ENUM_NS(MediumType)

    enum class InterfaceFeature : quint8
    {
        Unknown = 0x00,
        SupportedEmiType = 0x01,
        HostDeviceDescriptorType0 = 0x02,
        BusConnectionStatus = 0x03,
        KnxManufacturerCode = 0x04,
        ActiveEmiType = 0x05,
        IndividualAddress = 0x06,
        MaximumApduLength = 0x07,
        InterfaceFeatureInfoServiceEnable = 0x08
    };
    Q_ENUM_NS(InterfaceFeature)
    Q_KNX_EXPORT bool isInterfaceFeature(QKnx::InterfaceFeature feature);

    enum class ReturnCode : quint8
    {
        Success = 0x00,
        SuccessWithCrc = 0x01,
        MemoryError = 0xf1,
        CommandInvalid = 0xf2,
        CommandImpossible = 0xf3,
        LengthExceedsMaxApduLength = 0xf4,
        DataOverflow = 0xf5,
        DataMin = 0xf6,
        DataMax = 0xf7,
        DataVoid = 0xf8,
        TemporarilyNotAvailable = 0xf9,
        AccessWriteOnly = 0xfa,
        AccessReadOnly = 0xfb,
        AccessDenied = 0xfc,
        AddressVoid = 0xfd,
        DataTypeConflict = 0xfe,
        Error = 0xff
    };
    Q_ENUM_NS(ReturnCode)
}
Q_DECLARE_TYPEINFO(QKnx::MediumType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnx::InterfaceFeature, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnx::ReturnCode, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
