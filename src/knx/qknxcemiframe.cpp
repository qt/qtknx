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

#include "qknxcemiframe.h"

QT_BEGIN_NAMESPACE

QKnxCemiFrame::QKnxCemiFrame(QKnxCemiFrame::MessageCode messageCode)
    : m_code(messageCode)
{}

QKnxCemiFrame::QKnxCemiFrame(QKnxCemiFrame::MessageCode messageCode, const QKnxCemiPayload &payload)
    : m_code(messageCode)
    , m_serviceInformation(payload)
{}

QKnxCemiFrame::MessageCode QKnxCemiFrame::messageCode() const
{
    return m_code;
}

quint16 QKnxCemiFrame::size() const
{
    return m_serviceInformation.size() + 1 /* message code */;
}

QKnxCemiPayload QKnxCemiFrame::serviceInformation() const
{
    return m_serviceInformation;
}

QKnxCemiPayloadRef QKnxCemiFrame::serviceInformationRef(quint16 index) const
{
    return m_serviceInformation.ref(index);
}

bool QKnxCemiFrame::isValid() const
{
    switch (m_code) {
    case MessageCode::BusmonitorIndication:
    case MessageCode::DataRequest:
    case MessageCode::DataConfirmation:
    case MessageCode::DataIndication:
    case MessageCode::RawRequest:
    case MessageCode::RawIndication:
    case MessageCode::RawConfirmation:
    case MessageCode::PollDataRequest:
    case MessageCode::PollDataConfirmation:
    case MessageCode::DataConnectedRequest:
    case MessageCode::DataConnectedIndication:
    case MessageCode::DataIndividualRequest:
    case MessageCode::DataIndividualIndication:
    case MessageCode::PropertyReadRequest:
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteRequest:
    case MessageCode::PropertyWriteConfirmation:
    case MessageCode::PropertyInfoIndication:
    case MessageCode::FunctionPropertyCommandRequest:
    case MessageCode::FunctionPropertyStateReadRequest:
    case MessageCode::FunctionPropertyCommandConfirmation:
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::ResetRequest:
    case MessageCode::ResetIndication:
        return true;
    default:
        break;
    }
    return false;
}

QString QKnxCemiFrame::toString() const
{
    QString tmp;
    for (quint8 byte : m_serviceInformation.ref())
        tmp += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    tmp.chop(2);

    return QStringLiteral("Message code: { 0x%1 }, Service information: { 0x%2 }")
        .arg(quint8(m_code), 2, 16, QLatin1Char('0')).arg(tmp);
}

void QKnxCemiFrame::setServiceInformation(const QKnxCemiPayload &serviceInformation)
{
    m_serviceInformation = serviceInformation;
}

QT_END_NAMESPACE
