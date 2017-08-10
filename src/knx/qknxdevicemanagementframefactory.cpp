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

#include "qknxdevicemanagementframefactory.h"

QT_BEGIN_NAMESPACE

// -- PropertyRead

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex)
{
    return { QKnxCemiFrame::MessageCode::PropertyReadRequest, type, instance, pid, numberOfElements,
        startIndex };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyReadConfirmation, type, instance, pid,
        numberOfElements, startIndex, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnxCemi::Server::Error error)
{
    QKnxDeviceManagementFrame frame { QKnxCemiFrame::MessageCode::PropertyReadConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    frame.setError(error);
    return frame;
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxDeviceManagementFrame &request, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyReadConfirmation, request.objectType(), request
        .objectInstance(), request.property(), request.numberOfElements(), request.startIndex(),
        data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyRead::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnxCemi::Server::Error error)
{
    QKnxDeviceManagementFrame frame = { QKnxCemiFrame::MessageCode::PropertyReadConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    frame.setError(error);
    return frame;
}


// -- PropertyWrite

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyWrite::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyWriteRequest, type, instance, pid, numberOfElements,
        startIndex, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, QKnxCemi::Server::Error error)
{
    QKnxDeviceManagementFrame frame = { QKnxCemiFrame::MessageCode::PropertyWriteConfirmation, type,
        instance, pid, numberOfElements, startIndex };
    if (error != QKnxCemi::Server::Error::None)
        frame.setError(error);
    return frame;
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyWrite::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnxCemi::Server::Error error)
{
    QKnxDeviceManagementFrame frame = { QKnxCemiFrame::MessageCode::PropertyWriteConfirmation,
        request.objectType(), request.objectInstance(), request.property(), 0, request.startIndex() };
    if (error != QKnxCemi::Server::Error::None)
        frame.setError(error);
    return frame;
}


// -- PropertyIndication

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::PropertyIndication::create(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    quint8 numberOfElements, quint16 startIndex, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::PropertyInfoIndication, type, instance, pid,
        numberOfElements, startIndex, data };
}


// -- FunctionPropertyCommand

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandRequest, type, instance, pid, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, type, instance, pid };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    QKnxCemi::Server::ReturnCode code, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, type, instance, pid,
        QVector<quint8>({ quint8(code) }) + data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxDeviceManagementFrame &request)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyCommand::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnxCemi::Server::ReturnCode code,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QVector<quint8>({ quint8(code) }) + data };
}


// -- FunctionPropertyState

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyState::createRequest(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadRequest, type, instance, pid, data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, type, instance, pid };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
    QKnxCemi::Server::ReturnCode code, const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, type, instance, pid,
        QVector<quint8>({ quint8(code) }) + data };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    const QKnxDeviceManagementFrame &request)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, request.objectType(),
        request.objectInstance(), request.property() };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::FunctionPropertyState::createConfirmation(
    const QKnxDeviceManagementFrame &request, QKnxCemi::Server::ReturnCode code,
    const QVector<quint8> &data)
{
    return { QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation, request.objectType(),
        request.objectInstance(), request.property(), QVector<quint8>({ quint8(code) }) + data };
}


// -- Reset

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::Reset::createRequest()
{
    return QKnxDeviceManagementFrame { QKnxCemiFrame::MessageCode::ResetRequest };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrameFactory::Reset::createIndication()
{
    return QKnxDeviceManagementFrame { QKnxCemiFrame::MessageCode::ResetIndication };
}

QT_END_NAMESPACE
