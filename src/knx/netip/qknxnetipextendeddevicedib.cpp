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

#include "qknxnetipextendeddevicedib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpExtendedDeviceDibProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpExtendedDeviceDibProxy class provides the means to
    read the extended device information of a KNXnet/IP device from the generic
    \l QKnxNetIpDib class and to create a KNXnet/IP extended device information
    block (DIB) structure.

    A KNXnet/IP extended device information DIB structure contains specific
    information about a KNXnet/IP device, such as the KNX medium status,
    \l {QKnxInterfaceObjectProperty::MaxLocalApduLength} {maximum local APDU}
    length and the \l {QKnxInterfaceObjectProperty::DeviceDescriptor} {device
    descriptor type} (mask version).

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDiscoveryAgent and \l QKnxNetIpServerInfo are provided.

    \note When using QKnxNetIpExtendedDeviceDibProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the proxy on
    all code paths, lest the proxy ends up referencing deleted data.

    Reading the extended device information can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpExtendedDeviceDibProxy proxy(dib);
        if (!proxy.isValid())
            return;

        auto mediumStatus = proxy.mediumStatus();
        auto adpuLength = proxy.maximumLocalApduLength();
        auto descriptorType = proxy.deviceDescriptorType0();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \internal
    \fn QKnxNetIpExtendedDeviceDibProxy::QKnxNetIpExtendedDeviceDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpExtendedDeviceDibProxy::~QKnxNetIpExtendedDeviceDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpExtendedDeviceDibProxy::QKnxNetIpExtendedDeviceDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified KNXnet/IP DIB structure
    \a dib to read the extended device information of a KNXnet/IP device.
*/
QKnxNetIpExtendedDeviceDibProxy::QKnxNetIpExtendedDeviceDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpExtendedDeviceDibProxy::isValid() const
{
    return m_dib.isValid() && m_dib.size() == 8
        && m_dib.code() == QKnxNetIp::DescriptionType::ExtendedDeviceInfo;
}

/*!
    Returns the description type of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpExtendedDeviceDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns the medium status contained inside the KNXnet/IP DIB structure
    if the object that was passed during construction was valid; otherwise
    returns \l QKnx::Unknown.
*/
QKnx::MediumStatus QKnxNetIpExtendedDeviceDibProxy::mediumStatus() const
{
    if (isValid())
        return QKnx::MediumStatus(m_dib.constData().value(0));
    return QKnx::MediumStatus::Unknown;
}

/*!
    Returns the \l {QKnxInterfaceObjectProperty::MaxLocalApduLength}
    {maximum local APDU} length carried by this KNXnet/IP DIB structure if
    the object that was passed during construction was valid; otherwise
    returns a \l {default-constructed value} which can be \c 0.
*/
quint16 QKnxNetIpExtendedDeviceDibProxy::maximumLocalApduLength() const
{
    if (isValid())
        return QKnxUtils::QUint16::fromBytes(m_dib.constData(), 2);
    return 0u;
}

/*!
    Returns the \l {QKnxInterfaceObjectProperty::DeviceDescriptor} {device
    descriptor type} (mask version) carried by this KNXnet/IP DIB structure
    if the object that was passed during construction was valid; otherwise
    returns a \l {default-constructed value} which can be \c 0.
*/
quint16 QKnxNetIpExtendedDeviceDibProxy::deviceDescriptorType0() const
{
    if (isValid())
        return QKnxUtils::QUint16::fromBytes(m_dib.constData(), 4);
    return 0u;
}

/*!
    Returns a builder object to create a KNXnet/IP device information DIB
    structure.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder QKnxNetIpExtendedDeviceDibProxy::builder()
{
    return QKnxNetIpExtendedDeviceDibProxy::Builder();
}


/*!
    \class QKnxNetIpExtendedDeviceDibProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpExtendedDeviceDibProxy

    \brief The QKnxNetIpExtendedDeviceDibProxy::Builder class creates a
    KNXnet/IP extended device information DIB structure.

    A KNXnet/IP extended device information DIB structure contains specific
    information about a KNXnet/IP device, such as the KNX medium status,
    \l {QKnxInterfaceObjectProperty::MaxLocalApduLength} {maximum local APDU}
    length and the \l {QKnxInterfaceObjectProperty::DeviceDescriptor} {device
    descriptor type} (mask version).

    The common way to create such a DIB structure is:
    \code
         // fetch the medium status

         auto dib = QKnxNetIpExtendedDeviceDibProxy::builder()
            .setMediumStatus(mediumStatus)
            .setMaximumLocalApduLength(0xffff);
            .setDeviceDescriptorType0(0x091a)
            .create();
    \endcode
*/

class QKnxNetIpExtendedDeviceDibBuilderPrivate : public QSharedData
{
public:
    QKnxNetIpExtendedDeviceDibBuilderPrivate() = default;
    ~QKnxNetIpExtendedDeviceDibBuilderPrivate() = default;

    qint16 m_mediumStatus { -1 };
    qint32 m_maxApduLength { -1 };
    qint32 m_descriptorType = { -1 };
};

/*!
    Creates a new empty extended device information DIB structure builder object.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder::Builder()
    : d_ptr(new QKnxNetIpExtendedDeviceDibBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder::~Builder() = default;

/*!
    Sets the medium status to \a status and returns a reference to the builder.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder &
    QKnxNetIpExtendedDeviceDibProxy::Builder::setMediumStatus(QKnx::MediumStatus status)
{
    d_ptr->m_mediumStatus = quint16(status);
    return *this;
}

/*!
    Sets the maximum local APDU length of the KNXnet/IP DIB structure to
    \a length and returns a reference to the builder.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder &
    QKnxNetIpExtendedDeviceDibProxy::Builder::setMaximumLocalApduLength(quint16 length)
{
    d_ptr->m_maxApduLength = length;
    return *this;
}

/*!
    Sets the descriptor type of the KNXnet/IP DIB structure to \a type and
    returns a reference to the builder.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder &
    QKnxNetIpExtendedDeviceDibProxy::Builder::setDeviceDescriptorType0(quint16 type)
{
    d_ptr->m_descriptorType = type;
    return *this;
}

/*!
    Creates and returns a generic KNXnet/IP extended device information DIB
    structure containing the specific device information of a KNXnet/IP device.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpExtendedDeviceDibProxy::Builder::create() const
{
    if (d_ptr->m_mediumStatus < 0 || d_ptr->m_maxApduLength < 0 || d_ptr->m_descriptorType < 0)
        return { QKnxNetIp::DescriptionType::ExtendedDeviceInfo };

    return { QKnxNetIp::DescriptionType::ExtendedDeviceInfo,
        QKnxByteArray { quint8(d_ptr->m_mediumStatus), 0x00 /* reserved field */ }
            + QKnxUtils::QUint16::bytes(quint16(d_ptr->m_maxApduLength))
            + QKnxUtils::QUint16::bytes(quint16(d_ptr->m_descriptorType))
    };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpExtendedDeviceDibProxy::Builder &
    QKnxNetIpExtendedDeviceDibProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
