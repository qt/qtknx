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
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

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
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 8);
}

/*!
    Returns information about the service families that the KNXnet/IP server
    supports.
*/
QKnxNetIpDib QKnxNetIpSearchResponseProxy::supportedFamilies() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 62);
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
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SearchResponse
        && m_frame.size() >= 70;
}

/*!
    Returns a builder object to create a KNXnet/IP search response frame.
*/
QKnxNetIpSearchResponseProxy::Builder QKnxNetIpSearchResponseProxy::builder()
{
    return QKnxNetIpSearchResponseProxy::Builder();
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
    \l QKnxNetIpTunnel or \l QKnxNetIpDeviceManagement
    class is used to establish a functional connection to a KNXnet/IP server.

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

    \sa QKnxNetIpHpaiProxy::builder(), QKnxNetIpServiceFamiliesDibProxy::builder(), QKnxNetIpDeviceDibProxy::builder()

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
*/
QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setSupportedFamilies(const QKnxNetIpDib &sdib)
{
    if (QKnxNetIpServiceFamiliesDibProxy(sdib).isValid())
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

QT_END_NAMESPACE
