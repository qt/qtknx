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

#include "qknxbuilderdata_p.h"
#include "qknxnetipsearchresponse.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSearchResponseProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSearchResponseProxy class provides the means to read a
    search response from the generic \l QKnxNetIpFrame class and to create a
    KNXnet/IP frame based on the information.

    In networks where IP addresses are assigned at runtime, via BootP or DHCP,
    for example, it is important to be able to discover KNXnet/IP servers within
    a subnetwork and to establish connections to them without manual input.

    A KNXnet/IP client sends a search request frame, via multicast using its
    discovery endpoint. All KNXnet/IP servers that receive the request
    immediately send a search response using the host address protocol
    information (HPAI) of the client's discovery endpoint. The search response
    frame contains the HPAI of the server's control endpoint for all further
    communication.

    In addition, a search response can contain information about the server
    hardware and the supported service families.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDiscoveryAgent and \l QKnxNetIpServerInfo are provided to
    discover KNXnet/IP servers on the network.

    The following code sample illustrates how to read the search response
    information sent by a KNXnet/IP server:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpSearchResponseProxy searchResponse(netIpFrame);
        if (!searchResponse.isValid())
            return

        auto netIpHpai = searchResponse.controlEndpoint();
        auto hardwareNetIpDib = searchResponse.deviceHardware();
        auto supportedFamillies = searchResponse.supportedFamilies();
        // ...
    \endcode

    \sa builder(), QKnxNetIpSearchRequestProxy, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSearchResponseProxy::QKnxNetIpSearchResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSearchResponseProxy::~QKnxNetIpSearchResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSearchResponseProxy::QKnxNetIpSearchResponseProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the search response information carried
    by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSearchResponseProxy::QKnxNetIpSearchResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the control endpoint of the KNXnet/IP server.
*/
QKnxNetIpHpai QKnxNetIpSearchResponseProxy::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

/*!
    Returns information about the KNXnet/IP server hardware.
*/
QKnxNetIpDib QKnxNetIpSearchResponseProxy::deviceHardware() const
{
    if (!isExtended())
        return QKnxNetIpDib::fromBytes(m_frame.constData(), 8);

    const auto variable = variableDibs();
    for (const auto &dib : variable) {
        if (dib.code() == QKnxNetIp::DescriptionType::DeviceInfo)
            return dib;
    }
    return {};
}

/*!
    Returns information about the service families that the KNXnet/IP server
    supports.

    \note The returned service families must be allowed in a search response. A
    service family higher than or equal to the security family is not allowed
    in the case of a search response. In an extended search response there is
    no such restriction.
*/
QKnxNetIpDib QKnxNetIpSearchResponseProxy::supportedFamilies() const
{
    if (!isExtended())
        return QKnxNetIpDib::fromBytes(m_frame.constData(), 62);

    const auto variable = variableDibs();
    for (const auto &dib : variable) {
        if (dib.code() == QKnxNetIp::DescriptionType::SupportedServiceFamilies)
            return dib;
    }
    return {};
}

/*!
    Returns the vector of optional KNXnet/IP server device information block
    (DIB) structures of the extended search response frame. The function
    therefor will remove the mandatory device hardware and service family DIB.

    The vector can be empty if no optional structures are present or in
    case of an error while extracting the optional DIBs.

    \note The function does not perform validity checks on the
    \l QKnxNetIpFrame used to create the description response proxy object.
*/
QVector<QKnxNetIpDib> QKnxNetIpSearchResponseProxy::optionalDibs() const
{
    auto variable = variableDibs();
    variable.erase(std::remove_if(variable.begin(), variable.end(), [](const QKnxNetIpDib &dib) {
        return (dib.code() == QKnxNetIp::DescriptionType::DeviceInfo)
            || (dib.code() == QKnxNetIp::DescriptionType::SupportedServiceFamilies);
    }), variable.end());
    return variable;
}

