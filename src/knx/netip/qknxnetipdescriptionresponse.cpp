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

#include "qknxnetipdescriptionresponse.h"
#include "qknxnetipservicefamiliesdib.h"
#include "qknxnetipdevicedib.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDescriptionResponseProxy

    \inmodule QtKnx
    \brief The QKnxNetIpDescriptionResponseProxy class provides the means to
    read a description response from the generic \l QKnxNetIpFrame class and
    to create a KNXnet/IP frame based on the information.

    When a KNXnet/IP server receives a description request from a KNXnet/IP
    client, it replies by sending a description response frame that contains
    information about the supported protocol version range, its own
    capabilities, state information, and optionally a friendly name for this
    server's KNX connection.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnelConnection or \l QKnxNetIpDeviceManagementConnection
    class is used to establish a functional connection to a KNXnet/IP server.

    The following code sample illustrates how to read the description response
    information sent by a KNXnet/IP server:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpDescriptionResponseProxy response(netIpFrame);
        if (!response.isValid())
            return;

        auto hardware = response.deviceHardware();
        auto families = response.supportedFamilies();
    \endcode

    \sa builder(), QKnxNetIpDescriptionRequestProxy
*/

/*!
    \fn QKnxNetIpDescriptionResponseProxy::QKnxNetIpDescriptionResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDescriptionResponseProxy::~QKnxNetIpDescriptionResponseProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDescriptionResponseProxy::QKnxNetIpDescriptionResponseProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the description response information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpDescriptionResponseProxy::QKnxNetIpDescriptionResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpDescriptionResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 64
        && m_frame.serviceType() == QKnxNetIp::ServiceType::DescriptionResponse;
}

/*!
    Returns information about the KNXnet/IP server hardware.
*/
QKnxNetIpDib QKnxNetIpDescriptionResponseProxy::deviceHardware() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 0);
}

/*!
    Returns information about the service families that the KNXnet/IP server
    supports.
*/
QKnxNetIpDib QKnxNetIpDescriptionResponseProxy::supportedFamilies() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 54);
}

/*!
    Returns the optional KNXnet/IP server device information block (DIB)
    structure.
*/
QVector<QKnxNetIpDib> QKnxNetIpDescriptionResponseProxy::optionalDibs() const
{
    const auto &data = m_frame.constData();

    auto header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, 0);
    quint16 index = header.totalSize(); // total size of device DIB

    header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, index);
    index += header.totalSize(); // advance of total size of families DIB

    QVector<QKnxNetIpDib> dibs;
    while (index < data.size()) {
        header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, index);
        if (!header.isValid())
            return {};
        dibs.append(QKnxNetIpDib::fromBytes(data, index));
        index += header.totalSize(); // advance of total size of last read DIB
    }
    return dibs;
}

/*!
    Returns a builder object to create a KNXnet/IP description response frame.
*/
QKnxNetIpDescriptionResponseProxy::Builder QKnxNetIpDescriptionResponseProxy::builder()
{
    return QKnxNetIpDescriptionResponseProxy::Builder();
}


/*!
    \class QKnxNetIpDescriptionResponseProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpDescriptionResponseProxy::Builder class provides the
    means to create a KNXnet/IP description response frame.

    A description response frame contains information about the supported
    protocol version range, the KNXnet/IP server capabilities, state
    information, and optionally a friendly name for this server's KNX
    connection.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpTunnelConnection or \l QKnxNetIpDeviceManagementConnection
    class is used to establish a functional connection to a KNXnet/IP server.

    The common way to create a a description response is:

    \code
        auto ddib = QKnxNetIpDeviceDibProxy::builder().create();
        auto sdib = QKnxNetIpServiceFamiliesDibProxy::builder().create();

        auto response = QKnxNetIpDescriptionResponseProxy::builder()
            .setDeviceHardware(ddib)
            .setSupportedFamilies(sdib)
            .create();
    \endcode
*/

/*!
    Sets the KNXnet/IP server device information block (DIB) to \a ddib and
    returns a reference to the builder.
*/
QKnxNetIpDescriptionResponseProxy::Builder &
QKnxNetIpDescriptionResponseProxy::Builder::setDeviceHardware(const QKnxNetIpDib &ddib)
{
    if (QKnxNetIpDeviceDibProxy(ddib).isValid())
        m_ddib = ddib;
    return *this;
}

/*!
    Sets the device families supported by the KNXnet/IP server to \a sdib and
    returns a reference to the builder.
*/
QKnxNetIpDescriptionResponseProxy::Builder &
    QKnxNetIpDescriptionResponseProxy::Builder::setSupportedFamilies(const QKnxNetIpDib &sdib)
{
    if (QKnxNetIpServiceFamiliesDibProxy(sdib).isValid())
        m_sdib = sdib;
    return *this;
}

/*!
    Sets the optional KNXnet/IP server device information block (DIB) structure
    to \a dibs and returns a reference to the builder.
*/
QKnxNetIpDescriptionResponseProxy::Builder &
    QKnxNetIpDescriptionResponseProxy::Builder::setOptionalDibs(const QVector<QKnxNetIpDib> &dibs)
{
    m_optionalDibs = dibs;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP description response frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpDescriptionResponseProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DescriptionResponse, m_ddib.bytes() + m_sdib.bytes()
        + [&]() -> QKnxByteArray {
            QKnxByteArray bytes;
            for (const auto &dib : m_optionalDibs) {
                if (dib.isValid())
                    bytes += dib.bytes();
            }
            return bytes;
        }()
    };
}

QT_END_NAMESPACE
