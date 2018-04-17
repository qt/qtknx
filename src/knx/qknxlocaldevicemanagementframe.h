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

#ifndef QKNXLOCALDEVICEMANAGEMENTFRAME_H
#define QKNXLOCALDEVICEMANAGEMENTFRAME_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxnamespace.h>

QT_BEGIN_NAMESPACE

class QKnxLocalDeviceManagementFramePrivate;
class Q_KNX_EXPORT QKnxLocalDeviceManagementFrame final
{
    Q_GADGET
    friend class QKnxLocalDeviceManagementFrameFactory;

public:
    enum class MessageCode : quint8
    {
        Unknown = 0x00,

        PropertyReadRequest = 0xfc,
        PropertyReadConfirmation = 0xfb,

        PropertyWriteRequest = 0xf6,
        PropertyWriteConfirmation = 0xf5,

        PropertyInfoIndication = 0xf7,

        FunctionPropertyCommandRequest = 0xf8,
        FunctionPropertyStateReadRequest = 0xf9,
        FunctionPropertyCommandConfirmation = 0xfa,
        FunctionPropertyStateReadConfirmation = 0xfa,

        ResetRequest = 0xf1,
        ResetIndication = 0xf0,
    };
    Q_ENUM(MessageCode)

    QKnxLocalDeviceManagementFrame();
    ~QKnxLocalDeviceManagementFrame();

    explicit QKnxLocalDeviceManagementFrame(MessageCode code);
    QKnxLocalDeviceManagementFrame(QKnxLocalDeviceManagementFrame::MessageCode messageCode,
        const QKnxByteArray &serviceInfo);

    quint16 size() const;

    bool isValid() const;
    bool isNegativeConfirmation() const;

    MessageCode messageCode() const;
    void setMessageCode(MessageCode code);

    QKnxInterfaceObjectType objectType() const;
    void setObjectType(QKnxInterfaceObjectType type);

    quint8 objectInstance() const;
    void setObjectInstance(quint8 instance);

    QKnxInterfaceObjectProperty property() const;
    void setProperty(QKnxInterfaceObjectProperty pid);

    quint8 numberOfElements() const;
    void setNumberOfElements(quint8 count);

    quint16 startIndex() const;
    void setStartIndex(quint16 index);

    QKnxByteArray data() const;
    void setData(const QKnxByteArray &newData);

    QKnx::NetIp::CemiServer::Error error() const;
    void setError(QKnx::NetIp::CemiServer::Error error);

    QKnx::NetIp::CemiServer::ReturnCode returnCode() const;
    void setReturnCode(QKnx::NetIp::CemiServer::ReturnCode code);

    QKnxByteArray serviceInformation() const;
    void setServiceInformation(const QKnxByteArray &serviceInfo);

    QKnxByteArray bytes() const;
    static QKnxLocalDeviceManagementFrame fromBytes(const QKnxByteArray &data, quint16 index,
        quint16 size);

    QKnxLocalDeviceManagementFrame(const QKnxLocalDeviceManagementFrame &other);
    QKnxLocalDeviceManagementFrame &operator=(const QKnxLocalDeviceManagementFrame &other);

    QKnxLocalDeviceManagementFrame(QKnxLocalDeviceManagementFrame &&other) Q_DECL_NOTHROW;
    QKnxLocalDeviceManagementFrame &operator=(QKnxLocalDeviceManagementFrame &&other) Q_DECL_NOTHROW;

    void swap(QKnxLocalDeviceManagementFrame &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxLocalDeviceManagementFrame &other) const;
    bool operator!=(const QKnxLocalDeviceManagementFrame &other) const;

private:
    QKnxLocalDeviceManagementFrame(MessageCode code, QKnxInterfaceObjectType type,
            quint8 instance, QKnxInterfaceObjectProperty pid, quint8 noe, quint16 index,
            const QKnxByteArray &payload = {});

    QKnxLocalDeviceManagementFrame(MessageCode code, QKnxInterfaceObjectType type,
            quint8 instance, QKnxInterfaceObjectProperty pid, const QKnxByteArray &payload = {});

private:
    QSharedDataPointer<QKnxLocalDeviceManagementFramePrivate> d_ptr;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxLocalDeviceManagementFrame &frame);

QT_END_NAMESPACE

#endif
