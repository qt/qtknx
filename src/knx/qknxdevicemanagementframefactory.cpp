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
    \class QKnxDeviceManagementFrame::Builder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::Builder class provides the means to
    create valid device management service frames.

    The typical arguments needed to build a \l QKnxDeviceManagementFrame are:
    \list
        \li The code describing the service to use,
            \l QKnxDeviceManagementFrame::MessageCode.
        \li The type of the interface object holding the property one wants to
            access, \l QKnxInterfaceObjectType.
        \li The instance of this interface object (because it is possible to
            have more than one realization of an interface object in a given
            device).
        \li The property of the interface object one wants to access,
            \l QKnxInterfaceObjectProperty.
        \li The \l {setData}{data} part which might consist of the number of
            elements one wants to read in this property and start index from
            where to read.
    \endlist

    More specialized versions of the builder are also provided and it is
    recommended to prefer them over the generic version:

    \list
        \li \l QKnxDeviceManagementFrame::PropertyReadBuilder
        \li \l QKnxDeviceManagementFrame::PropertyWriteBuilder
        \li \l QKnxDeviceManagementFrame::PropertyInfoBuilder
        \li \l QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder
        \li \l QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder
        \li \l QKnxDeviceManagementFrame::ResetBuilder
    \endlist

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/

/*!
    Sets the message code of this builder to \a code and returns a reference to
    the builder.
*/
QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setMessageCode(QKnxDeviceManagementFrame::MessageCode code)
{
    m_code = code;
    return *this;
}

/*!
    Sets the interface object type of this builder to \a type and returns a
    reference to the builder.
*/
QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setObjectType(QKnxInterfaceObjectType type)
{
    m_type = type;
    return *this;
}

/*!
    Sets the object instance of this builder to \a instance and returns
    a reference to the builder.
*/
QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setObjectInstance(quint8 instance)
{
    m_instance = instance;
    return *this;
}

/*!
    Sets the interface object property of this builder to \a pid and returns
    a reference to the builder.
*/
QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_pid = pid;
    return *this;
}

/*!
    Sets the data of this builder to \a data and returns a reference to the
    builder.
*/
QKnxDeviceManagementFrame::Builder &
    QKnxDeviceManagementFrame::Builder::setData(const QKnxByteArray &data)
{
    m_data = data;
    return *this;
}

/*!
    Creates and returns a device management frame.

    The service used depends on the frame's \l {setMessageCode}{message code}.
    All other fields, such as \l {setObjectType}{object type},
    \l {setObjectInstance}{object instance},
    \l {setProperty}{interface object property}, and depending on the service
    \l {setData}{data}, need to be set as well.

    The common way to create a negative confirmation frame is:
    \code
        auto negativeConfirmation = QKnxDeviceManagementFrame::builder()
            .setMessageCode(QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadConfirmation)
            .setObjectType(...)
            .setObjectInstance(...)
            .setProperty(...)
            .createFrame()
        deviceManagement.sendFrame(negativeConfirmation);
    \endcode

    The above code is equivalent to calling the
    \l FunctionPropertyStateReadBuilder::createNegativeConfirmation function.

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
QKnxDeviceManagementFrame QKnxDeviceManagementFrame::Builder::createFrame() const
{
    return { m_code, m_type, m_instance, m_pid, m_data };
}


/*!
    \class QKnxDeviceManagementFrame::PropertyReadBuilder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::PropertyReadBuilder class provides
    the means to create valid device management property read service frames.

    The property read service is used by a common external message interface
    (cEMI) client to send a property read request frame, \c {M_PropRead.req}.
    It must be followed by a property read confirmation frame,
    \c {M_PropRead.con}, sent by a cEMI server. The confirmation indicates
    whether the request was successful.

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/
using PFRB = QKnxDeviceManagementFrame::PropertyReadBuilder;

/*!
    Sets the interface object type of this builder to \a type and returns a
    reference to the builder.
*/
PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

/*!
    Sets the object instance of this builder to \a instance and returns
    a reference to the builder.
*/
PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

/*!
    Sets the interface object property of this builder to \a pid and returns
    a reference to the builder.
*/
PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

/*!
    Sets the number of elements of this builder to \a noe and returns a
    reference to the builder.
*/
PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setNumberOfElements(quint8 noe)
{
    m_numberOfElements = noe;
    return *this;
}

/*!
    Sets the start index of this builder to \a startIndex and returns a
    reference to the builder.
*/
PFRB &QKnxDeviceManagementFrame::PropertyReadBuilder::setStartIndex(quint16 startIndex)
{
    m_startIndex= startIndex;
    return *this;
}

