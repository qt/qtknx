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

#include "qknxnetipdevicedib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceDibProxy

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceDibProxy class provides the means to read the
    device information of a KNXnet/IP device from the generic \l QKnxNetIpDib
    class and to create a KNXnet/IP device information block (DIB) structure.

    \note When using QKnxNetIpDeviceDibProxy, care must be taken to ensure that
    the referenced KNXnet/IP DIB structure outlives the view on all code paths,
    lest the view ends up referencing deleted data.

    Reading the device information can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpDeviceDibProxy view(dib);
        if (!view.isValid())
            return;

        auto mediumType = view.mediumType();
        auto programmingMode = view.deviceStatus();
        ...
        auto deviceName = view.deviceName();
    \endcode

    \sa builder()
*/

/*!
    \internal
    \fn QKnxNetIpDeviceDibProxy::QKnxNetIpDeviceDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpDeviceDibProxy::~QKnxNetIpDeviceDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpDeviceDibProxy::QKnxNetIpDeviceDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified a KNXnet/IP DIB structure
    \a dib to read the device information of a KNXnet/IP device.
*/
QKnxNetIpDeviceDibProxy::QKnxNetIpDeviceDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpDeviceDibProxy::isValid() const
{
    return m_dib.isValid() && m_dib.size() == 54
        && m_dib.code() == QKnxNetIp::DescriptionType::DeviceInfo;
}

/*!
    Returns the description type of this KNXnet/IP structure if the
    object that was passed during construction was valid; otherwise
    returns \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpDeviceDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns the medium type contained inside the KNXnet/IP DIB structure
    if the object that was passed during construction was valid; otherwise
    returns \l QKnx::Unknown.
*/
QKnx::MediumType QKnxNetIpDeviceDibProxy::mediumType() const
{
    if (isValid())
        return QKnx::MediumType(m_dib.constData().value(0));
    return QKnx::MediumType::Unknown;
}

/*!
    Returns the device status contained inside the KNXnet/IP DIB structure.

    \sa QKnx::NetIp::ProgrammingMode
*/
QKnxNetIp::ProgrammingMode QKnxNetIpDeviceDibProxy::deviceStatus() const
{
    return QKnxNetIp::ProgrammingMode(m_dib.constData().value(1));
}

/*!
    Returns the individual address contained inside the KNXnet/IP DIB
    structure if the object that was passed during construction was valid;
    otherwise returns a default constructed \l QKnxAddress.
*/
QKnxAddress QKnxNetIpDeviceDibProxy::individualAddress() const
{
    if (isValid())
        return { QKnxAddress::Type::Individual, m_dib.constData().mid(2, 2) };
    return {};
}

/*!
    Returns the KNX project installation ID contained inside the KNXnet/IP
    DIB structure.
*/
quint16 QKnxNetIpDeviceDibProxy::projectInstallationId() const
{
    return QKnxUtils::QUint16::fromBytes(m_dib.constData(), 4);
}

/*!
    Returns the device serial number contained inside the KNXnet/IP DIB
    structure if the object that was passed during construction was valid;
    otherwise returns an empty byte array.
*/
QKnxByteArray QKnxNetIpDeviceDibProxy::serialNumber() const
{
    if (isValid())
        return m_dib.constData().mid(6, 6);
    return {};
}

/*!
    Returns the multicast address contained inside the KNXnet/IP DIB structure
    if the object that was passed during construction was valid; otherwise
    returns a default constructed \l QHostAddress.
*/
QHostAddress QKnxNetIpDeviceDibProxy::multicastAddress() const
{
    if (isValid())
        return QKnxUtils::HostAddress::fromBytes(m_dib.constData(), 12);
    return {};
}

/*!
    Returns the MAC address contained inside the KNXnet/IP DIB structure if the
    object that was passed during construction was valid; otherwise returns an
    empty byte array.
*/
QKnxByteArray QKnxNetIpDeviceDibProxy::macAddress() const
{
    if (isValid())
        return m_dib.constData().mid(16, 6);
    return {};
}

/*!
    Returns the device name contained inside the KNXnet/IP DIB structure if the
    object that was passed during construction was valid; otherwise returns an
    empty byte array.
*/
QByteArray QKnxNetIpDeviceDibProxy::deviceName() const
{
    if (isValid()) {
        auto tmp = m_dib.constData().mid(22, 30);
        return QByteArray((const char *) tmp.constData());
    }
    return {};
}

/*!
    Returns a builder object to create a KNXnet/IP device information DIB
    structure.
*/
QKnxNetIpDeviceDibProxy::Builder QKnxNetIpDeviceDibProxy::builder()
{
    return QKnxNetIpDeviceDibProxy::Builder();
}


