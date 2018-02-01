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

#include "qknxlinklayerframefactory.h"
#include "qknxtpdufactory.h"

// TODO: introduce QKnx::MediumType dependency. The LinkLayer Frame look different
// depending on the chosen medium. For the moment only netIp Tunneling is taken care of.
// But GroupValue service can be send via L_Data (netIp) or L_Data_Standard / L_Data_Extended
// if medium is TP of PL. Maybe transform this factory into a builder, somehow "linked" to
// the LinkLayerFrame, so that the MediumType is known.

QT_BEGIN_NAMESPACE

/*!
    \class QKnxLinkLayerFrameFactory

    \inmodule QtKnx
    \brief The QKnxLinkLayerFrameFactory class allows you to create a factory that
    contains a specialized set of builders to produce \l QKnxLinkLayerFrame instances
    for one particular KNX application service type.
*/

// Reminder: in case of data.size() > 14 make sure the priority is set to Low

static QKnxControlField setupControlField(
            QKnxControlField::FrameType type = QKnxControlField::FrameType::Standard,
            QKnxControlField::Repeat repeat = QKnxControlField::Repeat::DoNotRepeat,
            QKnxControlField::Broadcast broad = QKnxControlField::Broadcast::System,
            QKnxControlField::Priority priority = QKnxControlField::Priority::Low,
            QKnxControlField::Acknowledge acknowledge = QKnxControlField::Acknowledge::NotRequested,
            QKnxControlField::Confirm errorStatus = QKnxControlField::Confirm::NoError)
{
    QKnxControlField ctrl;
    ctrl.setFrameType(type);
    ctrl.setRepeat(repeat);
    ctrl.setBroadcast(broad);
    ctrl.setPriority(priority);
    ctrl.setAcknowledge(acknowledge);
    ctrl.setConfirm(errorStatus);
    return ctrl;
}

static QKnxLinkLayerFrame createFrame(QKnxLinkLayerFrame::MessageCode code, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, const QKnxAddress &src, const QKnxAddress &dest,
    const QKnxTpdu &tpdu)
{
    if (dest.type() != extCtrl.destinationAddressType())
        return {};

    QKnxLinkLayerFrame frame;
    frame.setMessageCode(code);
    frame.setDestinationAddress(dest);
    frame.setSourceAddress(src);
    frame.setControlField(ctrl);
    frame.setExtendedControlField(extCtrl);
    frame.setTpdu(tpdu);
    return frame;
}


// -- A_GroupValueRead

static bool groupValueArgumentsValid(const QKnxAddress &src, const QKnxAddress &dest,
    const QKnxExtendedControlField &extCtrl)
{
    return src.isValid() && src.type() == QKnxAddress::Type::Individual
        && dest.isValid() && dest.type() == QKnxAddress::Type::Group
        && extCtrl.hopCount() < 7 && extCtrl.destinationAddressType() == QKnxAddress::Type::Group;
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createReadRequest(const QKnxAddress &src,
    const QKnxAddress &dest)
{
    return createReadRequest(src, dest, createRequestControlField(), createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createReadRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest,
        QKnxTpduFactory::Multicast::createGroupValueReadTpdu());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createReadConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, QKnxControlField::Confirm status)
{
    return createReadConfirmation(src, dest, createConfirmationControlField(status),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createReadConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        QKnxTpduFactory::Multicast::createGroupValueReadTpdu());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createReadIndication(const QKnxAddress &src,
    const QKnxAddress &dest)
{
    return createReadIndication(src, dest, createIndicationControlField(),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createReadIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest,
        QKnxTpduFactory::Multicast::createGroupValueReadTpdu());
}


// -- A_GroupValueResponse

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data)
{
    return createResponseRequest(src, dest, data, createRequestControlField(),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueResponseTpdu(data);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, QKnxControlField::Confirm status)
{
    return createResponseConfirmation(src, dest, data, createConfirmationControlField(status),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueResponseTpdu(data);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data)
{
    return createResponseIndication(src, dest, data, createIndicationControlField(),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueResponseTpdu(data);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest,
        tpdu);
}


// -- A_GroupValueWrite

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data)
{
    return createWriteRequest(src, dest, data, createRequestControlField(),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu(data);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, QKnxControlField::Confirm status)
{
    return createWriteConfirmation(src, dest, data, createConfirmationControlField(status),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu(data);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data)
{
    return createWriteIndication(src, dest, data, createIndicationControlField(),
        createExtentedControlField());
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QVector<quint8> &data, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl)
{
    if (!groupValueArgumentsValid(src, dest, extCtrl))
        return {};

    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu(data);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest,
        tpdu);
}


// -- A_GroupValue Tools

QKnxControlField
QKnxLinkLayerFrameFactory::GroupValue::createRequestControlField(QKnxControlField::Acknowledge ack,
    QKnxControlField::Priority priority)
{
    auto controlField = setupControlField();
    controlField.setAcknowledge(ack);
    controlField.setPriority(priority);
    controlField.setBroadcast(QKnxControlField::Broadcast::Domain);
    return controlField;
}

QKnxControlField
QKnxLinkLayerFrameFactory::GroupValue::createConfirmationControlField(QKnxControlField::Confirm status,
    QKnxControlField::Acknowledge acknowledge, QKnxControlField::Priority priority)
{
    auto controlField = setupControlField();
    controlField.setConfirm(status);
    controlField.setAcknowledge(acknowledge);
    controlField.setPriority(priority);
    controlField.setBroadcast(QKnxControlField::Broadcast::Domain);
    return controlField;
}

QKnxControlField
QKnxLinkLayerFrameFactory::GroupValue::createIndicationControlField(QKnxControlField::Priority priority)
{
    auto controlField = setupControlField();
    controlField.setPriority(priority);
    controlField.setBroadcast(QKnxControlField::Broadcast::Domain);
    return controlField;
}

QKnxExtendedControlField
QKnxLinkLayerFrameFactory::GroupValue::createExtentedControlField(quint8 hopCount,
    QKnxExtendedControlField::ExtendedFrameFormat format)
{
    QKnxExtendedControlField ctrl;
    ctrl.setDestinationAddressType(QKnxAddress::Type::Group);
    ctrl.setHopCount(hopCount);
    ctrl.setFormat(format);
    return ctrl;
}

QT_END_NAMESPACE
