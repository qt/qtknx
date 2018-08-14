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

#ifndef QKNXLINKLAYERFRAMEBUILDER_H
#define QKNXLINKLAYERFRAMEBUILDER_H

#include <QtCore/qvector.h>
#include <QtKnx/qknxadditionalinfo.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxcontrolfield.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxlinklayerframe.h>
#include <QtKnx/qknxnamespace.h>
#include <QtKnx/qknxtpdu.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxLinkLayerFrame::Builder final
{
public:
    Builder() = default;
    Builder &setSourceAddress(const QKnxAddress &source);
    Builder &setDestinationAddress(const QKnxAddress &dest);
    Builder &setControlField(const QKnxControlField &ctrl);
    Builder &setExtendedControlField(const QKnxExtendedControlField &extCtrl);
    Builder &setTpdu(const QKnxTpdu &tpdu);
    Builder &setMedium(QKnx::MediumType type);
    Builder &setData(const QKnxByteArray &data, quint16 offset = 0);
    Builder &setMessageCode(QKnxLinkLayerFrame::MessageCode code);
    Builder &setAdditionalInfos(const QVector<QKnxAdditionalInfo> &infos);

    QKnxLinkLayerFrame createFrame() const;

private:
    QKnxLinkLayerFrame::MessageCode m_code { QKnxLinkLayerFrame::MessageCode::DataRequest };
    QVector<QKnxAdditionalInfo> m_additionalInfos;
    QKnxAddress m_src { QKnxAddress::Type::Individual, 0x0000 };
    QKnxAddress m_dest;
    mutable QKnxControlField m_ctrl { 0xbc };
    QKnxExtendedControlField m_extCtrl { 0xe0 };
    QKnxTpdu m_tpdu { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueRead };
    QKnx::MediumType m_mediumType { QKnx::MediumType::NetIP };
    QKnxByteArray m_data;
    quint16 m_dataOffset { 0 };
};

QT_END_NAMESPACE

#endif
