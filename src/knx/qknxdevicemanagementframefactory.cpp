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

#include "qknxdevicemanagementframefactory.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxDeviceManagementFrameFactory

    \inmodule QtKnx
    \brief This class helps build valid CEMI frame of type \l QKnxDeviceManagementFrame.

    This class builds a CEMI frame dedicated to local device management
    One must first choose the service. The services one can choose from are:
    \list
        \li \l PropertyRead
        \li \l PropertyWrite
    \endlist

    Client only services:
    \list
        \li \l FunctionPropertyCommand
        \li \l FunctionPropertyStateRead
        \li \l Reset
    \endlist

    Server only services:
    \list
        \li \l PropertyInfo
        \li \l FunctionPropertyStateResponse
    \endlist

    Then, on creates a request, confirmation or indication, depending on what
    is needed or what is possible, using the functions of the structures
    representing the above named services.

    The typical arguments needed to build a \l QKnxDeviceManagementFrame are:
    \list
        \li The type of the interface object holding the property one wants to
            access \l QKnxInterfaceObjectType
        \li The instance of this interface object (because it is possible to
            have more than one realization of an interface object in a given
            device)
        \li The property of the interface object one wants to access
            \l QKnxInterfaceObjectProperty
        \li The number of elements one wants to read in this property
    \endlist
*/

// -- PropertyRead

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex)
{
    return { QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest, type, instance, pid, numberOfElements,
        startIndex };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation, type, instance, pid,
        numberOfElements, startIndex, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnx::NetIp::CemiServer::Error error)
{
    QKnxDeviceManagementFrame frame { QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    frame.setError(error);
    return frame;
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxDeviceManagementFrame &request, const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation, request.objectType(), request
        .objectInstance(), request.property(), request.numberOfElements(), request.startIndex(),
        data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnx::NetIp::CemiServer::Error error)
{
    QKnxDeviceManagementFrame frame = { QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    frame.setError(error);
    return frame;
}


// -- PropertyWrite

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyWrite::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::PropertyWriteRequest, type, instance, pid, numberOfElements,
        startIndex, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnx::NetIp::CemiServer::Error error)
{
    QKnxDeviceManagementFrame frame = { QKnxDeviceManagementFrame::MessageCode::PropertyWriteConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    if (error != QKnx::NetIp::CemiServer::Error::None)
        frame.setError(error);
    return frame;
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnx::NetIp::CemiServer::Error error)
{
    QKnxDeviceManagementFrame frame = { QKnxDeviceManagementFrame::MessageCode::PropertyWriteConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    if (error != QKnx::NetIp::CemiServer::Error::None)
        frame.setError(error);
    return frame;
}


// -- PropertyInfo

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyInfo::createIndication(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::PropertyInfoIndication, type, instance, pid,
        numberOfElements, startIndex, data };
}


// -- FunctionPropertyCommand

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyCommandRequest, type,
        instance, pid, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation,
        type, instance, pid };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    QKnx::NetIp::CemiServer::ReturnCode code, const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation,
        type, instance, pid, QKnxByteArray { quint8(code) } +data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxDeviceManagementFrame &request)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnx::NetIp::CemiServer::ReturnCode code,
    const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QKnxByteArray { quint8(code) } + data };
}


// -- FunctionPropertyStateRead

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyStateRead
    ::createRequest(QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
        const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadRequest, type,
        instance, pid, data };
}


// -- FunctionPropertyStateResponse

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(QKnxInterfaceObjectType type, quint8 instance,
        QKnxInterfaceObjectProperty pid)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation,
        type, instance, pid };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(QKnxInterfaceObjectType type, quint8 instance,
        QKnxInterfaceObjectProperty pid, QKnx::NetIp::CemiServer::ReturnCode code,
        const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation,
        type, instance, pid, QKnxByteArray { quint8(code) } + data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(const QKnxDeviceManagementFrame &request)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation,
        request.objectType(), request.objectInstance(), request.property() };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(const QKnxDeviceManagementFrame &request,
        QKnx::NetIp::CemiServer::ReturnCode code, const QKnxByteArray &data)
{
    return { QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation,
        request.objectType(), request.objectInstance(), request.property(),
        QKnxByteArray { quint8(code) } + data };
}


// -- Reset

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::Reset::createRequest()
{
    return QKnxDeviceManagementFrame { QKnxDeviceManagementFrame::MessageCode::ResetRequest };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::Reset::createIndication()
{
    return QKnxDeviceManagementFrame { QKnxDeviceManagementFrame::MessageCode::ResetIndication };
}

QT_END_NAMESPACE