namespace QKnxPrivate
{
    /*!
        \internal
    */
    static bool isMediumType(QKnx::MediumType type)
    {
        switch (type) {
        case QKnx::MediumType::Unknown:
        case QKnx::MediumType::TP:
        case QKnx::MediumType::PL:
        case QKnx::MediumType::RF:
        case QKnx::MediumType::NetIP:
            return true;
        default:
            break;
        }
        return false;
    }
}


/*!
    \class QKnxNetIpDeviceDibProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpDeviceDibProxy

    \brief The QKnxNetIpDeviceDibProxy::Builder class creates a KNXnet/IP device
    information DIB structure.

    A KNXnet/IP device information DIB structure contains specific information
    about a KNXnet/IP device.

    The common way to create such a DIB structure is:
    \code
         // setup the device information

         auto dib = QKnxNetIpDeviceDibProxy::builder()
            .setMediumType(mediumType)
            .setDeviceStatus(programmingMode);
            .setIndividualAddress(individualAddress)
            .setProjectInstallationId(projectId)
            .setSerialNumber(serialNumber)
            .setMulticastAddress(multicastAddress)
            .setMacAddress(macAddress)
            .setDeviceName(deviceName)
            .create();
    \endcode
*/

/*!
    Sets the medium type to \a mediumType if the passed argument is a valid
    \l QKnx::MediumType and returns a reference to the builder.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setMediumType(QKnx::MediumType mediumType)
{
    if (QKnxPrivate::isMediumType(mediumType))
        m_mediumType = mediumType;
    return *this;
}

/*!
    Sets the device status to \a mode if the passed argument is a valid
    \l QKnx::NetIp::ProgrammingMode and returns a reference to the builder.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setDeviceStatus(QKnxNetIp::ProgrammingMode mode)
{
    if (QKnxNetIp::isProgrammingMode(mode))
        m_progMode = mode;
    return *this;
}

/*!
    Sets the individual address to \a address if the passed argument is a valid
    \l QKnxAddress and of type \l {QKnxAddress::Type} {QKnxAddress::Individual}
    and returns a reference to the builder.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setIndividualAddress(const QKnxAddress &address)
{
    if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
        m_address = address;
    return *this;
}

/*!
    Sets the KNX project installation ID to \a projectId and returns a
    reference to the builder.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setProjectInstallationId(quint16 projectId)
{
    m_projectId = projectId;
    return *this;
}

/*!
    Sets the serial number to \a serialNumber and returns a reference to the
    builder.

    \note The serial number must contain exactly 6 bytes.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setSerialNumber(const QKnxByteArray &serialNumber)
{
    if (serialNumber.size() == 6)
        m_serialNumber = serialNumber;
    return *this;
}

/*!
    Sets the multicast address to \a multicastAddress if the passed argument is
    a valid \l QHostAddress and returns a reference to the builder.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setMulticastAddress(const QHostAddress &multicastAddress)
{
    if (multicastAddress == QHostAddress::AnyIPv4 || !multicastAddress.isMulticast())
        m_multicastAddress = multicastAddress;
    return *this;
}

/*!
    Sets the MAC address to \a macAddress and returns a reference to the
    builder. By default it is set to the KNXnet/IP System Setup Multicast
    Address.

    \note The MAC address must contain exactly 6 bytes.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setMacAddress(const QKnxByteArray &macAddress)
{
    if (macAddress.size() == 6)
        m_macAddress = macAddress;
    return *this;
}

/*!
    Sets the device name to \a deviceName and returns a reference to the
    builder.

    \note The size of the device name may not contain more than 30 characters.
*/
QKnxNetIpDeviceDibProxy::Builder &
    QKnxNetIpDeviceDibProxy::Builder::setDeviceName(const QByteArray &deviceName)
{
    m_deviceName = deviceName;
    return *this;
}

/*!
    Creates and returns a generic QKnxNetIpDib containing the specific device
    information of a KNXnet/IP device.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpDeviceDibProxy::Builder::create() const
{
    if (!QKnxPrivate::isMediumType(m_mediumType)
        || !QKnxNetIp::isProgrammingMode(m_progMode)
        || !m_address.isValid()
        || m_serialNumber.size() != 6
        || m_multicastAddress.isNull()
        || m_macAddress.size() != 6) {
            return { QKnxNetIp::DescriptionType::DeviceInfo };
    }

    QKnxByteArray data;
    data.append(quint8(m_mediumType));
    data.append(quint8(m_progMode));
    data.append(m_address.bytes());
    data.append(QKnxUtils::QUint16::bytes(m_projectId));
    data.append(m_serialNumber);
    data.append(QKnxUtils::HostAddress::bytes(m_multicastAddress));
    data.append(m_macAddress);
    data.append(QKnxByteArray::fromByteArray(m_deviceName));

    if (data.size() < 52)
        data.append(QKnxByteArray(52 - data.size(), 0));
    data.resize(52); // size enforced by 7.5.4.2 Device information DIB

    return { QKnxNetIp::DescriptionType::DeviceInfo, data };
}

QT_END_NAMESPACE
