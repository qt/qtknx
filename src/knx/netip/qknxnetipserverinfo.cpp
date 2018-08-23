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
#include "qknxnetipextendeddevicedib.h"
#include "qknxnetipserverinfo.h"
#include "qknxnetipserverinfo_p.h"
#include "qknxnetiphpai.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpServerInfo

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpServerInfo class stores information about a KNXnet/IP
    server.

    A KNXnet/IP server information object contains the information provided by
    the server during its discovery (using for example
    \l QKnxNetIpServerDiscoveryAgent) that the client needs to establish a
    connection. This includes the device name and individual address of the
    server, a KNXnet/IP transport connection endpoint, and the services
    supported by the server.

    KNXnet/IP servers are typically connected to one KNX subnetwork and to an
    IP network. Therefore, they have one KNX individual address returned by
    \l individualAddress() and one IP address returned by \l controlEndpointAddress().

    A KNXnet/IP server can support the following service types: core, device
    management, tunneling, routing, remote logging and configuration, and
    object server. The supported services are returned by \l supportedServices()
    as a vector of \l QKnxServiceInfo objects or by \l services() as a
    \l QKnxNetIpDib object which can be accessed via \l QKnxNetIpServiceFamiliesDibProxy.

    Furthermore, as specified by the KNX application note AN185, the
    server info may contain additional tunneling and extend device hardware
    information. If the information is present, it is made accessible through
    the \l mediumStatus(), \l {maximumLocalApduLength()}, \l maskVersion(),
    and \l tunnelingSlotInfos() functions. It is also possible to introspect
    the raw structures via their corresponding proxy objects.

    Here is an example on how to use the extended device information proxy
    in a search that is started using the \l QKnxNetIpServerDiscoveryAgent:

    \code
        const auto servers = agent.discoveredServers();
        for (auto server : servers) {
            const auto dib = server.extendedHardware();
            QKnxNetIpExtendedDeviceDibProxy proxy(dib);

            if (!proxy.isValid())
                continue;

            qDebug() << "Medium status:" << proxy.mediumStatus()
                << "Maximum local APDU length:" << proxy.maximumLocalApduLength()
                << "Device mask:" << proxy.deviceDescriptorType0();
        }

    \endcode

    \sa QKnxAddress, QKnxNetIpHpai, QKnxNetIpHpaiProxy, QKnxNetIpDib,
    QKnxNetIpDeviceDibProxy, QKnxNetIpServiceFamiliesDibProxy

    \sa {Qt KNXnet/IP Connection Classes}
*/

/*!
    Creates a KNXnet/IP server information object.
*/
QKnxNetIpServerInfo::QKnxNetIpServerInfo()
    : d_ptr(new QKnxNetIpServerInfoPrivate)
{}

/*!
    Deletes a KNXnet/IP server information object.
*/
QKnxNetIpServerInfo::~QKnxNetIpServerInfo()
{}

/*!
    Returns the device name.
*/
QString QKnxNetIpServerInfo::deviceName() const
{
    return QString::fromUtf8(QKnxNetIpDeviceDibProxy(d_ptr->hardware).deviceName());
}

/*!
    Returns the individual address of the server.
*/
QKnxAddress QKnxNetIpServerInfo::individualAddress() const
{
    return QKnxNetIpDeviceDibProxy(d_ptr->hardware).individualAddress();
}

/*!
    Returns the port number of the control endpoint.
*/
quint16 QKnxNetIpServerInfo::controlEndpointPort() const
{
    return QKnxNetIpHpaiProxy(d_ptr->hpai).port();
}

/*!
    Returns the host address of the control endpoint.

*/
QHostAddress QKnxNetIpServerInfo::controlEndpointAddress() const
{
    return QKnxNetIpHpaiProxy(d_ptr->hpai).hostAddress();
}

/*!
    Returns the services supported by a KNXnet/IP server.
*/
QVector<QKnxServiceInfo> QKnxNetIpServerInfo::supportedServices() const
{
    return QKnxNetIpServiceFamiliesDibProxy(d_ptr->services).serviceInfos();
}

/*!
    \since 5.12

    Returns the available tunneling slots of the discovered KNXnet/IP server
    if it supports providing this kind of information; otherwise returns an
    empty vector.
*/
QVector<QKnxNetIpTunnelingSlotInfo> QKnxNetIpServerInfo::tunnelingSlotInfos() const
{
    QKnxNetIpTunnelingInfoDibProxy proxy(d_ptr->tunnelingInfo);
    return QVector<QKnxNetIpTunnelingSlotInfo> { proxy.tunnelingSlotInfo() }
        + proxy.optionalSlotInfos();
}

/*!
    \since 5.12

    Returns the medium status of the discovered KNXnet/IP server if it supports
    extended device information; otherwise returns \l QKnx::Unknown.
*/
QKnx::MediumStatus QKnxNetIpServerInfo::mediumStatus() const
{
    QKnxNetIpExtendedDeviceDibProxy proxy(d_ptr->extendedHardware);
    if (!proxy.isValid())
        return QKnx::MediumStatus::Unknown;
    return proxy.mediumStatus();
}

