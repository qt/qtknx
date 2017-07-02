/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXCEMIFRAME_H
#define QKNXCEMIFRAME_H

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

using QKnxCemiPayload = QKnxNetIpPayload;
using QKnxCemiPayloadRef = QKnxByteStoreRef;

class Q_KNX_EXPORT QKnxCemiFrame
{
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

        // Device Management
        PropertyReadRequest = 0xfc,                     // M_PropRead.req
        PropertyReadConfirmation = 0xfb,                // M_PropRead.con

        PropertyWriteRequest = 0xf6,                    // M_PropWrite.req
        PropertyWriteConfirmation = 0xf5,               // M_PropWrite.con

        PropertyInfoIndication = 0xf7,                  // M_PropInfo.ind

        FunctionPropertyCommandRequest = 0xf8,          // M_FuncPropCommand.req
        FunctionPropertyStateReadRequest = 0xf9,        // M_FuncPropStateRead.req
        FunctionPropertyCommandConfirmation = 0xfa,     // M_FuncPropCommand.con
        FunctionPropertyStateReadConfirmation = 0xfa,   // M_FuncPropStateRead.con

        ResetRequest = 0xf1,                            // M_Reset.req
        ResetIndication = 0xf0,                         // M_Reset.ind
    };
    QKnxCemiFrame::MessageCode messageCode() const;

    virtual ~QKnxCemiFrame() = default;
    quint16 size() const;

    QKnxCemiPayload serviceInformation() const;
    QKnxCemiPayloadRef serviceInformationRef(quint16 index = 0) const;

    virtual bool isValid() const;
    virtual QString toString() const;

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
        static QKnxCemiFrame fromBytes(const T &type, quint16 index, quint16 size)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (type.size() < 1)
            return {};

        QKnxCemiPayload payload;
        auto begin = std::next(std::begin(type), index);
        payload.setBytes(std::next(begin, 1), std::next(begin, size));
        return QKnxCemiFrame(MessageCode(QKnxUtils::QUint8::fromBytes(type, index)), payload);
    }

protected:
    QKnxCemiFrame() = default;

    explicit QKnxCemiFrame(QKnxCemiFrame::MessageCode messageCode);
    QKnxCemiFrame(QKnxCemiFrame::MessageCode messageCode, const QKnxCemiPayload &payload);

    void setServiceInformation(const QKnxCemiPayload &serviceInformation);

private:
    QKnxCemiFrame::MessageCode m_code;
    QKnxCemiPayload m_serviceInformation;
};

QT_END_NAMESPACE

#endif
