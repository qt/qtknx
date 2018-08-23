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

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpCriProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpCriProxy class provides the means to read the connection
    request information (CRI) from the generic \l QKnxNetIpCri class and to
    create a KNXnet/IP CRI structure based on the information.

    A KNXnet/IP CRI structure contains additional information needed for
    different types of communication channels to fulfill a connection request.

    The structure may contain host protocol dependent and independent data,
    but the current KNX specification foresees additional data only in the
    case of tunneling.

    \note When using QKnxNetIpCriProxy, care must be taken to ensure that the
    referenced KNXnet/IP CRI structure outlives the proxy on all code paths, lest
    the proxy ends up referencing deleted data.

    Reading the connection type and tunneling layer can be achieved like this:
    \code
        auto cri = QKnxNetIpCri::fromBytes(...);

        QKnxNetIpCriProxy proxy(cri);
        if (!proxy.isValid())
            return;

        if (proxy.connectionType() != QKnxNetIp::ConnectionType::Tunnel)
            return;
        auto layer = proxy.tunnelLayer(); // read the requested tunneling layer
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpCriProxy::QKnxNetIpCriProxy()
    \internal
*/

/*!
    \fn QKnxNetIpCriProxy::~QKnxNetIpCriProxy()
    \internal
*/

/*!
    \fn QKnxNetIpCriProxy::QKnxNetIpCriProxy(const QKnxNetIpCri &&)
    \internal
*/

/*!
    Constructs a proxy object with the specified a KNXnet/IP structure
    \a cri to read the connection request information (CRI).
*/
QKnxNetIpCriProxy::QKnxNetIpCriProxy(const QKnxNetIpCri &cri)
    : m_cri(cri)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP CRI structure; otherwise returns \c false.
*/
bool QKnxNetIpCriProxy::isValid() const
{
    switch (m_cri.code()) {
        case QKnxNetIp::ConnectionType::Tunnel: {
            auto tmp = m_cri.constData().value(0);
            return m_cri.isValid() && QKnxNetIp::isTunnelLayer(QKnxNetIp::TunnelLayer(tmp))
                && (m_cri.size() == 4 || m_cri.size() == 6);
        }
        case QKnxNetIp::ConnectionType::DeviceManagement:
        case QKnxNetIp::ConnectionType::RemoteLogging:
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
        case QKnxNetIp::ConnectionType::ObjectServer:
            return m_cri.isValid() && m_cri.size() == 2;
        case QKnxNetIp::ConnectionType::Unknown:
            break;
    }
    return false;
}

/*!
    \since 5.12

    Returns \c true if this is an extended connection request information (CRI)
    frame; otherwise returns \c false.

    \note No validation checks are performed on the given KNXnet/IP frame
    that was passed during construction.

    \sa isValid()
*/
bool QKnxNetIpCriProxy::isExtended() const
{
    return (m_cri.code() == QKnxNetIp::ConnectionType::Tunnel) && (m_cri.size() == 6);
}

/*!
    Returns the connection type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::ConnectionType QKnxNetIpCriProxy::connectionType() const
{
    if (isValid())
        return m_cri.code();
    return QKnxNetIp::ConnectionType::Unknown;
}

/*!
    Returns the tunneling layer of this KNXnet/IP structure if the object that
    was passed during construction was valid and the connection type
    is \l QKnx::NetIp::Tunnel, otherwise returns \l QKnx::NetIp::Unknown.

    \sa additionalData(), individualAddress()
*/
QKnxNetIp::TunnelLayer QKnxNetIpCriProxy::tunnelLayer() const
{
    if (isValid())
        return QKnxNetIp::TunnelLayer(m_cri.constData().value(0));
    return QKnxNetIp::TunnelLayer::Unknown;
}

/*!
    \since 5.12

    Returns the individual address of the extended connection request information
    structure if the object that was passed during construction was valid and an
    extended structure; otherwise returns \l QKnx::NetIp::Unknown.

    \sa isExtended(), tunnelLayer(), additionalData()
*/
QKnxAddress QKnxNetIpCriProxy::individualAddress() const
{
    if (isExtended() && isValid())
        return { QKnxAddress::Type::Individual, m_cri.constData().mid(2, 2) };
    return {};
}

