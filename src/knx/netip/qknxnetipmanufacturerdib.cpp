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

#include "qknxnetipmanufacturerdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpManufacturerDibProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpManufacturerDibProxy class provides the means to read
    the manufacturer specific device information from the generic \l QKnxNetIpDib
    class and to create a KNXnet/IP manufacturer specific device information
    block (DIB) structure.

    \note When using QKnxNetIpManufacturerDibProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the view on all
    code paths, lest the view ends up referencing deleted data.

    Reading the manufacturer specific device information can be achieved like
    this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpManufacturerDibProxy view(dib);
        if (!view.isValid())
            return;

        auto id = view.manufacturerId();
        auto data = view.manufacturerData();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \internal
    \fn QKnxNetIpManufacturerDibProxy::QKnxNetIpManufacturerDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpManufacturerDibProxy::~QKnxNetIpManufacturerDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpManufacturerDibProxy::QKnxNetIpManufacturerDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified KNXnet/IP DIB structure
    \a dib to read a manufacturer specific device information block.
*/
QKnxNetIpManufacturerDibProxy::QKnxNetIpManufacturerDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpManufacturerDibProxy::isValid() const
{
    return m_dib.isValid() && m_dib.size() >= 4 && (m_dib.size() % 2 == 0) // must be even sized
        && m_dib.code() == QKnxNetIp::DescriptionType::ManufacturerData;
}

/*!
    Returns the description type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpManufacturerDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns the manufacturer ID of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns \c 0.
*/
quint16 QKnxNetIpManufacturerDibProxy::manufacturerId() const
{
    if (isValid())
        return QKnxUtils::QUint16::fromBytes(m_dib.constData());
    return 0;
}

/*!
    Returns the manufacturer data  if the object that was passed during
    construction was valid; otherwise returns an empty byte array.
*/
QKnxByteArray QKnxNetIpManufacturerDibProxy::manufacturerData() const
{
    if (isValid())
        return m_dib.constData().mid(2);
    return {};
}

/*!
    Returns a builder object to create a KNXnet/IP manufacturer DIB structure.
*/
QKnxNetIpManufacturerDibProxy::Builder QKnxNetIpManufacturerDibProxy::builder()
{
    return QKnxNetIpManufacturerDibProxy::Builder();
}


/*!
    \class QKnxNetIpManufacturerDibProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpManufacturerDibProxy

    \brief The QKnxNetIpManufacturerDibProxy::Builder class creates a
    KNXnet/IP manufacturer DIB structure.

    A KNXnet/IP manufacturer DIB structure contains the device manufacturer
    unique ID to clearly identify the device producer. It may also contain
    additional manufacturer specific data.

    The common way to create such a DIB structure is:
    \code
        quint16 dummyManufacturerId = 1000;
        constexpr quint8 data[] = "Some additional manufacturer data.";

        auto dib = QKnxNetIpManufacturerDibProxy::builder()
            .setManufacturerId(dummyManufacturerId)
            .setManufacturerData({ data, sizeof(data) })
            .create();
    \endcode
*/

/*!
    Sets the manufacturer ID of the KNXnet/IP DIB structure to \a manufacturerId
    and returns a reference to the builder.
*/
QKnxNetIpManufacturerDibProxy::Builder &
    QKnxNetIpManufacturerDibProxy::Builder::setManufacturerId(quint16 manufacturerId)
{
    m_manufacturerId = manufacturerId;
    return *this;
}

/*!
    Sets the manufacturer data of the KNXnet/IP DIB structure to
    \a manufacturerData and returns a reference to the builder.
*/
QKnxNetIpManufacturerDibProxy::Builder &
    QKnxNetIpManufacturerDibProxy::Builder::setManufacturerData(const QKnxByteArray &manufacturerData)
{
    m_manufacturerData = manufacturerData;
    if ((m_manufacturerData.size() % 2) == 1)
        m_manufacturerData.append(0x00); // The specification requires even sized structures.

    return *this;
}

/*!
    Creates and returns a QKnxNetIpDib.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpManufacturerDibProxy::Builder::create() const
{
    return { QKnxNetIp::DescriptionType::ManufacturerData,
        QKnxUtils::QUint16::bytes(m_manufacturerId) + m_manufacturerData };
}

QT_END_NAMESPACE
