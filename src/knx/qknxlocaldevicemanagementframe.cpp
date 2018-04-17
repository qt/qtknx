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
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

class QKnxLocalDeviceManagementFramePrivate : public QSharedData
{
public:
    QKnxByteArray m_serviceInformation;
    QKnxLocalDeviceManagementFrame::MessageCode m_code { QKnxLocalDeviceManagementFrame
        ::MessageCode::Unknown };
};

/*!
    \class QKnxLocalDeviceManagementFrame

    \inmodule QtKnx
    \brief This class represents a cEMI frame dedicated to communication
    between a cEMI client and a cEMI server. The communication allows access
    to device management functionalities.

    \l QKnxLocalDeviceManagementFrameFactory can be used to construct local
    device management cEMI frames.
*/

/*!
    \enum QKnxLocalDeviceManagementFrame::MessageCode

    This enum describes the different message codes of the local device
    management frame.

    \value Unknown                          Unknown message code, do not use.
    \value PropertyReadRequest
            The message code to be used for a property read request frame
            (M_PropRead.req).
    \value PropertyReadConfirmation
            Message code to be used for a property read confirmation frame
            (M_PropRead.con).
    \value PropertyWriteRequest
            Message code to be used for a property write request frame
            (M_PropWrite.req).
    \value PropertyWriteConfirmation
            Message code to be used for a property write confirmation frame
            (M_PropWrite.con).
    \value PropertyInfoIndication
            Message code to be used for a property info indication frame
            (M_PropInfo.ind).
    \value FunctionPropertyCommandRequest
            Message code to be used for a function property command frame
            (M_FuncPropCommand.req).
    \value FunctionPropertyStateReadRequest
            Message code to be used for a function property state read request
            frame (M_FuncPropStateRead.req).
    \value FunctionPropertyCommandConfirmation
            Message code to be used for a function property command
            confirmation frame (M_FuncPropCommand.con).
    \value FunctionPropertyStateReadConfirmation
            Message code to be used for a function property state read
            confirmation frame (M_FuncPropStateRead.con).
    \value ResetRequest
           Message code to be used for a reset request frame (M_Reset.req).
    \value ResetIndication
           Message code to be used for a reset indication frame (M_Reset.ind).
*/

/*!
    Constructs an empty invalid local device management frame.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame()
    : d_ptr(new QKnxLocalDeviceManagementFramePrivate)
{}

/*!
    Destroys the local device management frame and frees all allocated resources.
*/
QKnxLocalDeviceManagementFrame::~QKnxLocalDeviceManagementFrame()
{}

/*!
    Constructs a empty local device management frame with message code set
    to \a messageCode.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(MessageCode code)
    : d_ptr(new QKnxLocalDeviceManagementFramePrivate)
{
    d_ptr->m_code = code;
    if (code != MessageCode::ResetRequest && code != MessageCode::ResetIndication)
        d_ptr->m_serviceInformation = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
}

/*!
    Constructs a local device management frame with message code set to
    \a messageCode and service information set to \a serviceInfo.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(
        QKnxLocalDeviceManagementFrame::MessageCode code, const QKnxByteArray &serviceInfo)
    : d_ptr(new QKnxLocalDeviceManagementFramePrivate)
{
    d_ptr->m_code = code;
    d_ptr->m_serviceInformation = serviceInfo;
}

/*!
    Returns the number of bytes of the local device management frame.
*/
quint16 QKnxLocalDeviceManagementFrame::size() const
{
    return d_ptr->m_serviceInformation.size() + 1 /* message code */;
}

/*!
    Returns \c true if the local device management frame is valid, returns
    \c false otherwise.
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

/*!
    Returns \c true if this local device management frame is a negative
    confirmation; otherwise returns \c false.

    The data field of a negative confirmation contains the error information.

    \sa data(), error()
*/
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

/*!
    Returns the message code of this local device management frame.
*/
QKnxLocalDeviceManagementFrame::MessageCode QKnxLocalDeviceManagementFrame::messageCode() const
{
    return d_ptr->m_code;
}

/*!
    Sets the message code of this local device management frame to \a code.
*/
void QKnxLocalDeviceManagementFrame::setMessageCode(QKnxLocalDeviceManagementFrame::MessageCode code)
{
    d_ptr->m_code = code;
}

/*
    Returns the object type carried if available;
    otherwise returns \l QKnxInterfaceObjectType::Invalid;
*/
QKnxInterfaceObjectType QKnxLocalDeviceManagementFrame::objectType() const
{
    if (d_ptr->m_serviceInformation.size() >= 2)
        return QKnxUtils::QUint16::fromBytes(d_ptr->m_serviceInformation);
    return QKnxInterfaceObjectType::Invalid;
}

/*
    Set the interface object type to \a type if the argument is a valid;
    otherwise does nothing.
*/
void QKnxLocalDeviceManagementFrame::setObjectType(QKnxInterfaceObjectType type)
{
    if (QKnxInterfaceObjectType::isObjectType(type))
        d_ptr->m_serviceInformation.replace(0, 2, QKnxUtils::QUint16::bytes(quint16(type)));
}

