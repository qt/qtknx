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

#include "qknxnetipservicefamiliesdib.h"

#include <algorithm>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpServiceFamiliesDibProxy

    \inmodule QtKnx
    \brief The QKnxNetIpServiceFamiliesDibProxy class provides the means to read
    the supported service families and versions from the generic \l QKnxNetIpDib
    class and to create a KNXnet/IP device information block (DIB) structure
    based on the information.

    A KNXnet/IP supported service families DIB structure contains a set of
    supported services and their corresponding versions. The service family is
    the high octet of the service type ID and the version of a service family
    refers to the version of the corresponding KNXnet/IP document. The service
    family version is an integer and it does not represent the manufacturer's
    implementation version.

    \note When using QKnxNetIpServiceFamiliesDibProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the view on all
    code paths, lest the view ends up referencing deleted data.

    Reading the supported service families can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpServiceFamiliesDibProxy view(dib);
        if (!view.isValid())
            return;

        auto infos = view.serviceInfos();
    \endcode

    \sa builder()
*/

/*!
    \internal
    \fn QKnxNetIpServiceFamiliesDibProxy::QKnxNetIpServiceFamiliesDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpServiceFamiliesDibProxy::~QKnxNetIpServiceFamiliesDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpServiceFamiliesDibProxy::QKnxNetIpServiceFamiliesDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified a KNXnet/IP DIB structure
    \a dib to read the supported service families and versions.
*/
QKnxNetIpServiceFamiliesDibProxy::QKnxNetIpServiceFamiliesDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.
*/
bool QKnxNetIpServiceFamiliesDibProxy::isValid() const
{
    return m_dib.isValid() && (m_dib.size() % 2 == 0) // must be even sized
        && m_dib.code() == QKnxNetIp::DescriptionType::SupportedServiceFamilies;
}

/*!
    Returns the description type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpServiceFamiliesDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns a vector of the QKnxServiceInfo carried by this KNXnet/IP DIB
    structure if the object that was passed during construction was valid;
    otherwise returns an empty vector.
*/
QVector<QKnxServiceInfo> QKnxNetIpServiceFamiliesDibProxy::serviceInfos() const
{
    QVector<QKnxServiceInfo> infos;
    if (!isValid())
        return infos;

    const auto &data = m_dib.constData();
    for (quint16 i = 0 ; i < m_dib.dataSize() ; i += 2)
        infos.append({ QKnxNetIp::ServiceFamily(data.value(i)), data.value(i + 1) });
    return infos;
}

/*!
    Returns a builder object to create a KNXnet/IP service families DIB
    structure.
*/
QKnxNetIpServiceFamiliesDibProxy::Builder QKnxNetIpServiceFamiliesDibProxy::builder()
{
    return QKnxNetIpServiceFamiliesDibProxy::Builder();
}


/*!
    \class QKnxNetIpServiceFamiliesDibProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpServiceFamiliesDibProxy::Builder class creates a
    KNXnet/IP supported service families DIB structure.

    A KNXnet/IP supported service families DIB structure contains a set of
    supported services and their corresponding versions.

    The common way to create such a DIB structure is:
    \code
        auto dib = QKnxNetIpServiceFamiliesDibProxy::builder()
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
QKnxNetIpServiceFamiliesDibProxy::Builder &
    QKnxNetIpServiceFamiliesDibProxy::Builder::setServiceInfos(const QVector<QKnxServiceInfo> &infos)
{
    m_infos = infos;

    std::sort(m_infos.begin(), m_infos.end(),
        [](const QKnxServiceInfo &a, const QKnxServiceInfo &b) {
            if (a.ServiceFamily < b.ServiceFamily)
                return true;
            if (a.ServiceFamily == b.ServiceFamily) {
                if (a.ServiceFamilyVersion < b.ServiceFamilyVersion)
                    return true;
            }
            return false;
    });
    m_infos.erase(std::unique(m_infos.begin(), m_infos.end(),
        [](const QKnxServiceInfo &a, const QKnxServiceInfo &b) {
        return a.ServiceFamily == b.ServiceFamily
            && a.ServiceFamilyVersion == b.ServiceFamilyVersion;
    }), m_infos.end());

    return *this;
}

/*!
    Creates and returns a QKnxNetIpDib.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpDib QKnxNetIpServiceFamiliesDibProxy::Builder::create() const
{
    QKnxByteArray bytes;
    for (const auto &info : qAsConst(m_infos))
        bytes += { quint8(info.ServiceFamily), info.ServiceFamilyVersion };
    return { QKnxNetIp::DescriptionType::SupportedServiceFamilies, bytes };
}

QT_END_NAMESPACE