/*!
    Creates a and returns a device management property read request frame.

    The common way to create such a request is:
    \code
        auto request = QKnxDeviceManagementFrame::propertyReadBuilder()
            .setObjectType(...)
            .setObjectInstance(...)
            .setProperty(...)
            .setNumberOfElements(...)
            .setStartIndex(...)
            .createRequest()
        deviceManagement.sendFrame(request);
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
QKnxDeviceManagementFrame QKnxDeviceManagementFrame::PropertyReadBuilder::createRequest() const
{
    m_builder.setMessageCode(MessageCode::PropertyReadRequest);
    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(m_numberOfElements);
    frame.setStartIndex(m_startIndex);
    return frame;
}

/*!
    Creates a and returns a device management property read confirmation frame
    with the data field set to \a data.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The common way to create such a request is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            // do some work
            auto data = ...

            auto confirmation = QKnxDeviceManagementFrame::propertyReadBuilder()
                .createConfirmation(data, request);
            deviceManagement.sendFrame(confirmation);
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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

/*!
    Creates and returns a negative device management property read confirmation
    frame.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The number of elements of a negative response is set to zero and the start
    index needs to be set to the value received with the request.

    The \a error field of a negative response contains information about the
    error as a \l QKnx::NetIp::CemiServer::Error value.

    The common way to create such a confirmation is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            // do some work
            bool nonExistingProperty = ...
            if (nonExistingProperty) {
                auto confirmation = QKnxDeviceManagementFrame::propertyReadBuilder()
                    .createNegativeConfirmation(QKnxNetIpCemiServer::Error::NonExistingProperty,
                                                request);
                deviceManagement.sendFrame(confirmation);
            }
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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


/*!
    \class QKnxDeviceManagementFrame::PropertyWriteBuilder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::PropertyWriteBuilder class provides
    the means to create valid device management property write service frames.

    The property write service is used by a common external message interface
    (cEMI) client to send a property write request frame, \c {M_PropWrite.req}.
    It must be followed by a property write confirmation frame,
    \c {M_PropWrite.con}. The confirmation indicates whether the request was
    successful.

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/
using PFWB = QKnxDeviceManagementFrame::PropertyWriteBuilder;

/*!
    Sets the interface object type of this builder to \a type and returns a
    reference to the builder.
*/
PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

/*!
    Sets the object instance of this builder to \a instance and returns
    a reference to the builder.
*/
PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

/*!
    Sets the interface object property of this builder to \a pid and returns
    a reference to the builder.
*/
PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

/*!
    Sets the number of elements of this builder to \a noe and returns a
    reference to the builder.
*/
PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setNumberOfElements(quint8 noe)
{
    m_numberOfElements = noe;
    return *this;
}

/*!
    Sets the start index of this builder to \a startIndex and returns a
    reference to the builder.
*/
PFWB &QKnxDeviceManagementFrame::PropertyWriteBuilder::setStartIndex(quint16 startIndex)
{
    m_startIndex= startIndex;
    return *this;
}

/*!
    Creates a and returns a device management property write request frame with
    the data field set to \a data.

    The common way to create such a request is:
    \code
        auto data = ...
        auto request = QKnxDeviceManagementFrame::propertyWriteBuilder()
            .setObjectType(...)
            .setObjectInstance(...)
            .setProperty(...)
            .setNumberOfElements(...)
            .setStartIndex(...)
            .createRequest(data)
        deviceManagement.sendFrame(request);
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
QKnxDeviceManagementFrame PFWB::createRequest(const QKnxByteArray &data) const
{
    m_builder.setMessageCode(MessageCode::PropertyWriteRequest);
    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(m_numberOfElements);
    frame.setStartIndex(m_startIndex);
    frame.setData(data);
    return frame;
}

/*!
    Creates a and returns a device management property write confirmation frame.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The common way to create such a request is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            if (request.messageCode() != QKnxDeviceManagementFrame::MessageCode::PropertyWriteRequest)
                return;

            // execute the write operation

            auto confirmation = QKnxDeviceManagementFrame::propertyWriteBuilder()
                .createConfirmation(request);
            deviceManagement.sendFrame(confirmation);
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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

/*!
    Creates and returns a negative device management property write confirmation
    frame.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The number of elements of a negative response is set to zero and the start
    index needs to be set to the value received with the request.

    The \a error field of a negative response contains information about the
    error as a \l QKnx::NetIp::CemiServer::Error value.

    The common way to create such a confirmation is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            if (request.messageCode() != QKnxDeviceManagementFrame::MessageCode::PropertyWriteRequest)
                return;

            bool readOnly = ... // check if the property is read only
            if (readOnly) {
                auto confirmation = QKnxDeviceManagementFrame::propertyWriteBuilder()
                    .createNegativeConfirmation(QKnxNetIpCemiServer::Error::ReadOnly, request);
                deviceManagement.sendFrame(confirmation);
            }
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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


/*!
    \class QKnxDeviceManagementFrame::PropertyInfoBuilder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::PropertyInfoBuilder class provides
    the means to create a valid device management property info indication frame.

    The property info service is used for local device management. It is an
    \e unconfirmed service, that is used by a common external message interface
    (cEMI) cEMI server to send notifications upon events, for example.

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/
using PFIB = QKnxDeviceManagementFrame::PropertyInfoBuilder;

/*!
    Creates a device management property info builder object.
*/
QKnxDeviceManagementFrame::PropertyInfoBuilder::PropertyInfoBuilder()
{
    m_builder.setMessageCode(MessageCode::PropertyInfoIndication);
}