/*!
    Returns the vector of KNXnet/IP server device information blocks (DIBs)
    structure of the extended search response frame. The vector can be empty
    if no such structures are present or in case of an error while extracting
    the optional DIBs.

    \note The function does not perform validity checks on the
    \l QKnxNetIpFrame used to create the description response proxy object.
*/
QVector<QKnxNetIpDib> QKnxNetIpSearchResponseProxy::variableDibs() const
{
    if (!isExtended())
         return {};

    const auto &data = m_frame.constData();
    quint16 index = 8;

    QVector<QKnxNetIpDib> dibs;
    while (index < data.size()) {
        auto dib = QKnxNetIpDib::fromBytes(data, index);
        if (!dib.isValid())
            return {};
        dibs.append(dib);
        index += dib.size(); // advance of total size of last read DIB
    }
    return dibs;
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSearchResponseProxy::isValid() const
{
    const auto serviceType = m_frame.serviceType();
    return m_frame.isValid() && m_frame.size() >= 70 && (m_frame.size() % 2 == 0)
        && (serviceType == QKnxNetIp::ServiceType::SearchResponse
            || serviceType == QKnxNetIp::ServiceType::ExtendedSearchResponse)
        && controlEndpoint().code() == QKnxNetIp::HostProtocol::UDP_IPv4;
}

/*!
    Returns \c true if the frame service type is an extended search response,
    otherwise returns \c false.
*/
bool QKnxNetIpSearchResponseProxy::isExtended() const
{
    return (m_frame.serviceType() == QKnxNetIp::ServiceType::ExtendedSearchResponse);
}

/*!
    Returns a builder object to create a KNXnet/IP search response frame.
*/
QKnxNetIpSearchResponseProxy::Builder QKnxNetIpSearchResponseProxy::builder()
{
    return QKnxNetIpSearchResponseProxy::Builder();
}

/*!
    Returns a builder object to create a KNXnet/IP extended search response
    frame.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder QKnxNetIpSearchResponseProxy::extendedBuilder()
{
    return QKnxNetIpSearchResponseProxy::ExtendedBuilder();
}


/*!
    \class QKnxNetIpSearchResponseProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSearchResponseProxy

    \brief The QKnxNetIpSearchResponseProxy::Builder class provides the means to
    create a KNXnet/IP search response frame.

    A search response frame contains the host address protocol information
    (HPAI) of the KNXnet/IP server's control endpoint to use for communication
    with a KNXnet/IP client.

    In addition, a search response can contain information about the server
    hardware and the supported service families.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDiscoveryAgent and \l QKnxNetIpServerInfo are provided to
    discover KNXnet/IP servers on the network.

    The common way to create a a search response is:

    \code
        QKnxNetIpHpai controlEndpoint;
        QKnxNetIpDib deviceHardware, supportedFamillies;
        auto netIpFrame = QKnxNetIpSearchResponseProxy::builder()
            .setControlEndpoint(controlEndpoint)
            .setSupportedFamilies(deviceHardware)
            .setDeviceHardware(supportedFamillies)
            .create();
    \endcode

    \sa QKnxNetIpHpaiProxy::builder(), QKnxNetIpServiceFamiliesDibProxy::builder(),
    QKnxNetIpDeviceDibProxy::builder()

    Typically, after discovering a KNXnet/IP server, the KNXnet/IP client sends
    a description request, \l QKnxNetIpDescriptionRequestProxy, through a
    unicast or point-to-point connection to all control endpoints of the
    KNXnet/IP server.
*/

/*!
    Sets the control endpoint of the KNXnet/IP server to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Sets the KNXnet/IP server device information block (DIB) to \a ddib and
    returns a reference to the builder.
*/
QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setDeviceHardware(const QKnxNetIpDib &ddib)
{
    if (QKnxNetIpDeviceDibProxy(ddib).isValid())
        m_ddib = ddib;
    return *this;
}

/*!
    Sets the device families supported by the KNXnet/IP server to \a sdib and
    returns a reference to the builder.

    \note A service family higher than or equal to the security is not allowed
    in a search response and it shall not be set in the builder if it is passed
    in via this method.
*/
QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setSupportedFamilies(const QKnxNetIpDib &sdib)
{
    QKnxNetIpServiceFamiliesDibProxy supFamily(sdib);
    if (!supFamily.isValid())
            return *this;

    for (const auto &serviceInfo : supFamily.serviceInfos()) {
        if (serviceInfo.ServiceFamily >= QKnxNetIp::ServiceFamily::Security)
            return *this;
    }

    m_sdib = sdib;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP search response frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSearchResponseProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::SearchResponse, m_hpai.bytes() + m_ddib.bytes() + m_sdib
        .bytes() };
}
/*!
    \class QKnxNetIpSearchResponseProxy::ExtendedBuilder

    \inmodule QtKnx
    \ingroup qtknx-netip
    \inheaderfile QKnxNetIpSearchResponseProxy

    \brief The QKnxNetIpSearchResponseProxy::ExtendedBuilder class provides
    the means to create a KNXnet/IP extended search response.

    The KNXnet/IP server sends the extended search response frame as an
    answer to a received extended search request frame. It is addressed
    to the KNXnet/IP client’s discovery endpoint using the host address
    protocol information (HPAI) included in the received extended search
    request frame. The HPAI of the KNXnet/IP server’s own control endpoint
    is carried in the KNXnet/IP body of the extended search response frame
    along with the description of the device hardware and the supported service
    families. If the KNXnet/IP server supports more than one KNX connection,
    the KNXnet/IP server announces each of its own control endpoints in
    a single extended search response frame. KNXnet/IP servers supporting
    TCP only report the UDP address of their control endpoint in the
    extended search response frame. The KNXnet/IP server reports the
    data information blocks (DIBs) in the response in any order. Each DIB
    is present only once in the response.

    The common way to create an extended search response is:

    \code
        QKnxNetIpHpai controlEndpoint;
        QKnxNetIpDib deviceHardware, supportedFamillies;

        QSet<QKnxNetIpDib> optDibs = ...;

        auto netIpFrame = QKnxNetIpSearchResponseProxy::extendedBuilder()
            .setControlEndpoint(controlEndpoint)
            .setSupportedFamilies(deviceHardware)
            .setDeviceHardware(supportedFamillies)
            .setOptionalDibs(optDibs)
            .create();
    \endcode


    \sa QKnxNetIpSearchResponseProxy::Builder, QKnxNetIpHpaiProxy::Builder,
    QKnxNetIpDeviceDibProxy::Builder, QKnxNetIpServiceFamiliesDibProxy::Builder
*/

