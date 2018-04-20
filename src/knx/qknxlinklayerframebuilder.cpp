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

#include "qknxlinklayerframebuilder.h"

// TODO: introduce QKnx::MediumType dependency. The LinkLayer Frame look different
// depending on the chosen medium. For the moment only netIp Tunneling is taken care of.
// But GroupValue service can be send via L_Data (netIp) or L_Data_Standard / L_Data_Extended
// if medium is TP of PL. Maybe transform this factory into a builder, somehow "linked" to
// the LinkLayerFrame, so that the MediumType is known.

QT_BEGIN_NAMESPACE

/*!
    \class QKnxLinkLayerFrameBuilder

    \inmodule QtKnx
    \brief The QKnxLinkLayerFrameBuilder class allows you to create a
    QKnxLinkLayerFrame.

    A KNX link layer frame contains several fields: message code, additional
    information, control field, maybe an extended control field, a TPDU, source
    address and destination address. The builder requires valid fields to be
    able to create a frame.

    The builder is setup with default values to create a cEMI group value read
    frame, only the destination address needs to be specified during creation.

    \code
        // establish a KNXnet/IP tunnel connection
        QKnxNetIpTunnelConnection tunnel(...);
        tunnel.connectToHost(...);

        // create a group value read frame to be send via a KNXnet/IP tunnel
        auto frame = QKnxLinkLayerFrameBuilder()
            .setDestinationAddress({ QKnxAddress::Type::Group, QLatin1String("2/1/4") })
            .create();

        // send the frame
        tunnel.sendTunnelFrame(frame);

        // create a group value write frame to be send via a KNXnet/IP tunnel
        frame = QKnxLinkLayerFrameBuilder()
            .setDestinationAddress({ QKnxAddress::Type::Group, QLatin1String("2/1/4") })
            .setTpdu({
                QKnxTpdu::TransportControlField::DataGroup,
                QKnxTpdu::ApplicationControlField::GroupValueWrite,
                QKnxSwitch(QKnxSwitch::State::On).bytes()
             })
            .create();

        // send the frame
        tunnel.sendTunnelFrame(frame);
    \endcode

    \sa QKnxLinkLayerFrame
*/

/*!
    Sets the source address to \a source and returns a reference to the builder.

    \sa QKnxAddress
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setSourceAddress(const QKnxAddress &source)
{
    m_src = source;
    return *this;
}

/*!
    Sets the destination address to \a dest and returns a reference to the builder.

    \sa QKnxAddress
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setDestinationAddress(const QKnxAddress &dest)
{
    m_dest = dest;
    return *this;
}

/*!
    Sets the control field to \a ctrl and returns a reference to the builder.

    \sa QKnxControlField
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setControlField(const QKnxControlField &ctrl)
{
    m_ctrl = ctrl;
    return *this;
}

/*!
    Sets the extended control field to \a extCtrl and returns a reference to the builder.

    \sa QKnxExtendedControlField
*/
QKnxLinkLayerFrameBuilder &
    QKnxLinkLayerFrameBuilder::setExtendedControlField(const QKnxExtendedControlField &extCtrl)
{
    m_extCtrl = extCtrl;
    return *this;
}

/*!
    Sets a TPDU to \a tpdu and returns a reference to the builder.

    \sa QKnxTpdu
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setTpdu(const QKnxTpdu &tpdu)
{
    m_tpdu = tpdu;
    return *this;
}

/*!
    Sets the medium that will determine the format and order of fields to
    \a type and returns a reference to the builder.
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setMedium(QKnx::MediumType type)
{
    m_mediumType = type;
    return *this;
}

/*!
    Sets an array of bytes with all the fields encoded into the QKnxByteArray. Avoids
    using each of the field setters.

    \sa createFrame()
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setData(const QKnxByteArray &data, quint16 offset)
{
    m_data = data;
    m_dataOffset = offset;
    return *this;
}

/*!
    Sets a QKnxLinkLayerFrame::MessageCode to be used by the
    builder when createFrame() is called.
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setMsgCode(QKnxLinkLayerFrame::MessageCode code)
{
    m_code = code;
    return *this;
}

/*!
    Sets a QVector<QKnxAdditionalInfo> to be used by the builder
    when createFrame() is called.
*/
QKnxLinkLayerFrameBuilder &QKnxLinkLayerFrameBuilder::setAdditionalInfos(const QVector<QKnxAdditionalInfo> &infos)
{
    m_additionalInfos = infos;
    return *this;
}

/*!
    Creates a frame from: QKnxLinkLayerFrame::MessageCode, additional information,
    QKnxControlField, QKnxExtendedControlField, QKnxTpdu, source
    address and destination address; or from a QKnxByteArray with
    all fields already encoded into it.

    \sa QKnxLinkLayerFrame
*/
QKnxLinkLayerFrame QKnxLinkLayerFrameBuilder::createFrame() const
{
    if (m_mediumType == QKnx::MediumType::Unknown)
        return {};

    if (m_data.size() > 0) // ignore member variables and create frame from byte representation
        return QKnxLinkLayerFrame::fromBytes(m_data, m_dataOffset, m_mediumType);

    if (m_dest.type() != m_extCtrl.destinationAddressType()
        || !m_src.isValid()
        || !m_dest.isValid()
        || !m_tpdu.isValid()
        || m_extCtrl.hopCount() >= 7
        || m_tpdu.sequenceNumber() > 15)
        return {};

    QKnxLinkLayerFrame frame;
    frame.setDestinationAddress(m_dest);
    frame.setSourceAddress(m_src);
    // if data transmitted is over 15 bytes an extended frame is needed
    if (m_tpdu.dataSize() > 15) {
        m_ctrl.setFrameFormat(QKnxControlField::FrameFormat::Extended);
        m_ctrl.setPriority(QKnxControlField::Priority::Low);
    }
    frame.setControlField(m_ctrl);
    frame.setExtendedControlField(m_extCtrl);
    frame.setTpdu(m_tpdu);
    frame.setMediumType(m_mediumType);
    frame.setMessageCode(m_code);
    for (const auto &info : m_additionalInfos)
        frame.addAdditionalInfo(info);
    return frame;
}

QT_END_NAMESPACE