/*!
    \since 5.12

    Returns the maximum local application protocol data unit (APDU) length of
    the discovered KNXnet/IP server if it supports extended device information;
    otherwise returns a \l {default-constructed value} which can be \c 0.
*/
quint16 QKnxNetIpServerInfo::maximumLocalApduLength() const
{
    return QKnxNetIpExtendedDeviceDibProxy(d_ptr->extendedHardware).maximumLocalApduLength();
}

/*!
    \since 5.12

    Returns the mask version (device descriptor 0) of the discovered
    KNXnet/IP server if it supports extended device information; otherwise
    returns a \l {default-constructed value} which can be \c 0.
*/
quint16 QKnxNetIpServerInfo::maskVersion() const
{
    return QKnxNetIpExtendedDeviceDibProxy(d_ptr->extendedHardware).deviceDescriptorType0();
}

/*!
    Returns a KNXnet/IP transport connection endpoint.

    \sa QKnxNetIpHpaiProxy
*/
QKnxNetIpHpai QKnxNetIpServerInfo::endpoint() const
{
    return d_ptr->hpai;
}

/*!
    Returns information about the KNXnet/IP server hardware.

    \sa QKnxNetIpDeviceDibProxy
*/
QKnxNetIpDib QKnxNetIpServerInfo::hardware() const
{
    return d_ptr->hardware;
}

/*!
    Returns the services available on a KNXnet/IP server.

    \sa QKnxNetIpServiceFamiliesDibProxy
*/
QKnxNetIpDib QKnxNetIpServerInfo::services() const
{
    return d_ptr->services;
}

/*!
    \since 5.12

    Returns tunneling information if available on a KNXnet/IP server.

    \sa QKnxNetIpTunnelingInfoDibProxy
*/
QKnxNetIpDib QKnxNetIpServerInfo::tunnelingInfo() const
{
    return d_ptr->tunnelingInfo;
}

/*!
    \since 5.12

    Returns extended hardware information about the KNXnet/IP server hardware.

    \sa QKnxNetIpExtendedDeviceDibProxy
*/
QKnxNetIpDib QKnxNetIpServerInfo::extendedHardware() const
{
    return d_ptr->extendedHardware;
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpServerInfo::QKnxNetIpServerInfo(const QKnxNetIpServerInfo &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this object.
*/
QKnxNetIpServerInfo &QKnxNetIpServerInfo::operator=(const QKnxNetIpServerInfo &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxNetIpServerInfo::QKnxNetIpServerInfo(QKnxNetIpServerInfo &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxNetIpServerInfo &QKnxNetIpServerInfo::operator=(QKnxNetIpServerInfo &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Returns \c true if \a other points to the same item as this server
    information object. Otherwise, returns \c false.
*/
bool QKnxNetIpServerInfo::operator==(const QKnxNetIpServerInfo &other) const
{
    return d_ptr == other.d_ptr || [&]() -> bool {
        return d_ptr->hpai == other.d_ptr->hpai
            && d_ptr->hardware == other.d_ptr->hardware
            && d_ptr->services == other.d_ptr->services
            && d_ptr->tunnelingInfo == other.d_ptr->tunnelingInfo
            && d_ptr->extendedHardware == other.d_ptr->extendedHardware;
    }();
}

/*!
    Returns \c true if \a other points to a different item than this server
    information object. Otherwise, returns \c false.
*/
bool QKnxNetIpServerInfo::operator!=(const QKnxNetIpServerInfo &other) const
{
    return !operator==(other);
}

/*!
    Swaps the server information object \a other with this object.
*/
void QKnxNetIpServerInfo::swap(QKnxNetIpServerInfo &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    \internal
*/
QKnxNetIpServerInfo::QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai, const QKnxNetIpDib &hardware,
        QKnxNetIpDib services) // ### Qt6: pass services as const reference
    : QKnxNetIpServerInfo()
{
    d_ptr->hpai = hpai;
    d_ptr->hardware = hardware;
    d_ptr->services = services;
}

/*!
    \internal
*/
QKnxNetIpServerInfo::QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai, const QKnxNetIpDib &hardware,
        const QKnxNetIpDib &services, const QKnxNetIpDib &tunneling, const QKnxNetIpDib &extHardware)
    : QKnxNetIpServerInfo()
{
    d_ptr->hpai = hpai;
    d_ptr->hardware = hardware;
    d_ptr->services = services;
    d_ptr->tunnelingInfo = tunneling;
    d_ptr->extendedHardware = extHardware;
}

/*!
    \internal
*/
QKnxNetIpServerInfo::QKnxNetIpServerInfo(QKnxNetIpServerInfoPrivate &dd)
    : d_ptr(new QKnxNetIpServerInfoPrivate(dd))
{}

QT_END_NAMESPACE