/*!
    Returns the additional data of this KNXnet/IP structure.

    The current KNX specification foresees additional data only in the case of
    tunneling.

    \sa tunnelLayer(), individualAddress()
*/
QKnxByteArray QKnxNetIpCriProxy::additionalData() const
{
    return m_cri.data();
}

/*!
    Returns a builder object to create a KNXnet/IP connection request
    information structure.
*/
QKnxNetIpCriProxy::Builder QKnxNetIpCriProxy::builder()
{
    return QKnxNetIpCriProxy::Builder();
}


/*!
    \class QKnxNetIpCriProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpCriProxy

    \brief The QKnxNetIpCriProxy::Builder class creates a KNXnet/IP connection
    request information structure (CRI).

    The connection request information structure contains additional
    information needed for different types of communication channels to
    fulfill a connection request.

    The common way to create such a CRI structure is:
    \code
        auto cri = QKnxNetIpCriProxy::builder()
            .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
            .setTunnelLayer(QKnxNetIp::TunnelLayer::Link)
            .create();
    \endcode
*/

/*!
    Sets the connection type to \a type and returns a reference to the builder.

    Does nothing if \a type is not a \l QKnx::NetIp::ConnectionType.
*/
QKnxNetIpCriProxy::Builder &
    QKnxNetIpCriProxy::Builder::setConnectionType(QKnxNetIp::ConnectionType type)
{
    if (QKnxNetIp::isStructType(type))
        m_cType = type;
    return *this;
}

/*!
    Sets the additional data to the requested KNX tunneling layer \a layer and
    returns a reference to the builder.

    Does nothing if \a layer is not a \l QKnx::NetIp::TunnelLayer value.

    \sa setAdditionalData(), setIndividualAddress()
*/
QKnxNetIpCriProxy::Builder &
    QKnxNetIpCriProxy::Builder::setTunnelLayer(QKnxNetIp::TunnelLayer layer)
{
    if (QKnxNetIp::isTunnelLayer(layer))
        m_additionalData.replace(0, 2, { quint8(layer), 0x00 /* reserved byte */ });
    return *this;
}

/*!
    \since 5.12

    Sets the individual address of the extended connection request information
    to \a address and returns a reference to the builder.

    Does nothing if \a address is not of type
    \l {QKnxAddress::Type}{QKnxAddress::Type::Individual}.

    \note The current KNX specification foresees setting an individual address
    only in the case of extended tunneling connection requests.

    \sa setTunnelLayer(), setAdditionalData()
*/
QKnxNetIpCriProxy::Builder &
    QKnxNetIpCriProxy::Builder::setIndividualAddress(const QKnxAddress &address)
{
    if (address.type() == QKnxAddress::Type::Individual) {
        m_additionalData.resize(2);
        m_additionalData.append(address.bytes());
    }
    return *this;
}

/*!
    Sets the additional data to \a additionalData and returns a reference to
    the builder.

    The current KNX specification foresees additional data only in the case of
    tunneling.

    The common way to use the function is:
    \code
        QKnxAddress ia { QKnxAddress::Type::Individual, 2013 };
        auto cri = QKnxNetIpCriProxy::builder()
            .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
            .setAdditionalData(QKnxByteArray {
                   quint8(QKnxNetIp::TunnelLayer::Link), // tunnel layer
                   0x00                                  // reserved byte
                } + ia.bytes() // address for extended connection request
            ).create();
    \endcode

    The above code is equivalent to the more expressive one shown here:
    \code
        auto cri = QKnxNetIpCriProxy::builder()
            .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
            .setTunnelLayer(QKnxNetIp::TunnelLayer::Link)
            .setIndividualAddress({ QKnxAddress::Type::Individual, 2013 })
            .create();
    \endcode

    \sa setTunnelLayer(), setIndividualAddress()
*/
QKnxNetIpCriProxy::Builder &
    QKnxNetIpCriProxy::Builder::setAdditionalData(const QKnxByteArray &additionalData)
{
    m_additionalData = additionalData;
    return *this;
}

/*!
    Creates and returns a \l QKnxNetIpCri structure.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpCri QKnxNetIpCriProxy::Builder::create() const
{
    return { m_cType, m_additionalData };
}

QT_END_NAMESPACE
