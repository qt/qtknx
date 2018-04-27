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

#include "private/qknxtpdufactory_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTpduFactory::Multicast

    \inmodule QtKnx
    \brief The QKnxTpduFactory::Broadcast class is used to create a valid
    Transport protocol data unit (\l QKnxTpdu) for application services sent by
    multicast.

    From a client point of view, this is the service category to access devices
    functionality via group addressing.
*/

// -- A_GroupValue

/*!
    Returns a TPDU for a Group value read application service.
*/
QKnxTpdu QKnxTpduFactory::Multicast::createGroupValueReadTpdu()
{
    return { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueRead };
}

/*!
    Creates and returns a TPDU for a Group value response application service
    with the given \a data.
*/
QKnxTpdu QKnxTpduFactory::Multicast::createGroupValueResponseTpdu(const QKnxByteArray &data)
{
    if (data.size() > 253) // 3_02_02 Communication Medium TP1, Paragraph 2.2.4.1 -> 15 -1 but 2.2.5.1 -> 254 -1
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 03_03_07 Application Layer, Paragraph 3.1.2: Data can be up to 14 octets. (yes, but with Extended frame, should be more)

    return { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueResponse, data };
}

/*!
    Creates and returns a TPDU for a Group value write application service with
    the given \a data.
*/
QKnxTpdu QKnxTpduFactory::Multicast::createGroupValueWriteTpdu(const QKnxByteArray &data)
{
    if (data.size() > 253) // 3_02_02 Communication Medium TP1, Paragraph 2.2 -> 15 -1 but 2.2.5.1 -> 254 -1.4.1
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 03_03_07 Application Layer, Paragraph 3.1.3: Data can be up to 14 octets. (yes, but with Extended frame, should be more)

    return { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueWrite, data };
}


// -- A_GroupPropValue

static QKnxTpdu createGroupPropertyValueTpdu(QKnxTpdu::ApplicationControlField apci,
                                      QKnxInterfaceObjectType object, quint8 objectInstance,
                                      QKnxInterfaceObjectProperty property, const QKnxByteArray &data)
{
    if (data.size() > 249) // L_Data_Extended -> max 254 bytes
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 6 bytes already used for APCI, object, instance, property

    // 10_01 Logical Tag Extended, Paragraph 7.6.7
    // Only standardized properties may be used, range goes up to 154 (0x9a)
    if ((property > 0x9a) || (!QKnxInterfaceObjectType::isMatch(object, property)))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataTagGroup, apci,
        QKnxUtils::QUint16::bytes(quint16(object)) + QKnxUtils::QUint8::bytes(quint8(property))
        + QKnxUtils::QUint8::bytes(objectInstance) + data };
}

QKnxTpdu QKnxTpduFactory::Multicast::createGroupPropertyValueReadTpdu(QKnxInterfaceObjectType obj,
    quint8 objectInstance, QKnxInterfaceObjectProperty property)
{
    return createGroupPropertyValueTpdu(QKnxTpdu::ApplicationControlField::GroupPropValueRead,
        obj, objectInstance, property, {});
}

QKnxTpdu
QKnxTpduFactory::Multicast::createGroupPropertyValueResponseTpdu(QKnxInterfaceObjectType object,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QKnxByteArray &data)
{
    return createGroupPropertyValueTpdu(QKnxTpdu::ApplicationControlField::GroupPropValueResponse,
        object, objectInstance, property, data);
}

QKnxTpdu QKnxTpduFactory::Multicast::createGroupPropertyValueWriteTpdu(QKnxInterfaceObjectType obj,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QKnxByteArray &data)
{
    return createGroupPropertyValueTpdu(QKnxTpdu::ApplicationControlField::GroupPropValueWrite,
        obj, objectInstance, property, data);
}

QKnxTpdu
QKnxTpduFactory::Multicast::createGroupPropertyValueInfoReportTpdu(QKnxInterfaceObjectType object,
    quint8 objectInstance, QKnxInterfaceObjectProperty property, const QKnxByteArray &data)
{
    return createGroupPropertyValueTpdu(QKnxTpdu::ApplicationControlField::GroupPropValueInfoReport,
        object, objectInstance, property, data);
}

QT_END_NAMESPACE
