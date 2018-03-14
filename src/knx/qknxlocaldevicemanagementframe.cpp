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

#include "qknxlocaldevicemanagementframe.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxLocalDeviceManagementFrame

    \inmodule QtKnx
    \brief This class represents a cEMI frame dedicated to communication
    between a client and a KNXnet/IP server. The communication allow access
    device management functionalities.

    \l QKnxLocalDeviceManagementFrameFactory can be used to construct local
    device management cEMI frames.
*/

/*!
    \enum QKnxLocalDeviceManagementFrame::MessageCode
    This enum describes the different message codes of the Local Device Management frame.

    \value Unknown
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
    Constructs a LocalDeviceManagement frame starting with \a messageCode.

    \note The LocalDeviceManagement frame will be other wise empty and needs to be set by hand.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(MessageCode code)
    : m_code(code)
{
    if (code != MessageCode::ResetRequest && code != MessageCode::ResetIndication)
        m_serviceInformation = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
}
/*!
    Constructs a LocalDeviceManagement frame starting with \a messageCode and with a \l QKnxLocalDeviceManagementPayload \a payload.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(
        QKnxLocalDeviceManagementFrame::MessageCode messageCode, const QKnxByteArray &serviceInfo)
    : m_code(messageCode)
    , m_serviceInformation(serviceInfo)
{}


/*!
    Returns true if the cEMI frame is valid, returns \c false otherwise.
*/
bool QKnxLocalDeviceManagementFrame::isValid() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadRequest:
        if (size() != 7) // 4.1.7.3.2 The request shall not contain any further data.
            return false;
        break;
    case MessageCode::PropertyReadConfirmation:  // 4.1.7.3.3
    case MessageCode::PropertyWriteRequest:      // 4.1.7.3.4
        if (size() < 8)     // The frame shall contain further data, at minimum one quint8.
            return false;   // For 4.1.7.3.3 it shall be (NoE) or error code (negative response).
        break;
    case MessageCode::PropertyWriteConfirmation:
        if ((numberOfElements() >= 1 && size() != 7) || (numberOfElements() == 0 && size() != 8))
            return false; // 4.1.7.3.5 size == (write request - data) or negative confirmation
        break;
    case MessageCode::PropertyInfoIndication:
        if (size() < 7)
            return false;
        break;
    case MessageCode::FunctionPropertyCommandRequest:
    case MessageCode::FunctionPropertyStateReadRequest:
        if (size() < 6)
            return false;
        break;
    case MessageCode::FunctionPropertyCommandConfirmation:
//  case MessageCode::FunctionPropertyStateReadConfirmation: (same value as above)
        if (size() < 5)     // 4.1.7.4.5 Error and exception handling for cEMI Function Properties
            return false;   // return code and data are omitted on error.
        break;
    case MessageCode::ResetRequest:
    case MessageCode::ResetIndication:
        return size() == 1;
    default:
        return false;
    }

    auto type = objectType();
    if (!QKnxInterfaceObjectType::isObjectType(type))
        return false;

    if (objectInstance() < 1)
        return false;

    if (!QKnxInterfaceObjectType::isMatch(type, property()))
        return false;
    return true; // TODO: Find other definitions of validity.
}

bool QKnxLocalDeviceManagementFrame::isNegativeConfirmation() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation:
        // 4.1.7.3.3 Property read / 4.1.7.3.5 Property write confirmation
        // The confirmation indicates an error with number of elements == 0.
        return numberOfElements() == 0;

    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        // 4.1.7.4.5 The confirmation indicates error by omitting return code
        return size() == 5; // and data
    default:
        break;
    }
    return false;
}

QKnxInterfaceObjectType QKnxLocalDeviceManagementFrame::objectType() const
{
    return QKnxInterfaceObjectType(QKnxUtils::QUint16::fromBytes(m_serviceInformation));
}

void QKnxLocalDeviceManagementFrame::setObjectType(QKnxInterfaceObjectType type)
{
    if (!QKnxInterfaceObjectType::isObjectType(type))
        return;
    m_serviceInformation.replace(0, 2, QKnxUtils::QUint16::bytes(quint16(type)));
}

