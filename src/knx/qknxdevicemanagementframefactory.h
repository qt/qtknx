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
            const QKnxByteArray &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            QKnx::NetIp::CemiServer::Error error = QKnx::NetIp::CemiServer::Error::None);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request,
            const QKnxByteArray &data);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request,
            QKnx::NetIp::CemiServer::Error error);
    };

    struct Q_KNX_EXPORT PropertyWrite final
    {
        PropertyWrite() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QKnxByteArray &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            QKnx::NetIp::CemiServer::Error error = QKnx::NetIp::CemiServer::Error::None);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request,
            QKnx::NetIp::CemiServer::Error error = QKnx::NetIp::CemiServer::Error::None);
    };

    struct Q_KNX_EXPORT PropertyInfo final
    {
        PropertyInfo() = delete;

        static QKnxDeviceManagementFrame createIndication(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            quint8 numberOfElements,
            quint16 startIndex,
            const QKnxByteArray &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyCommand final
    {
        FunctionPropertyCommand() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            const QKnxByteArray &data);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            QKnx::NetIp::CemiServer::ReturnCode code,
            const QKnxByteArray &data);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request,
            QKnx::NetIp::CemiServer::ReturnCode code,
            const QKnxByteArray &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyStateRead final
    {
        FunctionPropertyStateRead() = delete;

        static QKnxDeviceManagementFrame createRequest(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            const QKnxByteArray &data);
    };

    struct Q_KNX_EXPORT FunctionPropertyStateResponse final
    {
        FunctionPropertyStateResponse() = delete;

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid);

        static QKnxDeviceManagementFrame createConfirmation(QKnxInterfaceObjectType type,
            quint8 instance,
            QKnxInterfaceObjectProperty pid,
            QKnx::NetIp::CemiServer::ReturnCode code,
            const QKnxByteArray &data);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request);

        static QKnxDeviceManagementFrame createConfirmation(
            const QKnxDeviceManagementFrame &request,
            QKnx::NetIp::CemiServer::ReturnCode code,
            const QKnxByteArray &data);
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
