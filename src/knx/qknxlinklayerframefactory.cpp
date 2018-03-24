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
            QKnxControlField::FrameFormat type = QKnxControlField::FrameFormat::Standard,
            QKnxControlField::Repeat repeat = QKnxControlField::Repeat::DoNotRepeat,
            QKnxControlField::Broadcast broad = QKnxControlField::Broadcast::System,
            QKnxControlField::Priority priority = QKnxControlField::Priority::Low,
            QKnxControlField::Acknowledge acknowledge = QKnxControlField::Acknowledge::NotRequested,
            QKnxControlField::Confirm errorStatus = QKnxControlField::Confirm::NoError)
{
    QKnxControlField ctrl;
    ctrl.setFrameFormat(type);
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
    QKnxControlField temp = ctrl;
    if (tpdu.dataSize() > 15) {
        temp.setFrameFormat(QKnxControlField::FrameFormat::Extended);
        temp.setPriority(QKnxControlField::Priority::Low);
    }
    frame.setControlField(temp);
    frame.setExtendedControlField(extCtrl);
    frame.setTpdu(tpdu);
    return frame;
}


// -- Tools

QKnxControlField
QKnxLinkLayerFrameFactory::createRequestControlField(QKnxControlField::Acknowledge ack,
    QKnxControlField::Priority priority, QKnxControlField::Broadcast broadcast)
{
    auto controlField = setupControlField();
    controlField.setAcknowledge(ack);
    controlField.setPriority(priority);
    controlField.setBroadcast(broadcast); // TODO: Is Domain correct for Memory Services?
    return controlField;
}

QKnxControlField
QKnxLinkLayerFrameFactory::createConfirmationControlField(QKnxControlField::Confirm status,
    QKnxControlField::Acknowledge acknowledge, QKnxControlField::Priority priority,
    QKnxControlField::Broadcast broadcast)
{
    auto controlField = setupControlField();
    controlField.setConfirm(status);
    controlField.setAcknowledge(acknowledge);
    controlField.setPriority(priority);
    controlField.setBroadcast(broadcast); // TODO: Is Domain correct for Memory Services?
    return controlField;
}

QKnxControlField
QKnxLinkLayerFrameFactory::createIndicationControlField(QKnxControlField::Priority priority,
    QKnxControlField::Broadcast broadcast)
{
    auto controlField = setupControlField();
    controlField.setPriority(priority);
    controlField.setBroadcast(broadcast); // TODO: Is Domain correct for Memory Services?
    return controlField;
}