/*!
    Returns the object instance if available; otherwise returns \c 0.
*/
quint8 QKnxLocalDeviceManagementFrame::objectInstance() const
{
    return d_ptr->m_serviceInformation.value(2);
}

/*!
    Sets the object instance to \a instance if the argument is valid; does
    nothing otherwise.

    \note The range for the object instance is from \c 1 to \c 255.
*/
void QKnxLocalDeviceManagementFrame::setObjectInstance(quint8 instance)
{
    if (instance < 1)
        return;
    d_ptr->m_serviceInformation.set(2, instance);
}

/*!
    Returns the interface object property if available; otherwise returns
    \ QKnxInterfaceObjectProperty::Invalid.
*/
QKnxInterfaceObjectProperty QKnxLocalDeviceManagementFrame::property() const
{
    if (d_ptr->m_serviceInformation.size() >= 4)
        return d_ptr->m_serviceInformation.at(3);
    return QKnxInterfaceObjectProperty::Invalid;
}

/*
    Set the interface object property to \a property if the argument is a valid;
    otherwise does nothing.
*/
void QKnxLocalDeviceManagementFrame::setProperty(QKnxInterfaceObjectProperty pid)
{
    if (QKnxInterfaceObjectProperty::isProperty(pid))
        d_ptr->m_serviceInformation.set(3, quint8(pid));
}

/*!
    Returns the number of elements for an array-structured property. A return
    value of \c 0 indicates a negative response.

    \sa isNegativeConfirmation()
*/
quint8 QKnxLocalDeviceManagementFrame::numberOfElements() const
{
    return quint8(d_ptr->m_serviceInformation.value(4) & 0xf0) >> 4;
}

/*!
    Sets the number of elements for an array-structured property. A value of
    \c 0 is used to indicate a negative response.

    \note The range for number of elements is from \c 0 to \c 15.
*/
void QKnxLocalDeviceManagementFrame::setNumberOfElements(quint8 numOfElements)
{
    if (numOfElements > 0x0f)
        return;
    d_ptr->m_serviceInformation.set(4, (d_ptr->m_serviceInformation.value(4) & 0x0f) | (numOfElements << 4));
}

/*!
    Returns the start index within an array-structured property if available;
    otherwise returns \c 0.
*/
quint16 QKnxLocalDeviceManagementFrame::startIndex() const
{
    return QKnxUtils::QUint16::fromBytes(d_ptr->m_serviceInformation, 4) & 0x0fff;
}

/*!
    Sets the start index within an array-structured property, the first
    element is placed at index \c 1; the array element \c 0 contains the
    current number of valid array elements.

    \note The range for number of elements is from \c 0 to \c 4095.
*/
void QKnxLocalDeviceManagementFrame::setStartIndex(quint16 index)
{
    if (index > 0x0fff)
        return;

    auto startIndex = QKnxUtils::QUint16::fromBytes(d_ptr->m_serviceInformation, 4);
    d_ptr->m_serviceInformation.replace(4, 2, QKnxUtils::QUint16::bytes(startIndex | index));
}

/*!
    Returns the data field of the local device management frame as
    an array of bytes.

    The length of the data depends on the property datatype of the property
    and in case of an array-structured property value also on the number of
    array elements that are accessed.

    In case of an error the data field stores the error code as single byte.
*/
QKnxByteArray QKnxLocalDeviceManagementFrame::data() const
{
    return d_ptr->m_serviceInformation.mid(6);
}

/*!
    Sets the data field of the local device management frame to \a newData.
    No validation checks or done on the function argument.

    \sa isValid()
*/
void QKnxLocalDeviceManagementFrame::setData(const QKnxByteArray &newData)
{
    d_ptr->m_serviceInformation.resize(6);
    d_ptr->m_serviceInformation += newData;
}

/*!
    Returns the error code stored in the local device management frame if
    the frame is a \l PropertyReadConfirmation or \l PropertyWriteConfirmation
    frame and the number of elements is set to \0. Otherwise returns \l None.

    \sa data()
*/
QKnx::NetIp::CemiServer::Error QKnxLocalDeviceManagementFrame::error() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation:
        if (numberOfElements() == 0) {
            auto err = data();
            if (err.size() < 1)
                return QKnx::NetIp::CemiServer::Error::Unspecified;
            return QKnx::NetIp::CemiServer::Error(err.at(0));
        }
    default:
        break;
    }
    return QKnx::NetIp::CemiServer::Error::None;
}

