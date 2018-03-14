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

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpCriView

    \inmodule QtKnx
    \brief The QKnxNetIpCriView class generates a KNXnet/IP connection request
    information (CRI) structure.

    This class provides the means to read KNXnet/IP CRI from the generic
    \l QKnxNetIpCri class and to create a KNXnet/IP CRI structure based on the
    information.

    A KNXnet/IP CRI structure contains additional information needed for
    different types of communication channels to fulfill a connection request.

    The structure may contain host protocol dependent and independent data,
    but the current KNX specification foresees additional data only in the
    case of tunneling.

    \note When using QKnxNetIpCriView, care must be taken to ensure that the
    referenced KNXnet/IP CRI structure outlives the view on all code paths, lest
    the view ends up referencing deleted data.

    Reading the connection type and tunneling layer can be achieved like this:
    \code
        auto cri = QKnxNetIpCri::fromBytes(...);

        QKnxNetIpCriView view(cri);
        if (!view.isValid())
            return;

        if (view.connectionType() != QKnxNetIp::ConnectionType::Tunnel)
            return;
        auto layer = view.tunnelingLayer(); // read the requested tunneling layer
    \endcode

    \sa builder()
*/

/*!
    \fn QKnxNetIpCriView::QKnxNetIpCriView()
    \internal
*/

/*!
    \fn QKnxNetIpCriView::~QKnxNetIpCriView()
    \internal
*/

/*!
    \fn QKnxNetIpCriView::QKnxNetIpCriView(const QKnxNetIpCri &&)
    \internal
*/

/*!
    Constructs a wrapper object with the specified a KNXnet/IP structure
    \a cri to read the connection request information (CRI).
*/
QKnxNetIpCriView::QKnxNetIpCriView(const QKnxNetIpCri &cri)
    : m_cri(cri)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP CRI structure; otherwise returns \c false.
*/
bool QKnxNetIpCriView::isValid() const
{
    switch (m_cri.code()) {
        case QKnxNetIp::ConnectionType::Tunnel:
            return m_cri.isValid() && m_cri.size() == 4
                && QKnxNetIp::isTunnelingLayer(tunnelingLayer());
        case QKnxNetIp::ConnectionType::DeviceManagement:
        case QKnxNetIp::ConnectionType::RemoteLogging:
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
        case QKnxNetIp::ConnectionType::ObjectServer:
            return m_cri.isValid() && m_cri.size() == 2;
        case QKnxNetIp::ConnectionType::Unknown:
            break;
    }
    return false;
}

/*!
    Returns the connection type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnxNetIp::Unknown.
*/
QKnxNetIp::ConnectionType QKnxNetIpCriView::connectionType() const
{
    if (m_cri.isValid())
        return m_cri.code();
    return QKnxNetIp::ConnectionType::Unknown;
}

/*!
    Returns the tunneling layer of this KNXnet/IP structure if the object that
    was passed during construction was valid and the connection type
    is \l QKnxNetIp::Tunnel, otherwise returns \l QKnxNetIp::Unknown.

    \sa additionalData()
*/
QKnxNetIp::TunnelingLayer QKnxNetIpCriView::tunnelingLayer() const
{
    if (m_cri.code() == QKnxNetIp::ConnectionType::Tunnel)
        return QKnxNetIp::TunnelingLayer(m_cri.constData().value(0));
    return QKnxNetIp::TunnelingLayer::Unknown;
}

/*!
    Returns the additional data of this KNXnet/IP structure.

    The current KNX specification foresees additional data only in the case of
    tunneling.

    \sa tunnelingLayer()
*/
QKnxByteArray QKnxNetIpCriView::additionalData() const
{
    return m_cri.data();
}

/*!
    Returns a builder object to create a KNXnet/IP connection request
    information structure.
*/
QKnxNetIpCriView::Builder QKnxNetIpCriView::builder()
{
    return QKnxNetIpCriView::Builder();
}


/*!
    \class QKnxNetIpCriView::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpCriView::Builder class creates a KNXnet/IP connection
    request information structure (CRI).

    The connection request information structure contains additional
    information needed for different types of communication channels to
    fulfill a connection request.

    The common way to create such a CRI structure is:
    \code
        auto cri = QKnxNetIpCriView::builder()
            .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
            .setTunnelingLayer(QKnxNetIp::TunnelingLayer::Link)
            .create();
    \endcode
*/

/*!
    Sets the connection type to \a type and returns a reference to the builder.

    Does nothing if \a type is not a \l QKnxNetIp::ConnectionType.
*/
QKnxNetIpCriView::Builder &
    QKnxNetIpCriView::Builder::setConnectionType(QKnxNetIp::ConnectionType type)
{
    if (QKnxNetIp::isStructType(type))
        m_cType = type;
    return *this;
}

/*!
    Sets the additional data to the requested KNX tunneling layer \a layer and
    returns a reference to the builder.

    Does nothing if \a layer is not a \l QKnxNetIp::TunnelingLayer value.

    \sa setAdditionalData()
*/
QKnxNetIpCriView::Builder &
    QKnxNetIpCriView::Builder::setTunnelingLayer(QKnxNetIp::TunnelingLayer layer)
{
    if (QKnxNetIp::isTunnelingLayer(layer))
        setAdditionalData({ quint8(layer), 0x00 });
    return *this;
}

/*!
    Sets the additional data to \a additionalData and returns a reference to
    the builder.

    The current KNX specification foresees additional data only in the case of
    tunneling.

    \sa setTunnelingLayer()
*/
QKnxNetIpCriView::Builder &
    QKnxNetIpCriView::Builder::setAdditionalData(const QKnxByteArray &additionalData)
{
    m_additionalData = additionalData;
    return *this;
}

/*!
    Creates and returns a \l QKnxNetIpCri structure.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()
*/
QKnxNetIpCri QKnxNetIpCriView::Builder::create() const
{
    return { m_cType, m_additionalData };
}

QT_END_NAMESPACE