QKnxExtendedControlField
QKnxLinkLayerFrameFactory::createExtentedControlField(QKnxAddress::Type type, quint8 hopCount,
    QKnxExtendedControlField::ExtendedFrameFormat format)
{
    QKnxExtendedControlField ctrl;
    ctrl.setDestinationAddressType(type);
    ctrl.setHopCount(hopCount);
    ctrl.setFormat(format);
    return ctrl;
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
    return createReadRequest(src, dest, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Group));
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
        createExtentedControlField(QKnxAddress::Type::Group));
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
        createExtentedControlField(QKnxAddress::Type::Group));
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
    const QKnxAddress &dest, const QKnxByteArray &data)
{
    return createResponseRequest(src, dest, data, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Group));
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxByteArray &data, const QKnxControlField &ctrl,
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
    const QKnxAddress &dest, const QKnxByteArray &data, QKnxControlField::Confirm status)
{
    return createResponseConfirmation(src, dest, data, createConfirmationControlField(status),
        createExtentedControlField(QKnxAddress::Type::Group));
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxByteArray &data, const QKnxControlField &ctrl,
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
    const QKnxAddress &dest, const QKnxByteArray &data)
{
    return createResponseIndication(src, dest, data, createIndicationControlField(),
        createExtentedControlField(QKnxAddress::Type::Group));
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createResponseIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxByteArray &data, const QKnxControlField &ctrl,
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
    const QKnxAddress &dest, const QKnxByteArray &data)
{
    return createWriteRequest(src, dest, data, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Group));
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteRequest(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxByteArray &data, const QKnxControlField &ctrl,
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
    const QKnxAddress &dest, const QKnxByteArray &data, QKnxControlField::Confirm status)
{
    return createWriteConfirmation(src, dest, data, createConfirmationControlField(status),
        createExtentedControlField(QKnxAddress::Type::Group));
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxByteArray &data, const QKnxControlField &ctrl,
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
    const QKnxAddress &dest, const QKnxByteArray &data)
{
    return createWriteIndication(src, dest, data, createIndicationControlField(),
        createExtentedControlField(QKnxAddress::Type::Group));
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::GroupValue::createWriteIndication(const QKnxAddress &src,
    const QKnxAddress &dest, const QKnxByteArray &data, const QKnxControlField &ctrl,
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


// -- A_Memory Tools

// Point to point connection oriented. A_MemoryRead/Write/Response
// This is Tunnel frame factory, so the message code has to be:
// L_Data.req or L_Data.con or L_Data.ind (never mind the L_Raw, LReaset and L_Busmon here)
// In doc 3/3/7 Application Layers 3.5.3, they talk about T_Data_Connected instead of L_Data,
// but this is because they are talking about exchange of a message at a deeper level

static bool memoryArgumentsValid(const QKnxAddress &src, const QKnxAddress &dest,
    const QKnxExtendedControlField &extCtrl, quint8 number, quint8 sequenceNumber)
{
    return src.isValid() && src.type() == QKnxAddress::Type::Individual
        && dest.isValid() && dest.type() == QKnxAddress::Type::Individual
        && extCtrl.hopCount() < 7
        && extCtrl.destinationAddressType() == QKnxAddress::Type::Individual
        && number < 64 && sequenceNumber <= 15;
}

static bool memoryArgumentsValid(const QKnxAddress &src, const QKnxAddress &dest,
    const QKnxExtendedControlField &extCtrl, quint8 number, quint8 sequenceNumber,
    const QKnxByteArray &data)
{
    return memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber)
        && number == data.size();
}


// -- A_Memory_Read

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createReadRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, quint8 sequenceNumber)
{
    return createReadRequest(src, dest, memoryAddress, number, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createReadRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryReadTpdu(number,
        memoryAddress, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createReadConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, quint8 sequenceNumber,
    QKnxControlField::Confirm status)
{
    return createReadConfirmation(src, dest, memoryAddress, number,
        createConfirmationControlField(status),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createReadConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryReadTpdu(number,
        memoryAddress, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createReadIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, quint8 sequenceNumber)
{
    return createReadIndication(src, dest, memoryAddress, number, createIndicationControlField(),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createReadIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryReadTpdu(number,
        memoryAddress, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest,
        tpdu);
}


// -- A_Memory_Response

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createResponseRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    quint8 sequenceNumber)
{
    return createResponseRequest(src, dest, memoryAddress, number, data, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createResponseRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber, data))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryResponseTpdu(number,
        memoryAddress, data, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createResponseConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    quint8 sequenceNumber, QKnxControlField::Confirm status)
{
    return createResponseConfirmation(src, dest, memoryAddress, number, data,
        createConfirmationControlField(status),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createResponseConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber, data))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryResponseTpdu(number,
        memoryAddress, data, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createResponseIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    quint8 sequenceNumber)
{
    return createResponseIndication(src, dest, memoryAddress, number, data,
        createIndicationControlField(), createExtentedControlField(QKnxAddress::Type::Individual),
        sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createResponseIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber, data))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryResponseTpdu(number,
        memoryAddress, data, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest,
        tpdu);
}


// -- A_MemoryWrite

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createWriteRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    quint8 sequenceNumber)
{
    return createWriteRequest(src, dest, memoryAddress, number, data, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createWriteRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber, data))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryWriteTpdu(number,
        memoryAddress, data, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createWriteConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    quint8 sequenceNumber, QKnxControlField::Confirm status)
{
    return createWriteConfirmation(src, dest, memoryAddress, number, data,
        createConfirmationControlField(status),
        createExtentedControlField(QKnxAddress::Type::Individual), sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createWriteConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber, data))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryWriteTpdu(number,
        memoryAddress, data, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createWriteIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    quint8 sequenceNumber)
{
    return createWriteIndication(src, dest, memoryAddress, number, data,
        createIndicationControlField(), createExtentedControlField(QKnxAddress::Type::Individual),
        sequenceNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::Memory::createWriteIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint16 memoryAddress, quint8 number, const QKnxByteArray &data,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber)
{
    if (!memoryArgumentsValid(src, dest, extCtrl, number, sequenceNumber, data))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryWriteTpdu(number,
        memoryAddress, data, sequenceNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest,
        tpdu);
}


// -- A_DeviceDescriptor Tools

static bool deviceDescriptorArgumentsValid(const QKnxAddress &src, const QKnxAddress &dest,
    const QKnxExtendedControlField &extCtrl, quint8 descriptorType, quint8 seqNumber,
    QKnxTpduFactory::PointToPoint::Mode mode)
{
    return src.isValid() && src.type() == QKnxAddress::Type::Individual
        && dest.isValid() && dest.type() == QKnxAddress::Type::Individual
        && extCtrl.hopCount() < 7
        && extCtrl.destinationAddressType() == QKnxAddress::Type::Individual
        && descriptorType < 64 && seqNumber <= 15
        && (mode == QKnxTpduFactory::PointToPoint::Mode::Connectionless
            || mode == QKnxTpduFactory::PointToPoint::ConnectionOriented);
}


// -- A_DeviceDescriptorRead

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createReadRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint8 descriptorType, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    return createReadRequest(src, dest, descriptorType, createRequestControlField(),
        createExtentedControlField(QKnxAddress::Type::Individual), mode, seqNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createReadRequest(const QKnxAddress &src,
    const QKnxAddress &dest, quint8 descriptorType, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    if (!deviceDescriptorArgumentsValid(src, dest, extCtrl, descriptorType, seqNumber, mode))
        return {};
    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(mode, descriptorType,
        seqNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createReadConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint8 descriptorType, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber, QKnxControlField::Confirm status)
{
    return createReadConfirmation(src, dest, descriptorType, createConfirmationControlField(status),
        createExtentedControlField(QKnxAddress::Type::Individual), mode, seqNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createReadConfirmation(const QKnxAddress &src,
    const QKnxAddress &dest, quint8 descriptorType, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    if (!deviceDescriptorArgumentsValid(src, dest, extCtrl, descriptorType, seqNumber, mode))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(mode, descriptorType,
        seqNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createReadIndication(const QKnxAddress &src,
    const QKnxAddress &dest, quint8 descriptorType, QKnxTpduFactory::PointToPoint::Mode mode, quint8 seqNumber)
{
    return createReadIndication(src, dest, descriptorType, createIndicationControlField(),
        createExtentedControlField(QKnxAddress::Type::Individual), mode, seqNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createReadIndication(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
    QKnxTpduFactory::PointToPoint::Mode mode, quint8 seqNumber)
{
    if (!deviceDescriptorArgumentsValid(src, dest, extCtrl, descriptorType, seqNumber, mode))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(mode, descriptorType,
        seqNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest, tpdu);
}


// -- A_DeviceDescriptorResponse

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createResponseRequest(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxByteArray &descriptor, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    auto ctrl = createRequestControlField();
    if (descriptor.size() > 15)
        ctrl.setPriority(QKnxControlField::Priority::Low);

    return createResponseRequest(src, dest, descriptorType, descriptor, ctrl,
        createExtentedControlField(QKnxAddress::Type::Individual), mode, seqNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createResponseRequest(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxByteArray &descriptor, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    if (!deviceDescriptorArgumentsValid(src, dest, extCtrl, descriptorType, seqNumber, mode))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(mode,
        descriptorType, descriptor, seqNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataRequest, ctrl, extCtrl, src, dest, tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createResponseConfirmation(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxByteArray &descriptor, QKnxTpduFactory::PointToPoint::Mode mode, quint8 seqNumber,
    QKnxControlField::Confirm status)
{
    auto ctrl = createConfirmationControlField(status);
    if (descriptor.size() > 15)
        ctrl.setPriority(QKnxControlField::Priority::Low);

    return createResponseConfirmation(src, dest, descriptorType, descriptor,
        ctrl, createExtentedControlField(QKnxAddress::Type::Individual), mode, seqNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createResponseConfirmation(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxByteArray &descriptor, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    if (!deviceDescriptorArgumentsValid(src, dest, extCtrl, descriptorType, seqNumber, mode))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(mode,
        descriptorType, descriptor, seqNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataConfirmation, ctrl, extCtrl, src, dest,
        tpdu);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createResponseIndication(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxByteArray &descriptor, QKnxTpduFactory::PointToPoint::Mode mode, quint8 seqNumber)
{
    auto ctrl = createIndicationControlField();
    if (descriptor.size() > 15)
        ctrl.setPriority(QKnxControlField::Priority::Low);

    return createResponseIndication(src, dest, descriptorType, descriptor,
        ctrl, createExtentedControlField(QKnxAddress::Type::Individual), mode, seqNumber);
}

QKnxLinkLayerFrame QKnxLinkLayerFrameFactory::DeviceDescriptor::createResponseIndication(
    const QKnxAddress &src, const QKnxAddress &dest, quint8 descriptorType,
    const QKnxByteArray &descriptor, const QKnxControlField &ctrl,
    const QKnxExtendedControlField &extCtrl, QKnxTpduFactory::PointToPoint::Mode mode,
    quint8 seqNumber)
{
    if (!deviceDescriptorArgumentsValid(src, dest, extCtrl, descriptorType, seqNumber, mode))
        return {};

    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(mode,
        descriptorType, descriptor, seqNumber);
    if (!tpdu.isValid())
        return {};

    return createFrame(QKnxLinkLayerFrame::MessageCode::DataIndication, ctrl, extCtrl, src, dest, tpdu);
}

QT_END_NAMESPACE
