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
    \class QKnxLinkLayerFrame::Builder

    \inmodule QtKnx
    \brief The QKnxLinkLayerFrame::Builder class provides the means to create a
    KNX link layer frame.

    \note To make use of the class please use the
    \b {#include <QKnxLinkLayerFrameBuilder>} statement.

    A KNX link layer frame contains several fields: message code, additional
    information, control field, maybe an extended control field, a TPDU, as
    well as a source address and destination address. The fields must be valid
    for the builder to be able to create a frame.

    The builder is set up with default values to create a common external
    message interface (cEMI) group value read frame, except for the destination
    address, which needs to be specified during the creation of the frame.

    The following code sample illustrates how to establish a KNXnet/IP tunnel
    connection, to create group value read and write frames, and to send the
    frames over the connection:

    \code
        QKnxNetIpTunnel tunnel(...);
        tunnel.connectToHost(...);

        auto frame = QKnxLinkLayerFrame::builder()
            .setDestinationAddress({ QKnxAddress::Type::Group, QLatin1String("2/1/4") })
            .create();

        tunnel.sendFrame(frame);

        frame = QKnxLinkLayerFrame::builder()
            .setDestinationAddress({ QKnxAddress::Type::Group, QLatin1String("2/1/4") })
            .setTpdu({
                QKnxTpdu::TransportControlField::DataGroup,
                QKnxTpdu::ApplicationControlField::GroupValueWrite,
                QKnxSwitch(QKnxSwitch::State::On).bytes()
            }).create();

        tunnel.sendFrame(frame);
    \endcode

    \sa QKnxLinkLayerFrame
*/

/*!
    Sets the source address to be used by the builder to \a source and returns
    a reference to the builder.

    \sa QKnxAddress
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setSourceAddress(const QKnxAddress &source)
{
    m_src = source;
    return *this;
}

/*!
    Sets the destination address to be used by the builder to \a dest and
    returns a reference to the builder.

    \sa QKnxAddress
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setDestinationAddress(const QKnxAddress &dest)
{
    m_dest = dest;
    return *this;
}

/*!
    Sets the control field to be used by the builder to \a ctrl and returns a
    reference to the builder.

    \sa QKnxControlField
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setControlField(const QKnxControlField &ctrl)
{
    m_ctrl = ctrl;
    return *this;
}

/*!
    Sets the extended control field to be used by the builder to \a extCtrl and
    returns a reference to the builder.

    \sa QKnxExtendedControlField
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setExtendedControlField(const QKnxExtendedControlField &extCtrl)
{
    m_extCtrl = extCtrl;
    return *this;
}

/*!
    Sets the TPDU to be used by the builder to \a tpdu and returns a reference
    to the builder.

    \sa QKnxTpdu
*/
QKnxLinkLayerFrame::Builder &QKnxLinkLayerFrame::Builder::setTpdu(const QKnxTpdu &tpdu)
{
    m_tpdu = tpdu;
    return *this;
}

/*!
    Sets the medium that will determine the format and order of fields to
    \a type and returns a reference to the builder.

    \sa QKnx::MediumType
*/
QKnxLinkLayerFrame::Builder &QKnxLinkLayerFrame::Builder::setMedium(QKnx::MediumType type)
{
    m_mediumType = type;
    return *this;
}

/*!
    Sets an array of bytes specified by \a data with all the fields encoded into
    a byte array starting from the position \a offset to avoid having to use
    each of the field setters separately.

    \sa createFrame(), QKnxByteArray
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setData(const QKnxByteArray &data, quint16 offset)
{
    m_data = data;
    m_dataOffset = offset;
    return *this;
}

/*!
    Sets the link layer frame message code to be used by the builder to \a code
    and returns a reference to the builder.

    \sa QKnxLinkLayerFrame::MessageCode
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setMessageCode(QKnxLinkLayerFrame::MessageCode code)
{
    m_code = code;
    return *this;
}

/*!
    Sets the additional information to be used by the builder to \a infos
    and returns a reference to the builder.

    \sa QKnxAdditionalInfo
*/
QKnxLinkLayerFrame::Builder &
    QKnxLinkLayerFrame::Builder::setAdditionalInfos(const QVector<QKnxAdditionalInfo> &infos)
{
    m_additionalInfos = infos;
    return *this;
}

/*!
    Creates a frame from the values set for the link layer frame message code,
    additional information, control field, extended control field, TPDU, source
    address, and destination address. Alternatively, uses a byte array with all
    the fields already encoded into it.

    \sa QKnxLinkLayerFrame, QKnxLinkLayerFrame::MessageCode, QKnxAdditionalInfo,
    QKnxControlField, QKnxExtendedControlField, QKnxTpdu, QKnxAddress,
    QKnxByteArray
*/
QKnxLinkLayerFrame QKnxLinkLayerFrame::Builder::createFrame() const
{
    if (m_mediumType == QKnx::MediumType::Unknown)
        return {};

    if (m_data.size() > 0) // ignore member variables and create frame from byte representation
        return QKnxLinkLayerFrame::fromBytes(m_data, m_dataOffset, m_data.size(), m_mediumType);

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
