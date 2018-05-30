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

#include "qknxnetipdescriptionrequest.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDescriptionRequestProxy

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpDescriptionRequestProxy class provides the means to read
    a description request from the generic \l QKnxNetIpFrame class and to create
    a KNXnet/IP frame based on the information.

    After discovering a KNXnet/IP server, the KNXnet/IP client sends a
    description request through a unicast or point-to-point connection to all
    control endpoints of the server.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDescriptionAgent and \l QKnxNetIpServerInfo are provided to
    check that the server supports the requested connection type and options.

    The following code sample illustrates how to read the description request
    information sent by a KNXnet/IP client:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpDescriptionRequestProxy descriptionRequest(netIpFrame);
        if (!descriptionRequest.isValid())
            return;

        auto endPoint = descriptionRequest.controlEndpoint();
    \endcode

    \sa builder(), QKnxNetIpDescriptionResponseProxy, {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpDescriptionRequestProxy::QKnxNetIpDescriptionRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDescriptionRequestProxy::~QKnxNetIpDescriptionRequestProxy()
    \internal
*/

/*!
    \fn QKnxNetIpDescriptionRequestProxy::QKnxNetIpDescriptionRequestProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the description request information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpDescriptionRequestProxy::QKnxNetIpDescriptionRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpDescriptionRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::DescriptionRequest
        && m_frame.size() == 14;
}

/*!
    Returns the control endpoint of the KNXnet/IP client sending the description
    request.
*/
QKnxNetIpHpai QKnxNetIpDescriptionRequestProxy::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

/*!
    Returns a builder object to create a KNXnet/IP description request frame.
*/
QKnxNetIpDescriptionRequestProxy::Builder QKnxNetIpDescriptionRequestProxy::builder()
{
    return QKnxNetIpDescriptionRequestProxy::Builder();
}


/*!
    \class QKnxNetIpDescriptionRequestProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpDescriptionRequestProxy

    \brief The QKnxNetIpDescriptionRequestProxy::Builder class provides the
    means to create a KNXnet/IP description request frame.

    After discovering a KNXnet/IP server, the KNXnet/IP client sends a
    description request through a unicast or point-to-point connection to all
    control endpoints of the server.

    In most programs, this class will not be used directly. Instead, the
    \l QKnxNetIpServerDescriptionAgent and \l QKnxNetIpServerInfo are provided to
    check that the server supports the requested connection type and options.

    The common way to create a a search response is:

    \code
        auto netIpFrame = QKnxNetIpDescriptionRequestProxy::builder()
            .setControlEndpoint(QKnxNetIpHpaiProxy::builder()
                .setHostAddress(QHostAddress::LocalHost)
                .setPort(3671).create())
            .create();
    \endcode
*/

/*!
    Sets the control endpoint of the KNXnet/IP client to \a hpai and returns a
    reference to the builder.
*/
QKnxNetIpDescriptionRequestProxy::Builder &
    QKnxNetIpDescriptionRequestProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP description request frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpDescriptionRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DescriptionRequest, m_hpai.bytes() };
}

QT_END_NAMESPACE
