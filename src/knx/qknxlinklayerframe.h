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

#ifndef QKNXLINKLAYERFRAME_H
#define QKNXLINKLAYERFRAME_H

#include <QtKnx/qknxadditionalinfo.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxcontrolfield.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtpdu.h>
#include <QtKnx/qknxnamespace.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxLinkLayerFrame final
{
    Q_GADGET

public:
    enum class MessageCode : quint8
    {
        Unknown = 0x00,
        BusmonitorIndication = 0x2b,                    // L_Busmon.ind

        DataRequest = 0x11,                             // L_Data.req
        DataConfirmation = 0x2e,                        // L_Data.con
        DataIndication = 0x29,                          // L_Data.ind

        RawRequest = 0x10,                              // L_Raw.req
        RawIndication = 0x2d,                           // L_Raw.ind
        RawConfirmation = 0x2f,                         // L_Raw.con

        PollDataRequest = 0x13,                         // L_Poll_Data.req
        PollDataConfirmation = 0x25,                    // L_Poll_Data.con

        DataConnectedRequest = 0x21,                    // T_Data_Connected.req
        DataConnectedIndication = 0x89,                 // T_Data_Connected.ind

        DataIndividualRequest = 0x4a,                   // T_Data_Individual.req
        DataIndividualIndication = 0x94,                // T_Data_Individual.ind

        ResetRequest = 0xf1,                            // M_Reset.req
    };
    Q_ENUM(MessageCode)

    QKnxLinkLayerFrame();
    ~QKnxLinkLayerFrame();

    explicit QKnxLinkLayerFrame(QKnx::MediumType mediumType);
    QKnxLinkLayerFrame(QKnx::MediumType mediumType, QKnxLinkLayerFrame::MessageCode messageCode);
    QKnxLinkLayerFrame(const QKnxLinkLayerFrame &other);

    quint16 size() const;

    bool isValid() const;
    bool isMessageCodeValid() const;

    MessageCode messageCode() const;
    void setMessageCode(MessageCode code);

    QKnx::MediumType mediumType() const;
    void setMediumType(QKnx::MediumType mediumType);

    QKnxByteArray serviceInformation() const;
    void setServiceInformation(const QKnxByteArray &serviceInfo);

    QKnxByteArray bytes() const;
    static QKnxLinkLayerFrame fromBytes(const QKnxByteArray &data, quint16 index,
        QKnx::MediumType mediumType);

    // Parts of the LinkLayer frame alway there (regardless of the MessageCode/Frame Type)
    const QKnxAddress sourceAddress() const;
    void setSourceAddress(const QKnxAddress &address);

    const QKnxAddress destinationAddress() const;
    void setDestinationAddress(const QKnxAddress &address);

    QKnxTpdu tpdu() const;
    void setTpdu(const QKnxTpdu &tpdu);

    QKnxControlField controlField() const;
    void setControlField(const QKnxControlField &field);

    // Parts of the LinkLayer Frame that are present or not depending on the
    // MessageCode/ Frame Type or because they are optional
    QKnxExtendedControlField extendedControlField() const;
    void setExtendedControlField(const QKnxExtendedControlField &field);

    quint8 additionalInfosSize() const;
    QVector<QKnxAdditionalInfo> additionalInfos() const;
    void addAdditionalInfo(const QKnxAdditionalInfo &info);
    void removeAdditionalInfo(QKnxAdditionalInfo::Type type);
    void removeAdditionalInfo(const QKnxAdditionalInfo &info);
    void clearAdditionalInfos();

protected:
    QKnxLinkLayerFrame(QKnx::MediumType mediumType, QKnxLinkLayerFrame::MessageCode messageCode,
        const QKnxByteArray &serviceInfo);

private:
    // TODO: introduce d pointer
    MessageCode m_code = MessageCode::Unknown;
    QKnx::MediumType m_mediumType = { QKnx::MediumType::NetIP };
    QKnxAddress m_srcAddress;
    QKnxAddress m_dstAddress;
    QKnxTpdu m_tpdu;
    QKnxControlField m_ctrl;
    QKnxExtendedControlField m_extCtrl;

    quint8 m_additionalInfoSize { 0 };
    mutable bool m_additionalInfosSorted { true };
    mutable QVector<QKnxAdditionalInfo> m_additionalInfos;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxLinkLayerFrame &frame);

QT_END_NAMESPACE

#endif