quint8 QKnxLocalDeviceManagementFrame::objectInstance() const
{
    return m_serviceInformation.value(2);
}

void QKnxLocalDeviceManagementFrame::setObjectInstance(quint8 instance)
{
    m_serviceInformation[2] = instance;
}

QKnxInterfaceObjectProperty QKnxLocalDeviceManagementFrame::property() const
{
    return m_serviceInformation.value(3);
}

void QKnxLocalDeviceManagementFrame::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_serviceInformation[3] = quint8(pid);
}

quint8 QKnxLocalDeviceManagementFrame::numberOfElements() const
{
    return quint8(m_serviceInformation.value(4) & 0xf0) >> 4;
}

void QKnxLocalDeviceManagementFrame::setNumberOfElements(quint8 numOfElements)
{
    if (numOfElements > 0x0f)
        return;
    m_serviceInformation[4] = (m_serviceInformation.value(4) & 0x0f) | (numOfElements << 4);
}

quint16 QKnxLocalDeviceManagementFrame::startIndex() const
{
    return QKnxUtils::QUint16::fromBytes(m_serviceInformation, 4) & 0x0fff;
}

void QKnxLocalDeviceManagementFrame::setStartIndex(quint16 index)
{
    if (index > 0x0fff)
        return;

    auto startIndex = QKnxUtils::QUint16::fromBytes(m_serviceInformation, 4);
    m_serviceInformation.replace(4, 2, QKnxUtils::QUint16::bytes((startIndex & 0xf000) | index));
}

QKnx::CemiServer::Error QKnxLocalDeviceManagementFrame::error() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation:
        if (numberOfElements() == 0) {
            auto err = data();
            if (err.size() < 1)
                return QKnx::CemiServer::Error::Unspecified;
            return QKnx::CemiServer::Error(quint8(err[0]));
        }
    default:
        break;
    }
    return QKnx::CemiServer::Error::None;
}

void QKnxLocalDeviceManagementFrame::setError(QKnx::CemiServer::Error error)
{
    // Set error code on confirmed messages only. See paragraph 4.1.7.3.7.1
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation: {
        if (m_serviceInformation.size() < 7)
            m_serviceInformation.resize(7);
        m_serviceInformation[6] = quint8(error);
    }
    default:
        break;
    }
}

QKnx::CemiServer::ReturnCode QKnxLocalDeviceManagementFrame::returnCode() const
{
    switch (messageCode()) {
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        if (size() >= 6)
            return QKnx::CemiServer::ReturnCode(m_serviceInformation.value(5));
    default:
        break;
    }
    return QKnx::CemiServer::ReturnCode::Error;
}

void QKnxLocalDeviceManagementFrame::setReturnCode(QKnx::CemiServer::ReturnCode code)
{
    switch (messageCode()) {
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        break;
    default:
        return;
    }

    if (m_serviceInformation.size() < 6)
        m_serviceInformation.resize(6);
    m_serviceInformation[5] = quint8(code);
}

QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(const QKnxLocalDeviceManagementFrame &other)
{
    m_code = other.messageCode();
    m_serviceInformation = other.serviceInformation();
}

/*!
    Returns the number of bytes of the LocalDeviceManagement frame.
*/
quint16 QKnxLocalDeviceManagementFrame::size() const
{
    return m_serviceInformation.size() + 1 /* message code */;
}

/*!
      Returns the \l QKnxLocalDeviceManagementPayload.
      This is the LocalDeviceManagement frame without the message code.
*/
QKnxByteArray QKnxLocalDeviceManagementFrame::serviceInformation() const
{
    return m_serviceInformation;
}

/*!
    Returns the message code of the LocalDeviceManagement frame.
*/
QKnxLocalDeviceManagementFrame::MessageCode QKnxLocalDeviceManagementFrame::messageCode() const
{
    return m_code;
}

/*!
    Sets the message code of the LocalDeviceManagement frame with \a code.
*/
void QKnxLocalDeviceManagementFrame::setMessageCode(QKnxLocalDeviceManagementFrame::MessageCode code)
{
    m_code = code;
}

QT_END_NAMESPACE
