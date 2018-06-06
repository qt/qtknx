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

#include "qknxnetiptunnelinginfodib.h"

QT_BEGIN_NAMESPACE


/*!
    \inmodule QtKnx
    \class QKnxNetIpTunnelingSlotInfo

    \since 5.12
    \ingroup qtknx-netip
    \ingroup qtknx-tunneling

    \brief The QKnxNetIpTunnelingSlotInfo class contains information about the
    individual address that is currently assigned to the tunneling slot as well
    as the current state of the tunneling slot.

    If a KNXnet/IP server supports tunneling, it can send more detailed
    information about the supported tunnel connections. The tunneling slot
    info class can be used to represent this information. Usually it includes
    the individual addresses used for the connection once it has been
    established and whether the connection slot is available at all.

    \sa QKnxNetIpTunnelingInfoDibProxy, {Qt KNX Tunneling Classes},
        {Qt KNXnet/IP Connection Classes}
*/

class QKnxNetIpTunnelingSlotInfoPrivate : public QSharedData
{
public:
    QKnxNetIpTunnelingSlotInfoPrivate() = default;
    ~QKnxNetIpTunnelingSlotInfoPrivate() = default;

    QKnxNetIpTunnelingSlotInfoPrivate(const QKnxAddress &ia, QKnxNetIpTunnelingSlotInfo::Status s)
        : m_ia(ia)
        , m_status(s)
    {}

    QKnxAddress m_ia;
    QKnxNetIpTunnelingSlotInfo::Status m_status { QKnxNetIpTunnelingSlotInfo::State::Invalid };
};

/*!
    \enum QKnxNetIpTunnelingSlotInfo::State

    This enumeration describes the current state of the tunneling slot. Only if
    all values are set, the slot is available for a KNXnet/IP client connection.

    \value NotAvailable
            Indicates that the slot is not available for a KNXnet/IP client
            connection.
    \value Free
            Indicates whether this tunneling slot is currently free or not.
    \value Authorized
             Indications about the authorization of this tunneling slot.
    \value Usable
            Indicates whether this tunneling slot is currently usable or not.
    \value Available
            Indicates that the slot is available for a KNXnet/IP client connection.
    \omitvalue Invalid
*/

