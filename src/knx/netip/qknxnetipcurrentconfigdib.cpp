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

#include "qknxnetipcurrentconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpCurrentConfigDibProxy

    \inmodule QtKnx
    \brief The QKnxNetIpCurrentConfigDibProxy class provides the means to read
    the current IP configuration from the generic \l QKnxNetIpDib class and to
    create a KNXnet/IP current IP configuration (DIB) structure based on the
    information.

    A KNXnet/IP current IP configuration DIB structure contains a set of
    values currently used on the device for IP communication.

    \note When using QKnxNetIpCurrentConfigDibProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the view on all
    code paths, lest the view ends up referencing deleted data.

    Reading the current IP configuration can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpCurrentConfigDibProxy view(dib);
        if (!view.isValid())
            return;

        auto infos = view.ipAddress();
        auto subnetMask = view.subnetMask();
        auto gateway = view.defaultGateway();
        auto dhcp = view.dhcpOrBootP();
        auto assignmentMethod = view.assignmentMethod();
    \endcode

    \sa builder()
*/

/*!
    \internal
    \fn QKnxNetIpCurrentConfigDibProxy::QKnxNetIpCurrentConfigDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpCurrentConfigDibProxy::~QKnxNetIpCurrentConfigDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpCurrentConfigDibProxy::QKnxNetIpCurrentConfigDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified a KNXnet/IP DIB structure
    \a dib to read the current KNX device configuration.
*/
QKnxNetIpCurrentConfigDibProxy::QKnxNetIpCurrentConfigDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpCurrentConfigDibProxy::isValid() const
{
    return m_dib.isValid() && m_dib.size() == 20
        && m_dib.code() == QKnxNetIp::DescriptionType::CurrentIpConfiguration;
}

/*!
    Returns the description type of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns \l QKnxNetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpCurrentConfigDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns the currently used IP address of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise returns
    a invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpCurrentConfigDibProxy::ipAddress() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData());
    return {};
}

/*!
    Returns the currently used subnet mask of this KNXnet/IP structure if
    the object that was passed during construction was valid; otherwise returns
    a invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpCurrentConfigDibProxy::subnetMask() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData(), 4);
    return {};
}

/*!
    Returns the currently used default gateway of this KNXnet/IP structure
    if the object that was passed during construction was valid; otherwise
    returns a invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpCurrentConfigDibProxy::defaultGateway() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData(), 8);
    return {};
}

/*!
    Returns the IP address of the DHCP/BootP server the KNXnet/IP device
    last received its IP address from of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise returns
    a invalid \l QHostAddress.
*/
QHostAddress QKnxNetIpCurrentConfigDibProxy::dhcpOrBootP() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData(), 12);
    return {};
}

/*!
    Returns the currently employed IP address assignment method of this
    KNXnet/IP structure if the object that was passed during construction
    was valid; otherwise returns \l QKnxNetIp::Unknown.
*/
QKnxNetIp::AssignmentMethod QKnxNetIpCurrentConfigDibProxy::assignmentMethod() const
{
    if (isValid())
        return QKnxNetIp::AssignmentMethod(m_dib.constData().value(16));
    return QKnxNetIp::AssignmentMethod::Unknown;
}

/*!
    Returns a builder object to create a KNXnet/IP current IP configuration DIB
    structure.
*/
QKnxNetIpCurrentConfigDibProxy::Builder QKnxNetIpCurrentConfigDibProxy::builder()
{
    return QKnxNetIpCurrentConfigDibProxy::Builder();
}


/*!
    \class QKnxNetIpCurrentConfigDibProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpCurrentConfigDibProxy::Builder class creates a KNXnet/IP
    current IP configuration DIB structure.

    A KNXnet/IP current IP configuration DIB structure contains a set of
    values currently used on the device for IP communication.

    The common way to create such a DIB structure is:
    \code
        // setup the IP configuration values

        auto dib = QKnxNetIpCurrentConfigDibProxy::builder()
            .setIpAddress(address)
            .setSubnetMask(subnetMask)
            .setDefaultGateway(gateway)
            .setDhcpOrBootP(dhcp)
            .setAssignmentMethod(QKnxNetIp::AssignmentMethod::Dhcp)
            .create();
    \endcode
*/

/*!
    Sets the currently used IP address to \a ipAddress if the passed argument
    is a valid \l QHostAddress and returns a reference to the builder.
*/
QKnxNetIpCurrentConfigDibProxy::Builder &
    QKnxNetIpCurrentConfigDibProxy::Builder::setIpAddress(const QHostAddress &ipAddress)
{
    if (!ipAddress.isNull())
        m_ipAddress = ipAddress;
    return *this;
}

/*!
    Sets the currently used subnet mask to \a subnetMask if the passed argument
    is a valid \l QHostAddress and returns a reference to the builder.
*/
QKnxNetIpCurrentConfigDibProxy::Builder &
    QKnxNetIpCurrentConfigDibProxy::Builder::setSubnetMask(const QHostAddress &subnetMask)
{
    if (!subnetMask.isNull())
        m_subnetMask = subnetMask;
    return *this;
}

/*!
    Sets the currently used default gateway to \a gateway if the passed
    argument is a valid \l QHostAddress and returns a reference to the builder.
*/
QKnxNetIpCurrentConfigDibProxy::Builder &
    QKnxNetIpCurrentConfigDibProxy::Builder::setDefaultGateway(const QHostAddress &gateway)
{
    if (!gateway.isNull())
        m_gateway = gateway;
    return *this;
}

/*!
    Sets the IP address of the DHCP/BootP server the KNXnet/IP device last
    received its IP address from to \a dhcpBootP if the passed argument is
    a valid \l QHostAddress and returns a reference to the builder.
*/
QKnxNetIpCurrentConfigDibProxy::Builder &
    QKnxNetIpCurrentConfigDibProxy::Builder::setDhcpOrBootP(const QHostAddress &dhcpBootP)
{
    if (!dhcpBootP.isNull())
        m_dhcpBootP = dhcpBootP;
    return *this;
}

/*!
    Sets the currently employed IP address assignment method to \a method if
    the passed argument is valid and returns a reference to the builder.

    \note Only one method can be set at a time.
*/
QKnxNetIpCurrentConfigDibProxy::Builder &
    QKnxNetIpCurrentConfigDibProxy::Builder::setAssignmentMethod(QKnxNetIp::AssignmentMethod method)
{
    if (QKnxNetIp::isAssignmentMethod(method))
        m_method = method;
    return *this;
}

/*!
    Creates and returns a QKnxNetIpDib.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpCurrentConfigDibProxy::Builder::create() const
{
    if (m_ipAddress.isNull()
        || m_subnetMask.isNull()
        || m_gateway.isNull()
        || m_dhcpBootP.isNull()
        || m_method == QKnxNetIp::AssignmentMethod::Unknown) {
            return { QKnxNetIp::DescriptionType::CurrentIpConfiguration };
    }

    return { QKnxNetIp::DescriptionType::CurrentIpConfiguration,
        QKnxUtils::HostAddress::bytes(m_ipAddress)
        + QKnxUtils::HostAddress::bytes(m_subnetMask)
        + QKnxUtils::HostAddress::bytes(m_gateway)
        + QKnxUtils::HostAddress::bytes(m_dhcpBootP)
        + QKnxByteArray { quint8(m_method), 0x00 } // last byte is reserved
    };
}

QT_END_NAMESPACE
