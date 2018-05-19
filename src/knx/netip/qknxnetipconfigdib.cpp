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

#include "qknxnetipconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpConfigDibProxy

    \inmodule QtKnx
    \brief The QKnxNetIpConfigDibProxy class provides the means to read the IP
    configuration from the generic \l QKnxNetIpDib class and to create a
    KNXnet/IP current IP configuration (DIB) structure based on the information.

    \note When using QKnxNetIpConfigDibProxy, care must be taken to ensure that
    the referenced KNXnet/IP DIB structure outlives the view on all code paths,
    lest the view ends up referencing deleted data.

    Reading the IP configuration can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpConfigDibProxy view(dib);
        if (!view.isValid())
            return;

        auto infos = view.ipAddress();
        auto subnetMask = view.subnetMask();
        auto gateway = view.defaultGateway();
        auto caps = view.capabilities();
        auto assignmentMethods = view.assignmentMethods();
    \endcode

    \sa builder()
*/

/*!
    \internal
    \fn QKnxNetIpConfigDibProxy::QKnxNetIpConfigDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpConfigDibProxy::~QKnxNetIpConfigDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpConfigDibProxy::QKnxNetIpConfigDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified a KNXnet/IP DIB structure
    \a dib to read the supported service families and versions.
*/
QKnxNetIpConfigDibProxy::QKnxNetIpConfigDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpConfigDibProxy::isValid() const
{
    return m_dib.isValid() && m_dib.size() == 16
        && m_dib.code() == QKnxNetIp::DescriptionType::IpConfiguration;
}

/*!
    Returns the description type of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpConfigDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns the fixed IP address of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns an invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpConfigDibProxy::ipAddress() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData());
    return {};
}

/*!
    Returns the subnet mask of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns an invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpConfigDibProxy::subnetMask() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData(), 4);
    return {};
}

/*!
    Returns the default gateway of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns an invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpConfigDibProxy::defaultGateway() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData(), 8);
    return {};
}

/*!
    Returns the IP capabilities supported by the KNXnet/IP device if the
    object that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::Capabilities QKnxNetIpConfigDibProxy::capabilities() const
{
    if (isValid())
        return QKnxNetIp::Capabilities(m_dib.constData().value(12));
    return QKnxNetIp::Capability::Unknown;
}

/*!
    Returns the enabled IP address assignment methods for setting the current
    IP address if the object that was passed during construction was valid;
    otherwise returns \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::AssignmentMethods QKnxNetIpConfigDibProxy::assignmentMethods() const
{
    if (isValid())
        return QKnxNetIp::AssignmentMethods(m_dib.constData().value(13));
    return QKnxNetIp::AssignmentMethod::Unknown;
}

/*!
    Returns a builder object to create a KNXnet/IP IP configuration DIB
    structure.
*/
QKnxNetIpConfigDibProxy::Builder QKnxNetIpConfigDibProxy::builder()
{
    return QKnxNetIpConfigDibProxy::Builder();
}


/*!
    \class QKnxNetIpConfigDibProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpConfigDibProxy

    \brief The QKnxNetIpConfigDibProxy::Builder class creates a KNXnet/IP
    IP configuration DIB structure.

    The common way to create such a DIB structure is:
    \code
        // setup the IP configuration values

        auto dib = QKnxNetIpConfigDibProxy::builder()
            .setIpAddress(address)
            .setSubnetMask(subnetMask)
            .setDefaultGateway(gateway)
            .setCapabilities(QKnxNetIp::Capabilities::Dhcp)
            .setAssignmentMethods(QKnxNetIp::AssignmentMethod::Dhcp)
            .create();
    \endcode
*/

/*!
    Sets the fixed IP address to \a ipAddress if the passed argument is a valid
    \l QHostAddress and returns a reference to the builder. It will be used if
    the manual address assignment method is enabled.
*/
QKnxNetIpConfigDibProxy::Builder &
    QKnxNetIpConfigDibProxy::Builder::setIpAddress(const QHostAddress &ipAddress)
{
    if (!ipAddress.isNull())
        m_ipAddress = ipAddress;
    return *this;
}

/*!
    Sets the subnet mask to \a subnetMask if the passed argument is a valid
    \l QHostAddress and returns a reference to the builder. It will be used
    if the manual address assignment method is enabled.
*/
QKnxNetIpConfigDibProxy::Builder &
    QKnxNetIpConfigDibProxy::Builder::setSubnetMask(const QHostAddress &subnetMask)
{
    if (!subnetMask.isNull())
        m_subnetMask = subnetMask;
    return *this;
}

/*!
    Sets the default gateway to \a gateway if the passed argument is
    a valid \l QHostAddress and returns a reference to the builder.
*/
QKnxNetIpConfigDibProxy::Builder &
    QKnxNetIpConfigDibProxy::Builder::setDefaultGateway(const QHostAddress &gateway)
{
    if (!gateway.isNull())
        m_gateway = gateway;
    return *this;
}

/*!
    Sets the IP capabilities supported by the KNXnet/IP device to
    \a capabilities if the passed argument is valid and returns a
    reference to the builder.
*/
QKnxNetIpConfigDibProxy::Builder &
    QKnxNetIpConfigDibProxy::Builder::setCapabilities(QKnxNetIp::Capabilities capabilities)
{
    if (capabilities >= 0x01 || capabilities <= 0x7)
        m_caps = capabilities;
    return *this;
}

/*!
    Sets the enabled IP address assignment methods for setting the current
    IP address to \a methods if the passed argument is valid and returns a
    reference to the builder.

    \note At least one method needs to be enabled.
*/
QKnxNetIpConfigDibProxy::Builder &
    QKnxNetIpConfigDibProxy::Builder::setAssignmentMethods(QKnxNetIp::AssignmentMethods methods)
{
    if (methods >= 0x01 || methods <= 0x15)
        m_methods = methods;
    return *this;
}

/*!
    Creates and returns a QKnxNetIpDib.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpConfigDibProxy::Builder::create() const
{
    if (m_ipAddress.isNull()
        || m_subnetMask.isNull()
        || m_gateway.isNull()
        || m_caps.testFlag(QKnxNetIp::Capability::Unknown)
        || m_methods.testFlag(QKnxNetIp::AssignmentMethod::Unknown)) {
            return { QKnxNetIp::DescriptionType::IpConfiguration };
    }

    return { QKnxNetIp::DescriptionType::IpConfiguration,
        QKnxUtils::HostAddress::bytes(m_ipAddress)
        + QKnxUtils::HostAddress::bytes(m_subnetMask)
        + QKnxUtils::HostAddress::bytes(m_gateway)
        + QKnxByteArray { quint8(m_caps), quint8(m_methods) }
    };
}

QT_END_NAMESPACE
