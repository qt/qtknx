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

#include "qknxlocaldevicemanagementframefactory.h"

QT_BEGIN_NAMESPACE

// -- PropertyRead

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex)
{
    return { QKnxCemiFrame::MessageCode::PropertyReadRequest, type, instance, pid, numberOfElements,
        startIndex };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyReadConfirmation, type, instance, pid,
        numberOfElements, startIndex, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnxCemi::Server::Error error)
{
    QKnxLocalDeviceManagementFrame frame { QKnxCemiFrame::MessageCode::PropertyReadConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    frame.setError(error);
    return frame;
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyReadConfirmation, request.objectType(), request
        .objectInstance(), request.property(), request.numberOfElements(), request.startIndex(),
        data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnxCemi::Server::Error error)
{
    QKnxLocalDeviceManagementFrame frame = { QKnxCemiFrame::MessageCode::PropertyReadConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    frame.setError(error);
    return frame;
}


// -- PropertyWrite

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyWrite::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyWriteRequest, type, instance, pid, numberOfElements,
        startIndex, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnxCemi::Server::Error error)
{
    QKnxLocalDeviceManagementFrame frame = { QKnxCemiFrame::MessageCode::PropertyWriteConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    if (error != QKnxCemi::Server::Error::None)
        frame.setError(error);
    return frame;
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnxCemi::Server::Error error)
{
    QKnxLocalDeviceManagementFrame frame = { QKnxCemiFrame::MessageCode::PropertyWriteConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    if (error != QKnxCemi::Server::Error::None)
        frame.setError(error);
    return frame;
}


// -- PropertyIndication

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::PropertyIndication::create(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyInfoIndication, type, instance, pid,
        numberOfElements, startIndex, data };
}


// -- FunctionPropertyCommand

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandRequest, type, instance, pid, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, type, instance, pid };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    QKnxCemi::Server::ReturnCode code, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, type, instance, pid,
        QVector<quint8>({ quint8(code) }) + data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnxCemi::Server::ReturnCode code,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QVector<quint8>({ quint8(code) }) + data };
}


// -- FunctionPropertyState

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyState::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadRequest, type, instance, pid, data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, type, instance, pid };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    QKnxCemi::Server::ReturnCode code, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, type, instance, pid,
        QVector<quint8>({ quint8(code) }) + data };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    const QKnxLocalDeviceManagementFrame &request, QKnxCemi::Server::ReturnCode code,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QVector<quint8>({ quint8(code) }) + data };
}


// -- Reset

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::Reset::createRequest()
{
    return QKnxLocalDeviceManagementFrame { QKnxCemiFrame::MessageCode::ResetRequest };
}

QKnxLocalDeviceManagementFrame QKnxLocalDeviceManagementFrameFactory::Reset::createIndication()
{
    return QKnxLocalDeviceManagementFrame { QKnxCemiFrame::MessageCode::ResetIndication };
}

QT_END_NAMESPACE