/*!
    Sets the interface object type of this builder to \a type and returns a
    reference to the builder.
*/
PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

/*!
    Sets the object instance of this builder to \a instance and returns
    a reference to the builder.
*/
PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

/*!
    Sets the interface object property of this builder to \a pid and returns
    a reference to the builder.
*/
PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

/*!
    Sets the number of elements of this builder to \a noe and returns a
    reference to the builder.
*/
PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setNumberOfElements(quint8 noe)
{
    m_numberOfElements = noe;
    return *this;
}

/*!
    Sets the start index of this builder to \a startIndex and returns a
    reference to the builder.
*/
PFIB &QKnxDeviceManagementFrame::PropertyInfoBuilder::setStartIndex(quint16 startIndex)
{
    m_startIndex= startIndex;
    return *this;
}

/*!
    Creates and returns a device management property info indication frame with
    the data field set to \a data.

    The common way to create such an indication is:
    \code
        auto indication = QKnxDeviceManagementFrame::propertyInfoBuilder()
            .setObjectType(...)
            .setObjectInstance(...)
            .setProperty(...)
            .setNumberOfElements(...)
            .setStartIndex(...)
            .createIndication(...)
        deviceManagement.sendFrame(indication);
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
QKnxDeviceManagementFrame PFIB::createIndication(const QKnxByteArray &data) const
{
    auto frame = m_builder.createFrame();
    frame.setNumberOfElements(m_numberOfElements);
    frame.setStartIndex(m_startIndex);
    frame.setData(data);
    return frame;
}


/*!
    \class QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder class
    provides the means to create valid device management function property
    command service frames.

    The function property command service is used by a common external message
    interface (cEMI) client to send a function property command request frame,
    \c {M_FuncPropCommand.req}. It must be followed by a function property
    command confirmation frame, \c {M_FuncPropCommand.con}, sent by a cEMI
    server. The confirmation indicates whether the request was successful.

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/
using FPCB = QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder;

/*!
    Sets the interface object type of this builder to \a type and returns a
    reference to the builder.
*/
FPCB &FPCB::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

/*!
    Sets the object instance of this builder to \a instance and returns
    a reference to the builder.
*/
FPCB &FPCB::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

/*!
    Sets the interface object property of this builder to \a pid and returns
    a reference to the builder.
*/
FPCB &FPCB::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

/*!
    Creates and returns a device management function property command request
    frame with the data field set to \a data.

    The common way to create such a request is:
    \code
        auto request = QKnxDeviceManagementFrame::functionPropertyCommandBuilder()
            .setObjectType(...)
            .setObjectInstance(...)
            .setProperty(...)
            .createRequest(...);
        deviceManagement.sendFrame(request);
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
QKnxDeviceManagementFrame FPCB::createRequest(const QKnxByteArray &data) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyCommandRequest);
    m_builder.setData(data);
    return m_builder.createFrame();
}

/*!
    Creates and returns a device management function property command
    confirmation frame with the return code set to \a code and the data field
    to \a data.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The common way to create such a confirmation is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            // do some work
            auto data = ...

            auto confirmation = QKnxDeviceManagementFrame::functionPropertyCommandBuilder()
                .createConfirmation(QKnxNetIpCemiServer::ReturnCode::NoError, data, request);
            deviceManagement.sendFrame(confirmation);
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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

/*!
    Creates and returns a negative device management function property command
    confirmation frame.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The common way to create such a confirmation is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            // do some work
            bool error = ...
            if (error) {
                auto confirmation = QKnxDeviceManagementFrame::functionPropertyCommandBuilder()
                    .createNegativeConfirmation(request);
                deviceManagement.sendFrame(confirmation);
            }
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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


/*!
    \class QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder
    class provides the means to create valid device management function
    property state read service.

    The function property state read service is used by a common external
    message interface (cEMI) client to send a function property state read
    request frame, \c {M_FuncPropStateRead.req}.

    It must be followed by a function property state read confirmation frame,
    \c {M_FuncPropStateRead.con}, sent by a cEMI server. The confirmation
    indicates whether the request was successful.

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/
using FPSRB = QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder;

/*!
    Sets the interface object type of this builder to \a type and returns a
    reference to the builder.
*/
FPSRB &FPSRB::setObjectType(QKnxInterfaceObjectType type)
{
    m_builder.setObjectType(type);
    return *this;
}

