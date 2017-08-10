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

#include "qknxcemi.h"
#include "qknxdevicemanagementframe.h"

QT_BEGIN_NAMESPACE

QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(QKnxDeviceManagementFrame::MessageCode code)
    : QKnxCemiFrame(code)
{
    if (code != QKnxCemiFrame::MessageCode::ResetRequest
        && code != QKnxCemiFrame::MessageCode::ResetIndication) {
        static const  constexpr quint8 data[6] = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
        setServiceInformation({ data, 6 });
    }
}

bool QKnxDeviceManagementFrame::isValid() const
{
    switch (messageCode()) {
    case QKnxCemiFrame::MessageCode::PropertyReadRequest:
        if (size() != 7) // 4.1.7.3.2 The request shall not contain any further data.
            return false;
        break;
    case QKnxCemiFrame::MessageCode::PropertyReadConfirmation:  // 4.1.7.3.3
    case QKnxCemiFrame::MessageCode::PropertyWriteRequest:      // 4.1.7.3.4
        if (size() < 8)     // The frame shall contain further data, at minimum one quint8.
            return false;   // For 4.1.7.3.3 it shall be (NoE) or error code (negative response).
        break;
    case QKnxCemiFrame::MessageCode::PropertyWriteConfirmation:
        if ((numberOfElements() >= 1 && size() != 7) || (numberOfElements() == 0 && size() != 8))
            return false; // 4.1.7.3.5 size == (write request - data) or negative confirmation
        break;
    case QKnxCemiFrame::MessageCode::PropertyInfoIndication:
        if (size() < 7)
            return false;
        break;
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandRequest:
    case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadRequest:
        if (size() < 6)
            return false;
        break;
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation:
//  case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation: (same value as above)
        if (size() < 5)     // 4.1.7.4.5 Error and exception handling for cEMI Function Properties
            return false;   // return code and data are omitted on error.
        break;
    case QKnxCemiFrame::MessageCode::ResetRequest:
    case QKnxCemiFrame::MessageCode::ResetIndication:
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

bool QKnxDeviceManagementFrame::isNegativeConfirmation() const
{
    switch (messageCode()) {
    case QKnxCemiFrame::MessageCode::PropertyReadConfirmation:
    case QKnxCemiFrame::MessageCode::PropertyWriteConfirmation:
        // 4.1.7.3.3 Property read / 4.1.7.3.5 Property write confirmation
        // The confirmation indicates an error with number of elements == 0.
        return numberOfElements() == 0;

    //case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation:
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation:
        // 4.1.7.4.5 The confirmation indicates error by omitting return code
        return size() == 5; // and data
    default:
        break;
    }
    return false;
}

QKnxInterfaceObjectType QKnxDeviceManagementFrame::objectType() const
{
    return QKnxInterfaceObjectType(QKnxUtils::QUint16::fromBytes(serviceInformationRef()));
}

void QKnxDeviceManagementFrame::setObjectType(QKnxInterfaceObjectType type)
{
    if (!QKnxInterfaceObjectType::isObjectType(type))
        return;

    auto si = serviceInformation();
    si.replaceBytes(0, QKnxUtils::QUint16::bytes(quint16(type)));
    setServiceInformation(si);
}

quint8 QKnxDeviceManagementFrame::objectInstance() const
{
    return serviceInformationRef().byte(2);
}

void QKnxDeviceManagementFrame::setObjectInstance(quint8 instance)
{
    auto si = serviceInformation();
    si.replaceBytes<std::array<quint8, 1>, 1>(2, { { instance } });
    setServiceInformation(si);
}

QKnxInterfaceObjectProperty QKnxDeviceManagementFrame::property() const
{
    return serviceInformationRef().byte(3);
}

void QKnxDeviceManagementFrame::setProperty(QKnxInterfaceObjectProperty pid)
{
    auto si = serviceInformation();
    si.replaceBytes<std::array<quint8, 1>, 1>(3, { { quint8(pid) } });
    setServiceInformation(si);
}

quint8 QKnxDeviceManagementFrame::numberOfElements() const
{
    return quint8(serviceInformationRef().byte(4) & 0xf0) >> 4;
}

void QKnxDeviceManagementFrame::setNumberOfElements(quint8 numOfElements)
{
    if (numOfElements > 0xf)
        return;

    auto si = serviceInformation();
    si.setByte(4, (si.byte(4) & 0xf) | (numOfElements << 4));
    setServiceInformation(si);
}

quint16 QKnxDeviceManagementFrame::startIndex() const
{
    return QKnxUtils::QUint16::fromBytes(serviceInformationRef(4)) & 0x0fff;
}

void QKnxDeviceManagementFrame::setStartIndex(quint16 index)
{
    if (index > 0x0fff)
        return;

    auto si = serviceInformation();
    auto startIndex = QKnxUtils::QUint16::fromBytes(serviceInformationRef(4));
    si.replaceBytes(4, QKnxUtils::QUint16::bytes((startIndex & 0xf000) | index));
    setServiceInformation(si);
}

QKnxCemi::Server::Error QKnxDeviceManagementFrame::error() const
{
    switch (messageCode()) {
    case QKnxCemiFrame::MessageCode::PropertyReadConfirmation:
    case QKnxCemiFrame::MessageCode::PropertyWriteConfirmation:
        if (numberOfElements() == 0) {
            auto err = data();
            if (err.size() < 1)
                return QKnxCemi::Server::Error::Unspecified;
            return QKnxCemi::Server::Error(quint8(err[0]));
        }
    default:
        break;
    }
    return QKnxCemi::Server::Error::None;
}

void QKnxDeviceManagementFrame::setError(QKnxCemi::Server::Error error)
{
    // Set error code on confirmed messages only. See paragraph 4.1.7.3.7.1
    switch (messageCode()) {
    case QKnxCemiFrame::MessageCode::PropertyReadConfirmation:
    case QKnxCemiFrame::MessageCode::PropertyWriteConfirmation: {
        auto sf = serviceInformation();
        if (sf.size() < 7)
            sf.resize(7);
        sf.setByte(6, quint8(error));
        setServiceInformation(sf);
    }
    default:
        break;
    }
}

QKnxCemi::Server::ReturnCode QKnxDeviceManagementFrame::returnCode() const
{
    switch (messageCode()) {
    //case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation:
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation:
        if (size() >= 6)
            return QKnxCemi::Server::ReturnCode(serviceInformationRef().byte(5));
    default:
        break;
    }
    return QKnxCemi::Server::ReturnCode::Error;
}

void QKnxDeviceManagementFrame::setReturnCode(QKnxCemi::Server::ReturnCode code)
{
    switch (messageCode()) {
    //case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation:
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation:
        break;
    default:
        return;
    }

    auto sf = serviceInformation();
    if (sf.size() < 6)
        sf.resize(6);
    sf.setByte(5, quint8(code));
    setServiceInformation(sf);
}

QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(const QKnxCemiFrame &other)
    : QKnxCemiFrame(other)
{}

QT_END_NAMESPACE
