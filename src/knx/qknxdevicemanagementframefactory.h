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

#ifndef QKNXDEVICEMANAGEMENTFRAMEFACTORY_H
#define QKNXDEVICEMANAGEMENTFRAMEFACTORY_H

#include <QtKnx/qknxdevicemanagementframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxDeviceManagementFrameFactory final
{
public:
    QKnxDeviceManagementFrameFactory() = delete;

    struct Q_KNX_EXPORT PropertyRead final
    {
        PropertyRead() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            QKnxCemi::Server::Error error = QKnxCemi::Server::Error::None);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request,
            QKnxCemi::Server::Error error);
    };

    struct Q_KNX_EXPORT PropertyWrite final
    {
        PropertyWrite() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            QKnxCemi::Server::Error error = QKnxCemi::Server::Error::None);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request,
            QKnxCemi::Server::Error error = QKnxCemi::Server::Error::None);
    };

    struct Q_KNX_EXPORT PropertyIndication final
    {
        PropertyIndication() = delete;

        static QKnxDeviceManagementFrame create(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QVector<quint8> &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyCommand final
    {
        FunctionPropertyCommand() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyState final
    {
        FunctionPropertyState() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request);

        static QKnxDeviceManagementFrame createConfirmation(const QKnxDeviceManagementFrame &request,
            QKnxCemi::Server::ReturnCode code,
            const QVector<quint8> &data);
    };

    struct Q_KNX_EXPORT Reset
    {
        Reset() = delete;

        static QKnxDeviceManagementFrame createRequest();
        static QKnxDeviceManagementFrame createIndication();
    };
};

QT_END_NAMESPACE

#endif
