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

#ifndef QKNXLOCALDEVICEMANAGEMENTFRAMEFACTORY_H
#define QKNXLOCALDEVICEMANAGEMENTFRAMEFACTORY_H

#include <QtKnx/qknxlocaldevicemanagementframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxLocalDeviceManagementFrameFactory final
{
public:
    QKnxLocalDeviceManagementFrameFactory() = delete;

    struct Q_KNX_EXPORT PropertyRead final
    {
        PropertyRead() = delete;

        static QKnxLocalDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            QKnxCemi::Server::Error error = QKnxCemi::Server::Error::None);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request,
            QKnxCemi::Server::Error error);
    };

    struct Q_KNX_EXPORT PropertyWrite final
    {
        PropertyWrite() = delete;

        static QKnxLocalDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            QKnxCemi::Server::Error error = QKnxCemi::Server::Error::None);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request,
            QKnxCemi::Server::Error error = QKnxCemi::Server::Error::None);
    };

    struct Q_KNX_EXPORT PropertyIndication final
    {
        PropertyIndication() = delete;

        static QKnxLocalDeviceManagementFrame create(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QVector<quint8> &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyCommand final
    {
        FunctionPropertyCommand() = delete;

        static QKnxLocalDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyState final
    {
        FunctionPropertyState() = delete;

        static QKnxLocalDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid);

        static QKnxLocalDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request);

        static QKnxLocalDeviceManagementFrame createConfirmation(const QKnxLocalDeviceManagementFrame &request,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);
    };

    struct Q_KNX_EXPORT Reset
    {
        Reset() = delete;

        static QKnxLocalDeviceManagementFrame createRequest();
        static QKnxLocalDeviceManagementFrame createIndication();
    };
};

QT_END_NAMESPACE

#endif
