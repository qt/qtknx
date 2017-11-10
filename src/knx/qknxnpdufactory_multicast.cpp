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

#include "qknxnpdufactory.h"

QT_BEGIN_NAMESPACE

// -- A_GroupValue

/*!
    Returns a NPDU for a Group value read application service.
*/
QKnxNpdu QKnxNpduFactory::Multicast::createGroupValueReadNpdu()
{
    return { QKnxNpdu::TransportControlField::DataGroup,
        QKnxNpdu::ApplicationControlField::GroupValueRead };
}

/*!
    Returns a NPDU for a Group value response application service.
*/
QKnxNpdu QKnxNpduFactory::Multicast::createGroupValueResponseNpdu(const QByteArray &data)
{
    if (data.size() > 14) // 3_02_02 Communication Medium TP1, Paragraph 2.2.4.1
        return {}; // 03_03_07 Application Layer, Paragraph 3.1.2: Data can be up to 14 octets.

    return { QKnxNpdu::TransportControlField::DataGroup,
        QKnxNpdu::ApplicationControlField::GroupValueResponse, data };
}

/*!
    Returns a NPDU for a Group value write application service.
*/
QKnxNpdu QKnxNpduFactory::Multicast::createGroupValueWriteNpdu(const QByteArray &data)
{
    if (data.size() > 14) // 3_02_02 Communication Medium TP1, Paragraph 2.2.4.1
        return {}; // 03_03_07 Application Layer, Paragraph 3.1.3: Data can be up to 14 octets.

    return { QKnxNpdu::TransportControlField::DataGroup,
        QKnxNpdu::ApplicationControlField::GroupValueWrite, data };
}


// -- A_GroupPropValue

static QKnxNpdu createGroupPropertyValueNpdu(QKnxNpdu::ApplicationControlField apci,
                                      QKnxInterfaceObjectType object, quint8 objectInstance,
                                      QKnxInterfaceObjectProperty property, const QByteArray &data)
{
    if (data.size() > 249) // L_Data_Extended -> max 254 bytes
        return {}; // 6 bytes already used for APCI, object, instance, property

    // 10_01 Logical Tag Extended, Paragraph 7.6.7
    // Only standardized properties may be used, range goes up to 154 (0x9a)
    if ((property > 0x9a) || (!QKnxInterfaceObjectType::isMatch(object, property)))
        return {};

    return { QKnxNpdu::TransportControlField::DataTagGroup, apci,
        QKnxUtils::QUint16::bytes(quint16(object)) + QKnxUtils::QUint8::bytes(quint8(property))
        + QKnxUtils::QUint8::bytes(objectInstance) + data };
}

QKnxNpdu QKnxNpduFactory::Multicast::createGroupPropertyValueReadNpdu(QKnxInterfaceObjectType obj,
    quint8 objectInstance, QKnxInterfaceObjectProperty property)
{
    return createGroupPropertyValueNpdu(QKnxNpdu::ApplicationControlField::GroupPropValueRead,
        obj, objectInstance, property, {});
}

QKnxNpdu
QKnxNpduFactory::Multicast::createGroupPropertyValueResponseNpdu(QKnxInterfaceObjectType object,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QByteArray &data)
{
    return createGroupPropertyValueNpdu(QKnxNpdu::ApplicationControlField::GroupPropValueResponse,
        object, objectInstance, property, data);
}

QKnxNpdu QKnxNpduFactory::Multicast::createGroupPropertyValueWriteNpdu(QKnxInterfaceObjectType obj,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QByteArray &data)
{
    return createGroupPropertyValueNpdu(QKnxNpdu::ApplicationControlField::GroupPropValueWrite,
        obj, objectInstance, property, data);
}

QKnxNpdu
QKnxNpduFactory::Multicast::createGroupPropertyValueInfoReportNpdu(QKnxInterfaceObjectType object,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QByteArray &data)
{
    return createGroupPropertyValueNpdu(QKnxNpdu::ApplicationControlField::GroupPropValueInfoReport,
        object, objectInstance, property, data);
}

QT_END_NAMESPACE
