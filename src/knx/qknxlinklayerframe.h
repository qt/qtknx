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
#include <QtKnx/qknxcontrolfield.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtpdu.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxnamespace.h>

QT_BEGIN_NAMESPACE

using QKnxLinkLayerPayload = QKnxNetIpPayload;// TODO remove the QKnxNetIpPayLoad dependency
using QKnxLinkLayerPayloadRef = QKnxByteStoreRef;

class Q_KNX_EXPORT QKnxLinkLayerFrame final
{
    Q_GADGET

public:
    // Table 1 - Overview EMI message codes and default destination (v01.03.03 AS.pdf)
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
    MessageCode messageCode() const;
    void setMessageCode(MessageCode code);
    QKnx::MediumType mediumType() const;
    void setMediumType(QKnx::MediumType mediumType);


    QKnxLinkLayerFrame() = default;
    ~QKnxLinkLayerFrame() = default;

    explicit QKnxLinkLayerFrame(QKnx::MediumType mediumType);
    QKnxLinkLayerFrame(QKnx::MediumType mediumType, QKnxLinkLayerFrame::MessageCode messageCode);
    QKnxLinkLayerFrame(const QKnxLinkLayerFrame &other);

    quint16 size() const;
    QString toString() const;
    bool isValid() const;
    bool isMessageCodeValid() const;

    QKnxLinkLayerPayload serviceInformation() const;
    QKnxLinkLayerPayloadRef serviceInformationRef(quint16 index = 0) const;

    template <typename T = QByteArray> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(m_serviceInformation.size() + 1, quint8(m_code));
        auto ref = m_serviceInformation.ref();
        std::copy(std::begin(ref), std::end(ref), std::next(std::begin(t), 1));

        return t;
    }

    template <typename T, std::size_t S = 0>
        static QKnxLinkLayerFrame fromBytes(const T &type, quint16 index, quint16 size,
        QKnx::MediumType mediumType = QKnx::MediumType::Unknown)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (type.size() < 1)
            return {};

        QKnxLinkLayerPayload payload;
        auto begin = std::next(std::begin(type), index);
        payload.setBytes(std::next(begin, 1), std::next(begin, size));

        MessageCode code = MessageCode(QKnxUtils::QUint8::fromBytes(type, index));
        if (mediumType == QKnx::MediumType::Unknown)
            mediumType = guessMediumType(code);

        return QKnxLinkLayerFrame(mediumType, code, payload);
    }

    // Parts of the LinkLayer frame alway there (regardless of the MessageCode/Frame Type)
    const QKnxAddress sourceAddress() const;
    void setSourceAddress(const QKnxAddress &address);

    const QKnxAddress destinationAddress() const;
    void setDestinationAddress(const QKnxAddress &address);

    QKnxTpdu tpdu() const;
    void setTpdu(const QKnxTpdu &tpdu);

    QKnxControlField controlField() const;
    void setControlField(const QKnxControlField &field);

    // Parts of the LinkLayer Frame that are present or not depending on the MessageCode/ Frame Type
    // or because thez are optional
    QKnxExtendedControlField extendedControlField() const;
    void setExtendedControlField(const QKnxExtendedControlField &field); // TODO: check if there is an extended control field!

    quint8 additionalInfosSize() const;

    void addAdditionalInfo(const QKnxAdditionalInfo &info);

    template <typename T = QVector<QKnxAdditionalInfo>> auto additionalInfos() const -> decltype(T())
    {
        static_assert(is_type<T, QVector<QKnxAdditionalInfo>, std::deque<QKnxAdditionalInfo>,
            std::vector<QKnxAdditionalInfo>>::value, "Type not supported.");

        const auto &store = serviceInformationRef();
        if (store.size() < 1)
            return {};

        quint8 size = store.byte(0);
        if (size < 0x02 || size == 0xff)
            return {};

        T infos;
        quint8 index = 1;
        while (index < size) {
            infos.push_back(QKnxAdditionalInfo::fromBytes(store, index));
            index += store.byte(index + 1) + 2; // type + size => 2
        }
        return infos;
    }
    void removeAdditionalInfo(QKnxAdditionalInfo::Type type);
    void removeAdditionalInfo(const QKnxAdditionalInfo &info);
    void clearAdditionalInfos();

protected:
    QKnxLinkLayerFrame(QKnx::MediumType mediumType, QKnxLinkLayerFrame::MessageCode messageCode, const QKnxLinkLayerPayload &payload);
    void setServiceInformation(const QKnxLinkLayerPayload &serviceInformation);

private:
    // TODO: introduce d pointer
    MessageCode m_code = MessageCode::Unknown;
    QKnx::MediumType m_mediumType = QKnx::MediumType::Unknown;
    QKnxLinkLayerPayload m_serviceInformation;

    // TODO: Move into .cpp file once ::fromBytes is there as well.
    static QKnx::MediumType guessMediumType(MessageCode messageCode)
    {
        switch (messageCode) {
        case MessageCode::BusmonitorIndication:
        case MessageCode::DataRequest:
        case MessageCode::DataConfirmation:
        case MessageCode::DataIndication:
        case MessageCode::RawRequest:
        case MessageCode::RawIndication:
        case MessageCode::RawConfirmation:
        case MessageCode::ResetRequest:
            return QKnx::MediumType::NetIP;
        // The following message code are not part of what netIp/Tunneling can send
        // Not quite sure to which medium they can be associated
        // TODO: associate those message code to a give MediumType
        case MessageCode::PollDataRequest:
        case MessageCode::PollDataConfirmation:
        case MessageCode::DataConnectedRequest:
        case MessageCode::DataConnectedIndication:
        case MessageCode::DataIndividualRequest:
        case MessageCode::DataIndividualIndication:
        default:
            break;
        }
        return QKnx::MediumType::Unknown;

    }
};

QT_END_NAMESPACE

#endif