/*!
    Sets the object instance of this builder to \a instance and returns
    a reference to the builder.
*/
FPSRB &FPSRB::setObjectInstance(quint8 instance)
{
    m_builder.setObjectInstance(instance);
    return *this;
}

/*!
    Sets the interface object property of this builder to \a pid and returns
    a reference to the builder.
*/
FPSRB &FPSRB::setProperty(QKnxInterfaceObjectProperty pid)
{
    m_builder.setProperty(pid);
    return *this;
}

/*!
    Creates and returns a device management function property state request
    frame with the data field set to \a data.

    The common way to create such a request is:
    \code
        auto request = QKnxDeviceManagementFrame::functionPropertyStateReadBuilder()
            .setObjectType(...)
            .setObjectInstance(...)
            .setProperty(...)
            .createRequest(...);
        deviceManagement.sendFrame(request);
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
QKnxDeviceManagementFrame FPSRB::createRequest(const QKnxByteArray &data) const
{
    m_builder.setMessageCode(MessageCode::FunctionPropertyStateReadRequest);
    m_builder.setData(data);
    return m_builder.createFrame();
}

/*!
    Creates and returns a device management function property state confirmation
    frame with the return code set to \a code and the data field to \a data.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The common way to create such a confirmation is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            // do some work
            auto data = ...

            auto confirmation = QKnxDeviceManagementFrame::functionPropertyStateReadBuilder()
                .createConfirmation(QKnxNetIpCemiServer::ReturnCode::NoError, data, request);
            deviceManagement.sendFrame(confirmation);
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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

/*!
    Creates and returns a negative device management function property state
    confirmation frame.

    The available default argument \a request can be used to fill some of the
    returned frame fields instead of using the setter functions provided.

    The common way to create such a confirmation is:
    \code
        void MyCemiServer::slotFrameReceived(const QKnxDeviceManagementFrame &request)
        {
            // do some work
            bool error = ...
            if (error) {
                auto confirmation = QKnxDeviceManagementFrame::functionPropertyStateReadBuilder()
                    .createNegativeConfirmation(request);
                deviceManagement.sendFrame(confirmation);
            }
        }
    \endcode

    \note The returned frame may be invalid depending on the values used
    during setup.

    \sa QKnxDeviceManagementFrame, QKnxDeviceManagementFrame::isValid()
*/
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


/*!
    \class QKnxDeviceManagementFrame::ResetBuilder

    \inmodule QtKnx
    \inheaderfile QKnxDeviceManagementFrameBuilder
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame::ResetBuilder class provides the means
    to build valid device management reset service frames.

    The reset service is used by a common external message interface (cEMI)
    client to send a reset request frame, \c {M_Reset.req}. It may be followed
    by a reset indication frame, \c {M_Reset.ind}.

    \sa QKnxDeviceManagementFrame::MessageCode, QKnxDeviceManagementFrame,
        {Qt KNX Device Management Classes}
*/

/*!
    Creates a reset request frame.
*/
QKnxDeviceManagementFrame QKnxDeviceManagementFrame::ResetBuilder::createRequest()
{
    return QKnxDeviceManagementFrame { QKnxDeviceManagementFrame::MessageCode::ResetRequest };
}

/*!
    Creates a reset indication frame.
*/
QKnxDeviceManagementFrame QKnxDeviceManagementFrame::ResetBuilder::createIndication()
{
    return QKnxDeviceManagementFrame { QKnxDeviceManagementFrame::MessageCode::ResetIndication };
}

QT_END_NAMESPACE
