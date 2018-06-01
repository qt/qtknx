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

#include "qknxnetipsearchrequest.h"
#include "qknxbuilderdata_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSearchRequestProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSearchRequestProxy class provides the means to read a
    search request from the generic \l QKnxNetIpFrame class and to create a
    KNXnet/IP frame based on the information.

    In networks where IP addresses are assigned at runtime, via BootP or DHCP,
    for example, it is important that KNXnet/IP clients are able to discover
    KNXnet/IP servers within a subnetwork and to establish connections to them
    without manual input.

    To discover servers, a client sends a search request data packet via
    multicast using its discovery endpoint. The search request contains the host
    address protocol information (HPAI) of the discovery endpoint. The HPAI may
    also contain a unicast IP address to receive the answers from the different
    servers directly in a point-to-point manner. Typically, it should contain
    the KNXnet/IP system setup multicast address to ensure reception from
    KNXnet/IP servers that are on a different subnetwork.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDiscoveryAgent and \l QKnxNetIpServerInfo are provided to
    discover KNXnet/IP servers on the network.

    The following code sample illustrates how to read the search request
    information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        QKnxNetIpSearchRequestProxy searchRequest(netIpFrame);
        if (!searchRequest.isValid())
            return;

        QKnxNetIpHpai hpai = searchRequest.discoveryEndpoint();
        // ...
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSearchRequestProxy::QKnxNetIpSearchRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSearchRequestProxy::~QKnxNetIpSearchRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSearchRequestProxy::QKnxNetIpSearchRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the search request information carried
    by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSearchRequestProxy::QKnxNetIpSearchRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of a
    search request with at least a valid header and a size in bytes
    corresponding to the total size of the KNXnet/IP frame header or a extended
    search request containing a valid header.

    \note The extended search parameter's validity check is not done by this
    function. It is up to the programmer to do the appropriate validity checks on
    that field.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSearchRequestProxy::isValid() const
{
    auto serviceType = m_frame.serviceType();
    if (serviceType == QKnxNetIp::ServiceType::SearchRequest) {
        return m_frame.isValid() && m_frame.size() == 14;
    } else if (serviceType == QKnxNetIp::ServiceType::SearchRequestExtended) {
        return m_frame.isValid()
               && m_frame.size() >= 14
               && (m_frame.size() % 2 == 0);
    }
    return false;
}

/*!
    Returns \c true if the frame service type is search request extended,
    otherwise returns \c false.
*/
bool QKnxNetIpSearchRequestProxy::isExtended() const
{
    return (m_frame.serviceType() == QKnxNetIp::ServiceType::SearchRequestExtended);
}

/*!
    Returns the discovery endpoint of the KNXnet/IP client.
*/
QKnxNetIpHpai QKnxNetIpSearchRequestProxy::discoveryEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

/*!
    Returns the extended search parameter structure.
*/
QVector<QKnxNetIpSrp> QKnxNetIpSearchRequestProxy::extendedSearchParameters() const
{
    QVector<QKnxNetIpSrp> srps;
    if (!isValid())
        return srps;

    const auto &data = m_frame.constData();
    quint16 index = discoveryEndpoint().size();
    while (index < data.size()) {
        auto srp = QKnxNetIpSrp::fromBytes(data, index);
        srps.append(srp);
        index += srp.size();
    }
    return srps;
}

/*!
    Returns a builder object to create a KNXnet/IP search request frame.
*/
QKnxNetIpSearchRequestProxy::Builder QKnxNetIpSearchRequestProxy::builder()
{
    return QKnxNetIpSearchRequestProxy::Builder();
}

/*!
    \class QKnxNetIpSearchRequestProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSearchRequestProxy

    \brief The QKnxNetIpSearchRequestProxy::Builder class provides the means to
    create a KNXnet/IP search request.

    To discover KNXnet/IP servers, a KNXnet/IP client sends a search request
    data packet via multicast using its discovery endpoint. The search request
    contains the host address protocol information (HPAI) of the discovery
    endpoint. The HPAI may also contain a unicast IP address to receive the
    answers from the different servers directly in a point-to-point manner.
    Typically, it should contain the KNXnet/IP system setup multicast address
    to ensure reception from KNXnet/IP servers that are on a different
    subnetwork.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDiscoveryAgent and \l QKnxNetIpServerInfo are provided to
    discover KNXnet/IP servers on the network.

    The common way to create a search request is:

    \code
        auto hpai = QKnxNetIpHpaiProxy::builder().create();

        auto netIpFrame = QKnxNetIpSearchRequestProxy::builder()
            .setDiscoveryEndpoint(hpai)
            .create();
    \endcode

    After sending the search request, the KNXnet/IP client waits until timeout
    for the response frame from the KNXnet/IP server,
    \l QKnxNetIpSearchResponseProxy. After the timeout, received response
    frames are ignored by the client until it sends another search request.
    Search requests received from other clients are always ignored.

    \sa QKnxNetIpHpaiProxy::Builder
*/

