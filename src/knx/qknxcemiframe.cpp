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

/*!
    \class QKnxCemiFrame

    \inmodule QtKnx
    \brief The QKnxCemiFrame class represent the Data Link layer frame to be
    included in the KNX/NetIP frame.

    This frame is sent to the KNX bus. It will be interpreted by
    the different layers of the KNX protocol. Reading the bytes from left to
    right, this frame holds:

    \list
        \li The \l QKnxCemiFrame::MessageCode
        \li The \l QKnxAdditionalInfo (optional)
        \li The \l QKnxControlField
        \li The \l QKnxExtendedControlField
        \li The \l QKnxAddress source address
        \li The \l QKnxAddress destination address
        \li The \l QKnxNpdu NPDU (payload)
    \endlist
*/

/*!
    \enum QKnxCemiFrame::MessageCode
    This enum describes the different message codes of the CEMI frame.

    \value Unknown
    \value BusmonitorIndication                       L_Busmon.ind
    \value DataRequest                                L_Data.req
    \value DataConfirmation                           L_Data.con
    \value DataIndication                             L_Data.ind
    \value RawRequest                                 L_Raw.req
    \value RawIndication                              L_Raw.ind
    \value RawConfirmation                            L_Raw.con
    \value PollDataRequest                            L_Poll_Data.req
    \value PollDataConfirmation                       L_Poll_Data.con
    \value DataConnectedRequest                       T_Data_Connected.req
    \value DataConnectedIndication                    T_Data_Connected.ind
    \value DataIndividualRequest                      T_Data_Individual.req
    \value DataIndividualIndication                   T_Data_Individual.ind

    The following values are for Device Management

    \value PropertyReadRequest                        M_PropRead.req
    \value PropertyReadConfirmation                   M_PropRead.con
    \value PropertyWriteRequest                       M_PropWrite.req
    \value PropertyWriteConfirmation                  M_PropWrite.con
    \value PropertyInfoIndication                     M_PropInfo.ind
    \value FunctionPropertyCommandRequest             M_FuncPropCommand.req
    \value FunctionPropertyStateReadRequest           M_FuncPropStateRead.req
    \value FunctionPropertyCommandConfirmation        M_FuncPropCommand.con
    \value FunctionPropertyStateReadConfirmation      M_FuncPropStateRead.con
    \value ResetRequest                               M_Reset.req
    \value ResetIndication                            M_Reset.ind
*/

/*!
    Constructs a CEMI frame starting with \a messageCode.

    \note The CEMI frame will be other wise empty and needs to be set by hand.
*/
QKnxCemiFrame::QKnxCemiFrame(QKnxCemiFrame::MessageCode messageCode)
    : m_code(messageCode)
{}

/*!
    Constructs a CEMI frame starting with \a messageCode and with a \l QKnxCemiPayload \a payload.
*/
QKnxCemiFrame::QKnxCemiFrame(QKnxCemiFrame::MessageCode messageCode, const QKnxCemiPayload &payload)
    : m_code(messageCode)
    , m_serviceInformation(payload)
{}

/*!
    Returns the message code of the CEMI frame.
*/
QKnxCemiFrame::MessageCode QKnxCemiFrame::messageCode() const
{
    return m_code;
}

/*!
    Sets the message code of the CEMI frame with \a code.
*/
void QKnxCemiFrame::setMessageCode(QKnxCemiFrame::MessageCode code)
{
    m_code = code;
}

/*!
    Returns the number of bytes of the CEMI frame.
*/
quint16 QKnxCemiFrame::size() const
{
    return m_serviceInformation.size() + 1 /* message code */;
}

/*!
      Returns the \l QKnxCemiPayload.
      This is the CEMI frame without the message code.
*/
QKnxCemiPayload QKnxCemiFrame::serviceInformation() const
{
    return m_serviceInformation;
}

/*!
    Returns a \l QKnxCemiPayloadRef at the given \a index of the CEMI frame
    payload.
*/
QKnxCemiPayloadRef QKnxCemiFrame::serviceInformationRef(quint16 index) const
{
    return m_serviceInformation.ref(index);
}

/*!
    Return \c true if the CEMI frame is valid; \c false otherwise.

    \note It only checks that the given code is a correct CEMI frame code. It
    does not check the validity of the payload.
*/
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

/*!
    Returns a \l QString representing the bytes of the CEIMI frame
*/
QString QKnxCemiFrame::toString() const
{
    QString tmp;
    for (quint8 byte : m_serviceInformation.ref())
        tmp += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    tmp.chop(2);

    return QStringLiteral("Message code: { 0x%1 }, Service information: { 0x%2 }")
        .arg(quint8(m_code), 2, 16, QLatin1Char('0')).arg(tmp);
}

/*!
    Sets the \l QKnxCemiPayload \a serviceInformation of the CEMI frame.
*/
void QKnxCemiFrame::setServiceInformation(const QKnxCemiPayload &serviceInformation)
{
    m_serviceInformation = serviceInformation;
}

QT_END_NAMESPACE
