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


// -- QKnxDeviceManagementFrame::Builder

QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setMessageCode(QKnxDeviceManagementFrame::MessageCode code)
{
    m_code = code;
    return *this;
}

QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setObjectType(QKnxInterfaceObjectType type)
{
    m_type = type;
    return *this;
}

QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setObjectInstance(quint8 instance)
{
    m_instance = instance;
    return *this;
}

QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_pid = pid;
    return *this;
}

QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setData(const QKnxByteArray &data)
{
    m_data = data;
    return *this;
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrame::Builder::createFrame() const
{
    return { m_code, m_type, m_instance, m_pid, m_data };
}


// -- QKnxDeviceManagementFrame::PropertyReadBuilder

using PFRB = QKnxDeviceManagementFrame::PropertyReadBuilder;
PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setNumberOfElements(quint8 noe)
{
    m_numberOfElements = noe;
    return *this;
}

PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setStartIndex(quint16 startIndex)
{
    m_startIndex= startIndex;
    return *this;
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrame::PropertyReadBuilder::createRequest() const
{
    m_builder.setMessageCode(MessageCode::PropertyReadRequest);
    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(m_numberOfElements);
    frame.setStartIndex(m_startIndex);
    return frame;
}

QKnxDeviceManagementFrame PFRB::createConfirmation(const QKnxByteArray &data,
                                                   const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::PropertyReadConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }

    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(request.isNull() ? m_numberOfElements : request.numberOfElements());
    frame.setStartIndex(request.isNull() ? m_startIndex : request.startIndex());
    frame.setData(data);
    return frame;
}

QKnxDeviceManagementFrame PFRB::createNegativeConfirmation(QKnxNetIpCemiServer::Error error,
                                                    const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::PropertyReadConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }

    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(0);
    frame.setStartIndex(request.isNull() ? m_startIndex : request.startIndex());
    frame.setError(error);
    return frame;
}


// -- QKnxDeviceManagementFrame::PropertyWriteBuilder

using PFWB = QKnxDeviceManagementFrame::PropertyWriteBuilder;
PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setNumberOfElements(quint8 noe)
{
    m_numberOfElements = noe;
    return *this;
}

PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setStartIndex(quint16 startIndex)
{
    m_startIndex= startIndex;
    return *this;
}

QKnxDeviceManagementFrame PFWB::createRequest(const QKnxByteArray &data) const
{
    m_builder.setMessageCode(MessageCode::PropertyWriteRequest);
    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(m_numberOfElements);
    frame.setStartIndex(m_startIndex);
    frame.setData(data);
    return frame;
}

QKnxDeviceManagementFrame PFWB::createConfirmation(const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::PropertyWriteConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }

    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(request.isNull() ? m_numberOfElements : request.numberOfElements());
    frame.setStartIndex(request.isNull() ? m_startIndex : request.startIndex());
    return frame;
}

QKnxDeviceManagementFrame PFWB::createNegativeConfirmation(QKnxNetIpCemiServer::Error error,
                                                    const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::PropertyWriteConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }

    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(0);
    frame.setStartIndex(request.isNull() ? m_startIndex : request.startIndex());
    frame.setError(error);
    return frame;
}


// -- QKnxDeviceManagementFrame::PropertyInfoBuilder

using PFIB = QKnxDeviceManagementFrame::PropertyInfoBuilder;
QKnxDeviceManagementFrame::PropertyInfoBuilder::PropertyInfoBuilder()
{
    m_builder.setMessageCode(MessageCode::PropertyInfoIndication);
}

PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setNumberOfElements(quint8 noe)
{
    m_numberOfElements = noe;
    return *this;
}

PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setStartIndex(quint16 startIndex)
{
    m_startIndex= startIndex;
    return *this;
}

QKnxDeviceManagementFrame PFIB::createIndication(const QKnxByteArray &data) const
{
    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(m_numberOfElements);
    frame.setStartIndex(m_startIndex);
    frame.setData(data);
    return frame;
}


// -- QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder

using FPCB = QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder;
FPCB &FPCB::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

FPCB &FPCB::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

FPCB &FPCB::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

QKnxDeviceManagementFrame FPCB::createRequest(const QKnxByteArray &data) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyCommandRequest);
    m_builder.setData(data);
    return m_builder.createFrame();
}

QKnxDeviceManagementFrame FPCB::createConfirmation(QKnxNetIpCemiServer::ReturnCode code,
        const QKnxByteArray &data, const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyCommandConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }

    auto frame = m_builder.createFrame();
    frame.setReturnCode(code);
    frame.setData(data);
    return frame;
}

QKnxDeviceManagementFrame
    FPCB::createNegativeConfirmation(const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyCommandConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }
    return m_builder.createFrame();
}


// -- QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder

using FPSRB = QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder;
FPSRB &FPSRB::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

FPSRB &FPSRB::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

FPSRB &FPSRB::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

QKnxDeviceManagementFrame FPSRB::createRequest(const QKnxByteArray &data) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyStateReadRequest);
    m_builder.setData(data);
    return m_builder.createFrame();
}

QKnxDeviceManagementFrame FPSRB::createConfirmation(QKnxNetIpCemiServer::ReturnCode code,
        const QKnxByteArray &data, const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyStateReadConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }

    auto frame = m_builder.createFrame();
    frame.setReturnCode(code);
    frame.setData(data);
    return frame;
}

QKnxDeviceManagementFrame
    FPSRB::createNegativeConfirmation(const QKnxDeviceManagementFrame &request) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyStateReadConfirmation);
    if (!request.isNull()) {
        m_builder.setObjectType(request.objectType());
        m_builder.setObjectInstance(request.objectInstance());
        m_builder.setProperty(request.property());
    }
    return m_builder.createFrame();
}


// -- QKnxDeviceManagementFrame::ResetBuilder

QKnxDeviceManagementFrame QKnxDeviceManagementFrame::ResetBuilder::createRequest()
{
    return QKnxDeviceManagementFrame { QKnxDeviceManagementFrame::MessageCode::ResetRequest };
}

QKnxDeviceManagementFrame QKnxDeviceManagementFrame::ResetBuilder::createIndication()
{
    return QKnxDeviceManagementFrame { QKnxDeviceManagementFrame::MessageCode::ResetIndication };
}

QT_END_NAMESPACE
