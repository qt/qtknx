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

#include "qknxnetipknxaddressesdib.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpKnxAddressesDibProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpKnxAddressesDibProxy class provides the means to read
    all assigned individual addresses of a KNXnet/IP device from the generic
    \l QKnxNetIpDib class and to create a KNXnet/IP addresses device information
    block (DIB) structure.

    \note When using QKnxNetIpKnxAddressesDibProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the proxy on
    all code paths, lest the proxy ends up referencing deleted data.

    Reading the assigned addresses can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpKnxAddressesDibProxy proxy(dib);
        if (!proxy.isValid())
            return;

        auto addresses = proxy.individualAddresses();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \internal
    \fn QKnxNetIpKnxAddressesDibProxy::QKnxNetIpKnxAddressesDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpKnxAddressesDibProxy::~QKnxNetIpKnxAddressesDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpKnxAddressesDibProxy::QKnxNetIpKnxAddressesDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified KNXnet/IP DIB structure
    \a dib to read all assigned individual addresses of a KNXnet/IP device.
*/
QKnxNetIpKnxAddressesDibProxy::QKnxNetIpKnxAddressesDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpKnxAddressesDibProxy::isValid() const
{
    return m_dib.isValid() && (m_dib.size() % 2 == 0) // must be even sized
        && m_dib.size() >= 2 // and stores the header and at least one address
        && m_dib.code() == QKnxNetIp::DescriptionType::KnxAddresses;
}

/*!
    Returns the description type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpKnxAddressesDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns a list of the individual \l {QKnxAddress} {QKnxAddresses}
    carried by this KNXnet/IP DIB structure if the object that was passed
    during construction was valid; otherwise returns an empty list.
*/
QList<QKnxAddress> QKnxNetIpKnxAddressesDibProxy::individualAddresses() const
{
    QList<QKnxAddress> addresses;
    if (!isValid())
        return addresses;

    const auto &load = m_dib.constData();
    for (quint16 i = 0; i < load.size(); i += 2)
        addresses.append({ QKnxAddress::Type::Individual, load.mid(i, 2) });
    return addresses;
}

/*!
    Returns a builder object to create a KNXnet/IP addresses DIB structure.
*/
QKnxNetIpKnxAddressesDibProxy::Builder QKnxNetIpKnxAddressesDibProxy::builder()
{
    return QKnxNetIpKnxAddressesDibProxy::Builder();
}


/*!
    \class QKnxNetIpKnxAddressesDibProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpKnxAddressesDibProxy

    \brief The QKnxNetIpKnxAddressesDibProxy::Builder class creates a
    KNXnet/IP addresses DIB structure.

    A KNXnet/IP addresses DIB structure contains the assigned individual
    addresses of a KNXnet/IP device.

    The common way to create such a DIB structure is:
    \code
         auto dib = QKnxNetIpKnxAddressesDibProxy::builder()
            .setIndividualAddresses({
                { QKnxAddress::Type::Individual, 1976 },
                { QKnxAddress::Type::Individual, 1978 },
                { QKnxAddress::Type::Individual, 2002 },
                { QKnxAddress::Type::Individual, 2013 }
             }).create();
    \endcode
*/

/*!
    Sets the individual addresses of the KNXnet/IP DIB structure to \a addresses
    and returns a reference to the builder.
*/
QKnxNetIpKnxAddressesDibProxy::Builder &
    QKnxNetIpKnxAddressesDibProxy::Builder::setIndividualAddresses(const QList<QKnxAddress> &addresses)
{
    m_addresses = addresses;

    m_addresses.erase(std::remove_if(m_addresses.begin(), m_addresses.end(),
        [](const QKnxAddress &a) {
            return a.type() != QKnxAddress::Type::Individual;
    }), m_addresses.end());

    return *this;
}

/*!
    Creates and returns a generic QKnxNetIpDib containing the individual
    addresses of the KNXnet/IP device.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpKnxAddressesDibProxy::Builder::create() const
{
    return { QKnxNetIp::DescriptionType::KnxAddresses, [&]() -> QKnxByteArray {
        QKnxByteArray bytes;
        for (const auto &addr : m_addresses)
            bytes += addr.bytes();
        return bytes;
    }() };
}

QT_END_NAMESPACE
