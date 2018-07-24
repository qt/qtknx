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

#include "qknxnetipsecuredservicefamiliesdib.h"

#include <algorithm>

QT_BEGIN_NAMESPACE

/*!
    \inmodule QtKnx
    \class QKnxSecuredServiceInfo

    \ingroup qtknx-netip

    \brief The QKnxSecuredServiceInfo class contains the supported service
    family and the corresponding required security version.

    The service family is the high octet of the service type ID and the
    required security version is an integer representing the security version.

    \sa QKnxNetIpSecuredServiceFamiliesDibProxy, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \variable QKnxSecuredServiceInfo::ServiceFamily

    The service family is the high octet of the service type ID.

    \sa QKnx::NetIp::ServiceType, QKnx::NetIp::ServiceFamily
*/

/*!
    \variable QKnxSecuredServiceInfo::RequiredSecurityVersion

    The required security version is an integer representing the security version.
*/

/*!
    \class QKnxNetIpSecuredServiceFamiliesDibProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSecuredServiceFamiliesDibProxy class provides the
    means to introspect the supported service families and required security
    version inside the generic \l QKnxNetIpDib class and to create a KNXnet/IP
    device information block (DIB) structure based on the information.

    A KNXnet/IP secured service families DIB structure contains a set of
    supported services and their corresponding required security version.
    The service family is the high octet of the service type ID and the
    required security version is an integer representing the security version.

    KNXnet/IP service families without security requirements will not be
    included in the secured service families DIB.

    \note When using QKnxNetIpSecuredServiceFamiliesDibProxy, care must be
    taken to ensure that the referenced KNXnet/IP DIB structure outlives the
    proxy on all code paths, lest the proxy ends up referencing deleted data.

    Reading the secured service families can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpSecuredServiceFamiliesDibProxy proxy(dib);
        if (!proxy.isValid())
            return;

        auto infos = proxy.serviceInfos();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \internal
    \fn QKnxNetIpSecuredServiceFamiliesDibProxy::QKnxNetIpSecuredServiceFamiliesDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpSecuredServiceFamiliesDibProxy::~QKnxNetIpSecuredServiceFamiliesDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpSecuredServiceFamiliesDibProxy::QKnxNetIpSecuredServiceFamiliesDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified KNXnet/IP DIB structure
    \a dib to read the supported service families and security versions.
*/
QKnxNetIpSecuredServiceFamiliesDibProxy::QKnxNetIpSecuredServiceFamiliesDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpSecuredServiceFamiliesDibProxy::isValid() const
{
    return m_dib.isValid() && (m_dib.size() % 2 == 0) // must be even sized
        && m_dib.code() == QKnxNetIp::DescriptionType::SecuredServices;
}

/*!
    Returns the description type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpSecuredServiceFamiliesDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns a vector of the QKnxSecuredServiceInfo carried by this KNXnet/IP
    DIB structure if the object that was passed during construction was valid;
    otherwise returns an empty vector.
*/
QVector<QKnxSecuredServiceInfo> QKnxNetIpSecuredServiceFamiliesDibProxy::serviceInfos() const
{
    QVector<QKnxSecuredServiceInfo> infos;
    if (!isValid())
        return infos;

    const auto &data = m_dib.constData();
    for (quint16 i = 0 ; i < m_dib.dataSize() ; i += 2)
        infos.append({ QKnxNetIp::ServiceFamily(data.value(i)), data.value(i + 1) });
    return infos;
}

/*!
    Returns a builder object to create a KNXnet/IP secured service families DIB
    structure.
*/
QKnxNetIpSecuredServiceFamiliesDibProxy::Builder QKnxNetIpSecuredServiceFamiliesDibProxy::builder()
{
    return QKnxNetIpSecuredServiceFamiliesDibProxy::Builder();
}


/*!
    \class QKnxNetIpSecuredServiceFamiliesDibProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSecuredServiceFamiliesDibProxy

    \brief The QKnxNetIpSecuredServiceFamiliesDibProxy::Builder class creates a
    KNXnet/IP secured service families DIB structure.

    A KNXnet/IP secured service families DIB structure contains a set of
    supported services and their corresponding versions.

    The common way to create such a DIB structure is:
    \code
        auto dib = QKnxNetIpSecuredServiceFamiliesDibProxy::builder()
            .setServiceInfos({
                { QKnxNetIp::ServiceFamily::Core, 0x01 },
                { QKnxNetIp::ServiceFamily::DeviceManagement, 0x01 },
                { QKnxNetIp::ServiceFamily::IpTunneling, 0x01 }
            })
            .create();
    \endcode
*/

/*!
    Sets the supported service families and versions of the KNXnet/IP DIB
    structure to \a infos and returns a reference to the builder.
*/
QKnxNetIpSecuredServiceFamiliesDibProxy::Builder &QKnxNetIpSecuredServiceFamiliesDibProxy::
    Builder::setServiceInfos(const QVector<QKnxSecuredServiceInfo> &infos)
{
    m_infos = infos;

    std::sort(m_infos.begin(), m_infos.end(),
        [](const QKnxSecuredServiceInfo &a, const QKnxSecuredServiceInfo &b) {
            if (a.ServiceFamily < b.ServiceFamily)
                return true;
            if (a.ServiceFamily == b.ServiceFamily) {
                if (a.RequiredSecurityVersion < b.RequiredSecurityVersion)
                    return true;
            }
            return false;
    });
    m_infos.erase(std::unique(m_infos.begin(), m_infos.end(),
        [](const QKnxSecuredServiceInfo &a, const QKnxSecuredServiceInfo &b) {
        return a.ServiceFamily == b.ServiceFamily
            && a.RequiredSecurityVersion == b.RequiredSecurityVersion;
    }), m_infos.end());

    return *this;
}

/*!
    Creates and returns a QKnxNetIpDib.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpSecuredServiceFamiliesDibProxy::Builder::create() const
{
    QKnxByteArray bytes;
    for (const auto &info : qAsConst(m_infos))
        bytes += { quint8(info.ServiceFamily), info.RequiredSecurityVersion };
    return { QKnxNetIp::DescriptionType::SecuredServices, bytes };
}

QT_END_NAMESPACE
