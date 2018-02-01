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

#include "qknxlocaldevicemanagementframefactory.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxLocalDeviceManagementFrameFactory

    \inmodule QtKnx
    \brief This class helps build valid CEMI frame of type \l QKnxLocalDeviceManagementFrame.

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

    The typical arguments needed to build a \l QKnxLocalDeviceManagementFrame are:
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

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadRequest, type, instance, pid, numberOfElements,
        startIndex };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadConfirmation, type, instance, pid,
        numberOfElements, startIndex, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnx::CemiServer::Error error)
{
    QKnxLocalDeviceManagementFrame frame { QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    frame.setError(error);
    return frame;
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadConfirmation, request.objectType(), request
        .objectInstance(), request.property(), request.numberOfElements(), request.startIndex(),
        data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnx::CemiServer::Error error)
{
    QKnxLocalDeviceManagementFrame frame = { QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    frame.setError(error);
    return frame;
}


// -- PropertyWrite

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyWrite::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::PropertyWriteRequest, type, instance, pid, numberOfElements,
        startIndex, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnx::CemiServer::Error error)
{
    QKnxLocalDeviceManagementFrame frame = { QKnxLocalDeviceManagementFrame::MessageCode::PropertyWriteConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    if (error != QKnx::CemiServer::Error::None)
        frame.setError(error);
    return frame;
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnx::CemiServer::Error error)
{
    QKnxLocalDeviceManagementFrame frame = { QKnxLocalDeviceManagementFrame::MessageCode::PropertyWriteConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    if (error != QKnx::CemiServer::Error::None)
        frame.setError(error);
    return frame;
}


// -- PropertyInfo

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyInfo::createIndication(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::PropertyInfoIndication, type, instance, pid,
        numberOfElements, startIndex, data };
}


// -- FunctionPropertyCommand

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyCommandRequest, type, instance, pid, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation, type, instance, pid };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    QKnx::CemiServer::ReturnCode code, const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation, type, instance, pid,
        QVector<quint8>({ quint8(code) }) + data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnx::CemiServer::ReturnCode code,
    const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QVector<quint8>({ quint8(code) }) + data };
}


// -- FunctionPropertyStateRead

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyStateRead
    ::createRequest(QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
        const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyStateReadRequest, type, instance, pid, data };
}


// -- FunctionPropertyStateResponse

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(QKnxInterfaceObjectType type, quint8 instance,
        QKnxInterfaceObjectProperty pid)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation, type, instance, pid };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(QKnxInterfaceObjectType type, quint8 instance,
        QKnxInterfaceObjectProperty pid, QKnx::CemiServer::ReturnCode code,
        const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation, type, instance, pid,
        QVector<quint8>({ quint8(code) }) + data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(const QKnxLocalDeviceManagementFrame &request)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyStateResponse
    ::createConfirmation(const QKnxLocalDeviceManagementFrame &request,
        QKnx::CemiServer::ReturnCode code, const QVector<quint8> &data)
{
    return { QKnxLocalDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QVector<quint8>({ quint8(code) }) + data };
}


// -- Reset

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::Reset::createRequest()
{
    return QKnxLocalDeviceManagementFrame { QKnxLocalDeviceManagementFrame::MessageCode::ResetRequest };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::Reset::createIndication()
{
    return QKnxLocalDeviceManagementFrame { QKnxLocalDeviceManagementFrame::MessageCode::ResetIndication };
}

QT_END_NAMESPACE