/*!
    Sets the discovery endpoint of the KNXnet/IP client to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpSearchRequestProxy::Builder &
    QKnxNetIpSearchRequestProxy::Builder::setDiscoveryEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP search request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSearchRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::SearchRequest, m_hpai.bytes() };
}

/*!
    \class QKnxNetIpSearchRequestProxy::ExtendedBuilder

    \inmodule QtKnx
    \brief The QKnxNetIpSearchRequestProxy::ExtendedBuilder class provides
    the means to create a KNXnet/IP extended search request.

    To discover KNXnet/IP servers, a KNXnet/IP client sends a extended search
    request data packet via multicast using its discovery endpoint. The request
    contains the host address protocol information (HPAI) of the discovery
    endpoint. The HPAI may also contain a unicast IP address to receive the
    answers from the different servers directly in a point-to-point manner.
    Typically, it should contain the KNXnet/IP system setup multicast address
    to ensure reception from KNXnet/IP servers that are on a different
    subnetwork. Additionally, the client may include 0 or more search request
    parameter blocks.

    The common way to create a search request is:

    \code
        auto hpai = QKnxNetIpHpaiProxy::builder().create();
        auto macAddress = QKnxByteArray::fromHex("4CCC6AE40000");
        QKnxNetIpSrp macSrp = SrpBuilders::MacAddress()
                               .setMandatory()
                               .setMac(macAddress)
                               .create();
        auto modeSrp = SrpBuilders::ProgrammingMode()
                       .setMandatory()
                       .create();
        auto srps = { macSrp, modeSrp };
        auto netIpFrame = QKnxNetIpSearchRequestProxy::ExtendedBuilder()
            .setDiscoveryEndpoint(hpai)
            .setExtendedParameter(srps)
            .create();
    \endcode

    After sending the search request, the KNXnet/IP client waits until timeout
    for the response frame from the KNXnet/IP server,
    \l QKnxNetIpSearchResponseProxy. After the timeout, received response
    frames are ignored by the client until it sends another search request.
    Search requests received from other clients are always ignored.

    \sa QKnxNetIpHpaiProxy::Builder
*/

/*!
    Creates an extended search request builder.
*/
QKnxNetIpSearchRequestProxy::ExtendedBuilder::ExtendedBuilder()
    : d_ptr(new ExtendedBuilderPrivate)
{}

/*!
    Destroys an extended search request builder.
*/
QKnxNetIpSearchRequestProxy::ExtendedBuilder::~ExtendedBuilder()
{}

/*!
    Sets the discovery endpoint of the KNXnet/IP client to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpSearchRequestProxy::ExtendedBuilder &
QKnxNetIpSearchRequestProxy::ExtendedBuilder::setDiscoveryEndpoint(const QKnxNetIpHpai &hpai)
{
    d_ptr->m_hpai = hpai;
    return *this;
}

/*!
    Adds the QVector of extended parameters \a srps to the KNXnet/IP extended
    search request.
*/
QKnxNetIpSearchRequestProxy::ExtendedBuilder &
QKnxNetIpSearchRequestProxy::ExtendedBuilder::setExtendedParameters(const QVector<QKnxNetIpSrp> &srps)
{
    d_ptr->m_srps = srps;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP extended search request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSearchRequestProxy::ExtendedBuilder::create() const
{
    QKnxByteArray srpBytes;
    for (auto &srp : d_ptr->m_srps)
        srpBytes += srp.bytes();

    return { QKnxNetIp::ServiceType::SearchRequestExtended, d_ptr->m_hpai.bytes() + srpBytes };
}


/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSearchRequestProxy::ExtendedBuilder::ExtendedBuilder(const QKnxNetIpSearchRequestProxy::ExtendedBuilder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this builder.
*/
QKnxNetIpSearchRequestProxy::ExtendedBuilder &QKnxNetIpSearchRequestProxy::ExtendedBuilder::operator=(const QKnxNetIpSearchRequestProxy::ExtendedBuilder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