/*!
    Creates an extended search response builder.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder::ExtendedBuilder()
    : d_ptr(new QKnxNetIpSearchResponseExtendedBuilderPrivate)
{}

/*!
    Destroys an extended search response builder.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder::~ExtendedBuilder() = default;

/*!
    Sets the control endpoint of the KNXnet/IP client to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder &
    QKnxNetIpSearchResponseProxy::ExtendedBuilder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    d_ptr->m_hpai = hpai;
    return *this;
}

/*!
    Sets the device hardware device information block (DIB) and returns a
    reference to the builder.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder &
    QKnxNetIpSearchResponseProxy::ExtendedBuilder::setDeviceHardware(const QKnxNetIpDib &ddib)
{
    if (QKnxNetIpDeviceDibProxy(ddib).isValid())
        d_ptr->m_hardware = ddib;
    return *this;
}

/*!
    Sets the supported families information block (DIB) and returns a
    reference to the builder.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder &
    QKnxNetIpSearchResponseProxy::ExtendedBuilder::setSupportedFamilies(const QKnxNetIpDib &sdib)
{
    if (QKnxNetIpServiceFamiliesDibProxy(sdib).isValid())
        d_ptr->m_supFamilies = sdib;
    return *this;
}

/*!
    Sets the optional KNXnet/IP server device information block (DIB) structure
    to \a dibs and returns a reference to the builder.

    \note The device information blocks \a dibs argument may not contain the
    mandatory device hardware DIB and supported families DIB.
    To set the mandatory DIBs use the dedicated setter functions instead.

    \sa setDeviceHardware(), setSupportedFamilies()
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder &
    QKnxNetIpSearchResponseProxy::ExtendedBuilder::setOptionalDibs(const QSet<QKnxNetIpDib> &dibs)
{
    d_ptr->m_optionalDibs = dibs;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP extended search response frame.

    \note The returned frame may be invalid depending on the values used
    during setup. For the frame to be valid, at least a device hardware
    information block (DIB) and a supported families DIB must be set in the
    builder.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSearchResponseProxy::ExtendedBuilder::create() const
{
    if (!(QKnxNetIpDeviceDibProxy(d_ptr->m_hardware).isValid()
        && QKnxNetIpServiceFamiliesDibProxy(d_ptr->m_supFamilies).isValid())) {
            return { QKnxNetIp::ServiceType::ExtendedSearchResponse };
    }
    auto dibsBytes = d_ptr->m_hpai.bytes();
    dibsBytes += d_ptr->m_hardware.bytes();
    dibsBytes += d_ptr->m_supFamilies.bytes();

    for (auto &dib : d_ptr->m_optionalDibs) {
        if (dib.code() != QKnxNetIp::DescriptionType::DeviceInfo
            && dib.code() != QKnxNetIp::DescriptionType::SupportedServiceFamilies) {
                dibsBytes += dib.bytes();
        }
    }
    return { QKnxNetIp::ServiceType::ExtendedSearchResponse, dibsBytes };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder::ExtendedBuilder(const ExtendedBuilder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this builder.
*/
QKnxNetIpSearchResponseProxy::ExtendedBuilder &
    QKnxNetIpSearchResponseProxy::ExtendedBuilder::operator=(const ExtendedBuilder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