/*!
    Sets the error code stored in the frame to \a error if the frame is a
    \l PropertyReadConfirmation or \l PropertyWriteConfirmation frame; does
    nothing otherwise.
*/
void QKnxLocalDeviceManagementFrame::setError(QKnx::NetIp::CemiServer::Error error)
{
    // Set error code on confirmed messages only. See paragraph 4.1.7.3.7.1
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation: {
        if (d_ptr->m_serviceInformation.size() < 7)
            d_ptr->m_serviceInformation.resize(7);
        d_ptr->m_serviceInformation.set(6, quint8(error));
    }
    default:
        break;
    }
}

/*!
    Returns the code set by the cEMI server after a cEMI function property
    service request if the frame is a \l FunctionPropertyCommandConfirmation
    or \l FunctionPropertyStateReadConfirmation frame.

    In all other cases the function will always return \l Error.
*/
QKnx::NetIp::CemiServer::ReturnCode QKnxLocalDeviceManagementFrame::returnCode() const
{
    switch (messageCode()) {
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        if (size() >= 6)
            return QKnx::NetIp::CemiServer::ReturnCode(d_ptr->m_serviceInformation.value(5));
    default:
        break;
    }
    return QKnx::NetIp::CemiServer::ReturnCode::Error;
}

/*!
    Returns the code set by the cEMI server after a cEMI function property
    service request.
*/
void QKnxLocalDeviceManagementFrame::setReturnCode(QKnx::NetIp::CemiServer::ReturnCode code)
{
    switch (messageCode()) {
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        break;
    default:
        return;
    }

    if (d_ptr->m_serviceInformation.size() < 6)
        d_ptr->m_serviceInformation.resize(6);
    d_ptr->m_serviceInformation.set(5, quint8(code));
}

/*!
    Returns the service information of the local device management frame as
    an array of bytes.
*/
QKnxByteArray QKnxLocalDeviceManagementFrame::serviceInformation() const
{
    return d_ptr->m_serviceInformation;
}

/*!
    Sets the service information of the local device management frame to
    \a serviceInfo. No validation checks or done on the function argument.

    \sa isValid()
*/
void QKnxLocalDeviceManagementFrame::setServiceInformation(const QKnxByteArray &serviceInfo)
{
    d_ptr->m_serviceInformation = serviceInfo;
}

/*!
    Returns an array of bytes that represent the local device management frame.
*/
QKnxByteArray QKnxLocalDeviceManagementFrame::bytes() const
{
    return QKnxByteArray { quint8(d_ptr->m_code) } + d_ptr->m_serviceInformation;
}

/*!
    Constructs the local device management frame from the byte array \a data
    starting at position \a index inside the array with given size \a size.
*/
QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrame::fromBytes(const QKnxByteArray &data,
    quint16 index, quint16 size)
{
    if (data.size() < 1)
        return {};
    return { MessageCode(data.at(index)), data.mid(index + 1, size - 1) };
}

/*!
    Constructs a copy of \a other.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(const QKnxLocalDeviceManagementFrame &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxLocalDeviceManagementFrame &
    QKnxLocalDeviceManagementFrame::operator=(const QKnxLocalDeviceManagementFrame &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(QKnxLocalDeviceManagementFrame &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxLocalDeviceManagementFrame &
    QKnxLocalDeviceManagementFrame::operator=(QKnxLocalDeviceManagementFrame &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxLocalDeviceManagementFrame::swap(QKnxLocalDeviceManagementFrame &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxLocalDeviceManagementFrame::operator==(const QKnxLocalDeviceManagementFrame &other) const
{
    return d_ptr == other.d_ptr
        || (d_ptr->m_code == other.d_ptr->m_code
            && d_ptr->m_serviceInformation == other.d_ptr->m_serviceInformation);
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxLocalDeviceManagementFrame::operator!=(const QKnxLocalDeviceManagementFrame &other) const
{
    return !operator==(other);
}

/*!
    \internal
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(MessageCode code,
        QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
        quint8 noe, quint16 index, const QKnxByteArray &payload)
    : QKnxLocalDeviceManagementFrame(code)
{
    d_ptr->m_serviceInformation = QKnxUtils::QUint16::bytes(quint16(type));
    d_ptr->m_serviceInformation.append(instance);
    d_ptr->m_serviceInformation.append(pid);
    d_ptr->m_serviceInformation.append(QKnxUtils::QUint16::bytes((quint16(noe) << 12) | index));
    d_ptr->m_serviceInformation + payload;
}

/*!
    \internal
*/
QKnxLocalDeviceManagementFrame::QKnxLocalDeviceManagementFrame(MessageCode code,
        QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
        const QKnxByteArray &payload)
    : QKnxLocalDeviceManagementFrame(code)
{
    d_ptr->m_serviceInformation = QKnxUtils::QUint16::bytes(quint16(type));
    d_ptr->m_serviceInformation.append(instance);
    d_ptr->m_serviceInformation.append(pid);
    d_ptr->m_serviceInformation + payload;
}

/*!
    \relates QKnxLocalDeviceManagementFrame

    Writes the local device management frame \a frame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxLocalDeviceManagementFrame &frame)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << frame.bytes().toHex();
}

QT_END_NAMESPACE
