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

void QKnxCemiFrame::setMessageCode(QKnxCemiFrame::MessageCode code)
{
    m_code = code;
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
