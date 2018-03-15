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

#include "qknxnetipcrd.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpCrdView

    \inmodule QtKnx
    \brief The QKnxNetIpCrdView class provides the means to read the KNXnet/IP
    connection response data (CRD) from the generic \l QKnxNetIpCrd class and to
    create a KNXnet/IP CRD structure based on the information.

    A KNXnet/IP CRD structure contains the data block returned with the connect
    response \l QKnxNetIpFrame. The content of such a frame can be viewed
    by using the \l QKnxNetIpConnectResponse view class.

    The structure may contain host protocol dependent and independent data,
    but the current KNX specification foresees additional data only in the
    case of tunneling.

    \note When using QKnxNetIpCrdView, care must be taken to ensure that the
    referenced KNXnet/IP CRD structure outlives the view on all
    code paths, lest the view ends up referencing deleted data.

    Reading the connection type and assigned individual address can be achieved
    like this:
    \code
        auto crd = QKnxNetIpCrd::fromBytes(...);

        QKnxNetIpCrdView view(crd);
        if (!view.isValid())
            return;

        if (view.connectionType() != QKnxNetIp::ConnectionType::Tunnel)
            return;
        auto address = view.individualAddress(); // read the individual address
    \endcode

    \sa builder()
*/

/*!
    \fn QKnxNetIpCrdView::QKnxNetIpCrdView()
    \internal
*/

/*!
    \fn QKnxNetIpCrdView::~QKnxNetIpCrdView()
    \internal
*/

/*!
    \fn QKnxNetIpCrdView::QKnxNetIpCrdView(const QKnxNetIpCrd &&)
    \internal
*/

/*!
    Constructs a wrapper object with the specified a KNXnet/IP structure \a crd
    to read the connection response data block (CRD).
*/
QKnxNetIpCrdView::QKnxNetIpCrdView(const QKnxNetIpCrd &crd)
    : m_crd(crd)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP CRD structure; otherwise returns \c false.
*/
bool QKnxNetIpCrdView::isValid() const
{
    switch (m_crd.code()) {
        case QKnxNetIp::ConnectionType::Tunnel: {
            const auto address = individualAddress();
            return m_crd.isValid() && m_crd.size() == 4
                && address.isValid() && address.type() == QKnxAddress::Type::Individual;
        } break;
        case QKnxNetIp::ConnectionType::DeviceManagement:
        case QKnxNetIp::ConnectionType::RemoteLogging:
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
        case QKnxNetIp::ConnectionType::ObjectServer:
            return m_crd.isValid() && m_crd.size() == 2;
        case QKnxNetIp::ConnectionType::Unknown:
            break;
    }
    return false;
}

/*!
    Returns the connection type of this KNXnet/IP structure if the object that
    was passed during construction was valid; otherwise returns
    \l QKnxNetIp::Unknown.
*/
QKnxNetIp::ConnectionType QKnxNetIpCrdView::connectionType() const
{
    if (m_crd.isValid())
        return m_crd.code();
    return QKnxNetIp::ConnectionType::Unknown;
}

/*!
    Returns the individual address of this KNXnet/IP structure if the object
    that was passed during construction was valid and the connection type
    is \l QKnxNetIp::Tunnel; otherwise returns \l QKnxNetIp::Unknown.

    \sa additionalData()
*/
QKnxAddress QKnxNetIpCrdView::individualAddress() const
{
    if (m_crd.code() == QKnxNetIp::ConnectionType::Tunnel)
        return { QKnxAddress::Type::Individual, m_crd.constData() };
    return {};
}

/*!
    Returns the additional data of this KNXnet/IP structure.

    The current KNX specification foresees additional data only in the case of
    tunneling.

    \sa individualAddress()
*/
QKnxByteArray QKnxNetIpCrdView::additionalData() const
{
    return m_crd.data();
}

/*!
    Returns a builder object to create a KNXnet/IP connection response
    data block structure.
*/
QKnxNetIpCrdView::Builder QKnxNetIpCrdView::builder()
{
    return QKnxNetIpCrdView::Builder();
}


/*!
    \class QKnxNetIpCrdView::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpCrdView::Builder class creates a KNXnet/IP connection
    response data (CDR) structure.

    A KNXnet/IP CRD structure contains the data block returned with the
    connect response \l QKnxNetIpFrame.

    The common way to create such a CRD structure is:
    \code
        auto crd = QKnxNetIpCrdView::builder()
            .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
            .setIndividualAddress(QKnxAddress::createIndividual(1, 1, 1))
            .create();
    \endcode
*/

/*!
    Sets the connection type to \a type and returns a reference to the builder.

    Does nothing if \a type is not \l QKnxNetIp::ConnectionType.
*/
QKnxNetIpCrdView::Builder &
    QKnxNetIpCrdView::Builder::setConnectionType(QKnxNetIp::ConnectionType type)
{
    if (QKnxNetIp::isStructType(type))
        m_cType = type;
    return *this;
}

/*!
    Sets the additional data to the individual address \a address and
    returns a reference to the builder.

    Does nothing if \a address is invalid or not an individual address.

    \sa setAdditionalData()
*/
QKnxNetIpCrdView::Builder &
    QKnxNetIpCrdView::Builder::setIndividualAddress(const QKnxAddress &address)
{
    if (address.isValid() && (address.type() == QKnxAddress::Type::Individual))
        setAdditionalData(address.bytes());
    return *this;
}

/*!
    Sets the additional data to \a additionalData and returns a reference to
    the builder.

    The current KNX specification foresees additional data only in the case of
    tunneling.

    \sa setIndividualAddress()
*/
QKnxNetIpCrdView::Builder &
    QKnxNetIpCrdView::Builder::setAdditionalData(const QKnxByteArray &additionalData)
{
    m_additionalData = additionalData;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP CRD structure.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpCrd QKnxNetIpCrdView::Builder::create() const
{
    return { m_cType, m_additionalData };
}
QT_END_NAMESPACE