/*!
    Creates a new empty invalid tunneling slot information object.
*/
QKnxNetIpTunnelingSlotInfo::QKnxNetIpTunnelingSlotInfo()
    : d_ptr(new QKnxNetIpTunnelingSlotInfoPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTunnelingSlotInfo::~QKnxNetIpTunnelingSlotInfo() = default;

/*!
    Creates a tunneling slot information object with the individual address set
    to \a ia and the status field set to \l NotAvailable.
*/
QKnxNetIpTunnelingSlotInfo::QKnxNetIpTunnelingSlotInfo(const QKnxAddress &ia)
    : d_ptr(new QKnxNetIpTunnelingSlotInfoPrivate(ia, State::NotAvailable))
{}

/*!
    Creates a tunneling slot information object with the individual address set
    to \a ia and the status field set to \a status.
*/
QKnxNetIpTunnelingSlotInfo::QKnxNetIpTunnelingSlotInfo(const QKnxAddress &ia, Status status)
    : d_ptr(new QKnxNetIpTunnelingSlotInfoPrivate(ia, status))
{}

/*!
    Returns \c true if the tunneling slot information is valid; otherwise
    returns \c false. The object is considered valid if the individual address
    is valid and the status field is properly set.
*/
bool QKnxNetIpTunnelingSlotInfo::isValid() const
{
    return d_ptr->m_ia.isValid() && (d_ptr->m_ia.type() == QKnxAddress::Type::Individual)
        && (quint32(d_ptr->m_status) <= 0x07); // only the first 3 bits can be set as of now
}

/*!
    Returns the individual address of the tunneling information slot object.
*/
QKnxAddress QKnxNetIpTunnelingSlotInfo::individualAddress() const
{
    return d_ptr->m_ia;
}

/*!
    Sets the individual address of the tunneling information slot object
    to \a ia if the passed argument is a valid \l QKnxAddress and of type
    \l {QKnxAddress::Type} {QKnxAddress::Individual}.
*/
void QKnxNetIpTunnelingSlotInfo::setIndividualAddress(const QKnxAddress &ia)
{
    if (ia.isValid() && (ia.type() == QKnxAddress::Type::Individual))
        d_ptr->m_ia = ia;
}

/*!
    Returns the status of the tunneling information slot object.
*/
QKnxNetIpTunnelingSlotInfo::Status QKnxNetIpTunnelingSlotInfo::status() const
{
    return d_ptr->m_status;
}

/*!
    Sets the status of the tunneling information slot object to \a status if
    the passed argument is a valid.
*/
void QKnxNetIpTunnelingSlotInfo::setStatus(QKnxNetIpTunnelingSlotInfo::Status status)
{
    if (quint32(status) <= 0x07) // only the first 3 bits can be set as of now
        d_ptr->m_status = status;
}

/*!
    Returns an array of data that represents the tunneling information slot
    object.
*/
QKnxByteArray QKnxNetIpTunnelingSlotInfo::bytes() const
{
    if (!isValid())
        return {};
    return d_ptr->m_ia.bytes() + QKnxUtils::QUint16::bytes(0xfff8 | quint16(d_ptr->m_status));
}

/*!
    Constructs the tunneling information slot object from the byte array \a data
    starting at the position \a index inside the array.
*/
QKnxNetIpTunnelingSlotInfo
    QKnxNetIpTunnelingSlotInfo::fromBytes(const QKnxByteArray &data, quint16 index)
{
    const qint32 availableSize = data.size() - index;
    if (availableSize < 4) // 2 bytes for address and 2 bytes for the status
        return {}; // not enough data ...

    auto status = QKnxUtils::QUint16::fromBytes(data.mid(index + 2));
    return { { QKnxAddress::Type::Individual, data.mid(index, 2) },
        QKnxNetIpTunnelingSlotInfo::Status(quint32(status) & 0x00000007) };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTunnelingSlotInfo::QKnxNetIpTunnelingSlotInfo(const QKnxNetIpTunnelingSlotInfo &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTunnelingSlotInfo &
    QKnxNetIpTunnelingSlotInfo::operator=(const QKnxNetIpTunnelingSlotInfo &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxNetIpTunnelingSlotInfo::swap(QKnxNetIpTunnelingSlotInfo &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxNetIpTunnelingSlotInfo::QKnxNetIpTunnelingSlotInfo(QKnxNetIpTunnelingSlotInfo &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxNetIpTunnelingSlotInfo &
    QKnxNetIpTunnelingSlotInfo::operator=(QKnxNetIpTunnelingSlotInfo &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxNetIpTunnelingSlotInfo::operator==(const QKnxNetIpTunnelingSlotInfo &other) const
{
    return d_ptr == other.d_ptr
        || (d_ptr->m_ia == other.d_ptr->m_ia && d_ptr->m_status == other.d_ptr->m_status);
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxNetIpTunnelingSlotInfo::operator!=(const QKnxNetIpTunnelingSlotInfo &other) const
{
    return !operator==(other);
}

/*!
    \class QKnxNetIpTunnelingInfoDibProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip
    \ingroup qtknx-tunneling

    \brief The QKnxNetIpTunnelingInfoDibProxy class provides the means to read
    the maximum ADPU length supported by a KNXnet/IP tunneling interface and
    tunneling slot information from the generic \l QKnxNetIpDib class and to
    create a KNXnet/IP tunneling information block (DIB) structure.

    If a KNXnet/IP server supports tunneling, it can send more detailed
    information about the supported tunnel connections. The tunneling slot
    info class can be used to represent this information. Usually it includes
    the individual addresses used for the connection once it has been
    established and whether the connection slot is available at all.

    \note When using QKnxNetIpTunnelingInfoDibProxy, care must be taken to
    ensure that the referenced KNXnet/IP DIB structure outlives the view on all
    code paths, lest the view ends up referencing deleted data.

    Reading the application layer protocol data unit (APDU) length and default
    tunneling slot information can be achieved like this:
    \code
        auto dib = QKnxNetIpDib::fromBytes(...);

        QKnxNetIpTunnelingInfoDibProxy proxy(dib);
        if (!proxy.isValid())
            return;

        quint16 apduLength = proxy.maximumInterfaceApduLength();
        auto tunnelingSlotInfo = proxy.tunnelingSlotInfo(); // mandatory
    \endcode

    \sa builder(), QKnxNetIpTunnelingSlotInfo, {Qt KNX Tunneling Classes},
        {Qt KNXnet/IP Connection Classes}
*/

/*!
    \internal
    \fn QKnxNetIpTunnelingInfoDibProxy::QKnxNetIpTunnelingInfoDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpTunnelingInfoDibProxy::~QKnxNetIpTunnelingInfoDibProxy()
*/

/*!
    \internal
    \fn QKnxNetIpTunnelingInfoDibProxy::QKnxNetIpTunnelingInfoDibProxy(const QKnxNetIpDib &&)
*/

/*!
    Constructs a proxy object with the specified a KNXnet/IP DIB structure
    \a dib to read the maximum APDU length and tunneling slot information.
*/
QKnxNetIpTunnelingInfoDibProxy::QKnxNetIpTunnelingInfoDibProxy(const QKnxNetIpDib &dib)
    : m_dib(dib)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP DIB structure; otherwise returns \c false.

    A KNXnet/IP tunneling information DIB structure is considered valid if it
    contains the maximum APDU length and at least one tunneling slot information
    object.
*/
bool QKnxNetIpTunnelingInfoDibProxy::isValid() const
{
    return m_dib.isValid() && (m_dib.code() == QKnxNetIp::DescriptionType::TunnelingInfo)
        && (m_dib.size() >= 8) && (m_dib.size() % 4 == 0);
}

/*!
    Returns the description type of this KNXnet/IP structure if the object
    that was passed during construction was valid; otherwise returns
    \l QKnx::NetIp::Unknown.
*/
QKnxNetIp::DescriptionType QKnxNetIpTunnelingInfoDibProxy::descriptionType() const
{
    if (isValid())
        return m_dib.code();
    return QKnxNetIp::DescriptionType::Unknown;
}

/*!
    Returns the \l {QKnxInterfaceObjectProperty::MaxInterfaceApduLength}
    {maximum APDU} length carried by this KNXnet/IP DIB structure.

    \note If the object passed during construction was invalid, the function
    returns a \l {default-constructed value} which can be \c 0.

    \sa isValid()
*/
quint16 QKnxNetIpTunnelingInfoDibProxy::maximumInterfaceApduLength() const
{
    return QKnxUtils::QUint16::fromBytes(m_dib.constData());
}

/*!
    Returns the mandatory tunneling slot information carried by this KNXnet/IP
    DIB structure or a \l {default-constructed value} in case of an error.
*/
QKnxNetIpTunnelingSlotInfo QKnxNetIpTunnelingInfoDibProxy::tunnelingSlotInfo() const
{
    return QKnxNetIpTunnelingSlotInfo::fromBytes(m_dib.constData(), 2);
}

/*!
    Returns a vector of \l QKnxNetIpTunnelingSlotInfo objects carried by this
    KNXnet/IP DIB structure if the object that was passed during construction
    was valid; otherwise returns an empty vector.
*/
QVector<QKnxNetIpTunnelingSlotInfo> QKnxNetIpTunnelingInfoDibProxy::optionalSlotInfos() const
{
    QVector<QKnxNetIpTunnelingSlotInfo> infos;
    if (!isValid())
        return infos;

    const auto &data = m_dib.constData();
    // 6 bytes max APDU + mandatory slot info, advance by 4 -> expected info size
    for (quint16 i = 6 ; i < data.size(); i += 4) {
        auto info = QKnxNetIpTunnelingSlotInfo::fromBytes(data, i);
        if (!info.isValid())
            return {};
        infos.append(info);
    }
    return infos;
}

/*!
    Returns a builder object to create a KNXnet/IP tunneling info DIB structure.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder QKnxNetIpTunnelingInfoDibProxy::builder()
{
    return QKnxNetIpTunnelingInfoDibProxy::Builder();
}


/*!
    \class QKnxNetIpTunnelingInfoDibProxy::Builder

    \since 5.12
    \inmodule QtKnx
    \inheaderfile QKnxNetIpTunnelingInfoDibProxy

    \brief The QKnxNetIpTunnelingInfoDibProxy::Builder class creates a
    KNXnet/IP tunneling info DIB structure.

    A KNXnet/IP tunneling info DIB structure contains the maximum application
    layer protocol data unit (APDU) length supported by a KNXnet/IP tunneling
    interface for bus access and one or more tunneling slot information objects.

    The common way to create such a DIB structure is:
    \code
        auto dib = QKnxNetIpTunnelingInfoDibProxy::builder()
            .setMaximumInterfaceApduLength(0x1000)
            .setTunnelingSlotInfo({ { QKnxAddress::Type::Individual, 1976 },
                QKnxNetIpTunnelingSlotInfo::Available
            }).setOptionalSlotInfos({
                { { QKnxAddress::Type::Individual, 2013 },
                    QKnxNetIpTunnelingSlotInfo::Usable
                        | QKnxNetIpTunnelingSlotInfo::Authorized
                        | QKnxNetIpTunnelingSlotInfo::Free },
                { { QKnxAddress::Type::Individual, 1978 }, QKnxNetIpTunnelingSlotInfo::NotAvailable },
                { QKnxNetIpTunnelingSlotInfo({ QKnxAddress::Type::Individual, 2002 }) }
            }).create();
    \endcode
*/

class QKnxNetIpTunnelingInfoDibPrivate : public QSharedData
{
public:
    QKnxNetIpTunnelingInfoDibPrivate() = default;
    ~QKnxNetIpTunnelingInfoDibPrivate() = default;

    quint16 maxApduLength { 0 };
    QKnxNetIpTunnelingSlotInfo m_info; // mandatory
    QVector<QKnxNetIpTunnelingSlotInfo> m_infos;
};

/*!
    Creates a new empty tunneling info DIB structure builder object.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder::Builder()
    : d_ptr(new QKnxNetIpTunnelingInfoDibPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder::~Builder() = default;

/*!
    Sets the maximum APDU length of the KNXnet/IP DIB structure to \a length
    and returns a reference to the builder.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder &
    QKnxNetIpTunnelingInfoDibProxy::Builder::setMaximumInterfaceApduLength(quint16 length)
{
    d_ptr->maxApduLength = length;
    return *this;
}

/*!
    Sets the mandatory tunneling slot information of the KNXnet/IP DIB structure
    to \a info and returns a reference to the builder.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder &QKnxNetIpTunnelingInfoDibProxy::Builder
    ::setTunnelingSlotInfo(const QKnxNetIpTunnelingSlotInfo &info)
{
    d_ptr->m_info = info;
    return *this;
}

/*!
    Sets the optional tunneling slot information of the KNXnet/IP DIB structure
    to \a infos and returns a reference to the builder.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder &QKnxNetIpTunnelingInfoDibProxy::Builder
    ::setOptionalSlotInfos(const QVector<QKnxNetIpTunnelingSlotInfo> &infos)
{
    d_ptr->m_infos = infos;
    return *this;
}

/*!
    Creates and returns a tunneling info QKnxNetIpDib.

    \note The returned structure may be invalid depending on the values used
    during setup.

    \sa isValid()

    \note Invalid tunneling slot information objects are not taken into account
    when creating the tunneling information QKnxNetIpDib.
*/
QKnxNetIpDib QKnxNetIpTunnelingInfoDibProxy::Builder::create() const
{
    if (!d_ptr->m_info.isValid())
        return { QKnxNetIp::DescriptionType::TunnelingInfo };

    auto bytes = QKnxUtils::QUint16::bytes(d_ptr->maxApduLength) + d_ptr->m_info.bytes();
    for (const auto &info : qAsConst(d_ptr->m_infos)) {
        if (info.isValid())
            bytes += info.bytes();
    }
    return { QKnxNetIp::DescriptionType::TunnelingInfo, bytes };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpTunnelingInfoDibProxy::Builder &
    QKnxNetIpTunnelingInfoDibProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


QT_END_NAMESPACE
