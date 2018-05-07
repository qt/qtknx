/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxdevicemanagementframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxDeviceManagementFrame::Builder final
{
public:
    Builder() = default;
    ~Builder() = default;

    Builder &setMessageCode(QKnxDeviceManagementFrame::MessageCode code);
    Builder &setObjectType(QKnxInterfaceObjectType type);
    Builder &setObjectInstance(quint8 instance);
    Builder &setProperty(QKnxInterfaceObjectProperty pid);
    Builder &setData(const QKnxByteArray &data);

    QKnxDeviceManagementFrame createFrame() const;

private:
    QKnxDeviceManagementFrame::MessageCode m_code;
    QKnxInterfaceObjectType m_type { QKnxInterfaceObjectType::Invalid };
    quint8 m_instance;
    QKnxInterfaceObjectProperty m_pid { QKnxInterfaceObjectProperty::Invalid };
    QKnxByteArray m_data;
};

class Q_KNX_EXPORT QKnxDeviceManagementFrame::PropertyReadBuilder final
{
public:
    PropertyReadBuilder &setObjectType(QKnxInterfaceObjectType type);
    PropertyReadBuilder &setObjectInstance(quint8 instance);
    PropertyReadBuilder &setProperty(QKnxInterfaceObjectProperty pid);
    PropertyReadBuilder &setNumberOfElements(quint8 noe);
    PropertyReadBuilder &setStartIndex(quint16 startIndex);

    QKnxDeviceManagementFrame createRequest() const;
    QKnxDeviceManagementFrame createConfirmation(const QKnxByteArray &data,
        const QKnxDeviceManagementFrame &request = {}) const;
    QKnxDeviceManagementFrame createNegativeConfirmation(QKnxNetIpCemiServer::Error error,
        const QKnxDeviceManagementFrame &request = {}) const;

private:
    int m_numberOfElements;
    int m_startIndex;
    mutable QKnxDeviceManagementFrame::Builder m_builder;
};

class Q_KNX_EXPORT QKnxDeviceManagementFrame::PropertyWriteBuilder final
{
public:
    PropertyWriteBuilder &setObjectType(QKnxInterfaceObjectType type);
    PropertyWriteBuilder &setObjectInstance(quint8 instance);
    PropertyWriteBuilder &setProperty(QKnxInterfaceObjectProperty pid);
    PropertyWriteBuilder &setNumberOfElements(quint8 noe);
    PropertyWriteBuilder &setStartIndex(quint16 startIndex);

    QKnxDeviceManagementFrame createRequest(const QKnxByteArray &data) const;
    QKnxDeviceManagementFrame createConfirmation(
        const QKnxDeviceManagementFrame &request = {}) const;
    QKnxDeviceManagementFrame createNegativeConfirmation(QKnxNetIpCemiServer::Error error,
        const QKnxDeviceManagementFrame &request = {}) const;

private:
    int m_numberOfElements;
    int m_startIndex;
    mutable QKnxDeviceManagementFrame::Builder m_builder;
};

class Q_KNX_EXPORT QKnxDeviceManagementFrame::PropertyInfoBuilder final
{
public:
    PropertyInfoBuilder();

    PropertyInfoBuilder &setObjectType(QKnxInterfaceObjectType type);
    PropertyInfoBuilder &setObjectInstance(quint8 instance);
    PropertyInfoBuilder &setProperty(QKnxInterfaceObjectProperty pid);
    PropertyInfoBuilder &setNumberOfElements(quint8 noe);
    PropertyInfoBuilder &setStartIndex(quint16 startIndex);

    QKnxDeviceManagementFrame createIndication(const QKnxByteArray &data) const;

private:
    int m_numberOfElements;
    int m_startIndex;
    mutable QKnxDeviceManagementFrame::Builder m_builder;
};

class Q_KNX_EXPORT QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder final
{
public:
    FunctionPropertyCommandBuilder &setObjectType(QKnxInterfaceObjectType type);
    FunctionPropertyCommandBuilder &setObjectInstance(quint8 instance);
    FunctionPropertyCommandBuilder &setProperty(QKnxInterfaceObjectProperty pid);

    QKnxDeviceManagementFrame createRequest(const QKnxByteArray &data) const;
    QKnxDeviceManagementFrame createConfirmation(QKnxNetIpCemiServer::ReturnCode code,
        const QKnxByteArray &data, const QKnxDeviceManagementFrame &request = {}) const;
    QKnxDeviceManagementFrame createNegativeConfirmation(
        const QKnxDeviceManagementFrame &request = {}) const;

private:
    mutable QKnxDeviceManagementFrame::Builder m_builder;
};

class Q_KNX_EXPORT QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder final
{
public:
    FunctionPropertyStateReadBuilder &setObjectType(QKnxInterfaceObjectType type);
    FunctionPropertyStateReadBuilder &setObjectInstance(quint8 instance);
    FunctionPropertyStateReadBuilder &setProperty(QKnxInterfaceObjectProperty pid);

    QKnxDeviceManagementFrame createRequest(const QKnxByteArray &data) const;
    QKnxDeviceManagementFrame createConfirmation(QKnxNetIpCemiServer::ReturnCode code,
        const QKnxByteArray &data, const QKnxDeviceManagementFrame &request = {}) const;
    QKnxDeviceManagementFrame createNegativeConfirmation(
        const QKnxDeviceManagementFrame &request = {}) const;

private:
    mutable QKnxDeviceManagementFrame::Builder m_builder;
};

class Q_KNX_EXPORT QKnxDeviceManagementFrame::ResetBuilder final
{
public:
    QKnxDeviceManagementFrame createRequest();
    QKnxDeviceManagementFrame createIndication();
};

QT_END_NAMESPACE

#endif
