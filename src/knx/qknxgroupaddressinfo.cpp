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

#include "qknxgroupaddressinfo.h"
#include "qknxgroupaddresses.h"

QT_BEGIN_NAMESPACE

//-- QKnxGroupAddressInfoPrivate

class QKnxGroupAddressInfoPrivate final : public QSharedData
{
public:
    QKnxGroupAddressInfoPrivate() = default;
    ~QKnxGroupAddressInfoPrivate() = default;

    QKnxAddress address;
    QString installation, name, description;
    QKnxDatapointType::Type type = QKnxDatapointType::Type::Unknown;
};


/*!
    \class QKnxGroupAddressInfo

    \inmodule QtKnx
    \brief The QKnxGroupAddressInfo class contains information about a single
    KNX group address object used inside a KNX installation.

    The information contained in this class corresponds to the information
    described by the GroupRange_t/GroupAddress XML element in the KNX
    Project-Schema XML file.

    \note Not all GroupRange_t/GroupAddress attributes are reflected by the API.
*/

/*!
    Creates a new empty group address info object.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo()
    : d_ptr(new QKnxGroupAddressInfoPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxGroupAddressInfo::~QKnxGroupAddressInfo()
{}

/*!
    Creates a new group address info object and sets the \a installation,
    \a name, KNX group \a address, the corresponding datapoint \a type, and
    the object \a description.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QString &installation, const QString &name,
        const QKnxAddress &address, QKnxDatapointType::Type type, const QString &description)
    : d_ptr(new QKnxGroupAddressInfoPrivate)
{
    d_ptr->installation = installation;
    d_ptr->name = name;
    d_ptr->description = description;
    d_ptr->address = address;
    d_ptr->type = type;
}

/*!
    Creates a new group address info object and sets the \a installation,
    \a name, KNX group \a address, the corresponding datapoint type
    \a datapointType, and the object \a description.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QString &installation, const QString &name,
        const QKnxAddress &address, const QString &datapointType, const QString &description)
    : QKnxGroupAddressInfo(installation, name, address, QKnxDatapointType::toType(datapointType),
        description)
{}

/*!
    Returns \c true if the object is non-empty and valid; otherwise returns
    \c false.

    A valid object has a KNX address of the type \l {QKnxAddress::Type}
    {QKnxAddress::Group} set.
*/
bool QKnxGroupAddressInfo::isValid() const
{
    return d_ptr->address.isValid() && d_ptr->address.type() == QKnxAddress::Type::Group;
}

/*!
    Creates a new group address info object and sets the \a installation,
    \a name, KNX group \a address, the corresponding datapoint \a type, and
    the object \a description.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QString &installation, const QString &name,
        quint16 address, QKnxDatapointType::Type type, const QString &description)
    : QKnxGroupAddressInfo(installation, name, { QKnxAddress::Type::Group, quint16(address) },
        type, description)
{}

/*!
    Creates a new group address info object and sets the \a installation,
    \a name, KNX group \a address, the corresponding datapoint type
    \a datapointType, and the object \a description.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QString &installation, const QString &name,
        quint16 address, const QString &datapointType, const QString &description)
    : QKnxGroupAddressInfo(installation, name, { QKnxAddress::Type::Group, quint16(address) },
        QKnxDatapointType::toType(datapointType), description)
{}

/*!
    Returns the name of the installation this group address info object belongs
    to. The value can be empty.
*/
QString QKnxGroupAddressInfo::installation() const
{
    return d_ptr->installation;
}

/*!
    Sets the name of the \a installation this group address info object belongs
    to. The value can be empty.
*/
void QKnxGroupAddressInfo::setInstallation(const QString &installation)
{
    d_ptr->installation = installation;
}

/*!
    Returns the name of this group address info object. The value can be empty.
*/
QString QKnxGroupAddressInfo::name() const
{
    return d_ptr->name;
}

/*!
    Sets the \a name of this group address info object. The value can be empty.
*/
void QKnxGroupAddressInfo::setName(const QString &name)
{
    d_ptr->name = name;
}

/*!
    Returns the KNX address of this group address info object.
*/
QKnxAddress QKnxGroupAddressInfo::address() const
{
    return d_ptr->address;
}

/*!
    Sets the KNX \a address of this group address info object. The address
    must be of type \l {QKnxAddress::Type}{QKnxAddress::Group} to keep the
    object valid.

    \sa isValid
*/
void QKnxGroupAddressInfo::setAddress(const QKnxAddress &address)
{
    d_ptr->address = address;
}

/*!
    Returns the datapoint type of this group address info object. If the
    datapoint is not set, it will return \l {QKnxDatapointType::Unknown}.
*/
QKnxDatapointType::Type QKnxGroupAddressInfo::datapointType() const
{
    return d_ptr->type;
}

/*!
    Sets the datapoint \a type of this group address info object.
*/
void QKnxGroupAddressInfo::setDatapointType(QKnxDatapointType::Type type)
{
    d_ptr->type = type;
}

/*!
    Returns the description of this group address info object. The value can
    be empty.
*/
QString QKnxGroupAddressInfo::description() const
{
    return d_ptr->description;
}

/*!
    Sets the description of this group address info object to \a description.
    The value can be empty.
*/
void QKnxGroupAddressInfo::setDescription(const QString &description)
{
    d_ptr->description = description;
}

/*!
    Constructs a copy of \a other.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QKnxGroupAddressInfo &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxGroupAddressInfo &QKnxGroupAddressInfo::operator=(const QKnxGroupAddressInfo &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(QKnxGroupAddressInfo &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxGroupAddressInfo &QKnxGroupAddressInfo::operator=(QKnxGroupAddressInfo &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxGroupAddressInfo::swap(QKnxGroupAddressInfo &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxGroupAddressInfo::operator==(const QKnxGroupAddressInfo &other) const
{
    return d_ptr == other.d_ptr || [&]() -> bool {
        return d_ptr->installation == other.d_ptr->installation
            && d_ptr->name == other.d_ptr->name
            && d_ptr->description == other.d_ptr->description
            && d_ptr->address == other.d_ptr->address
            && d_ptr->type == other.d_ptr->type;
    }();
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxGroupAddressInfo::operator!=(const QKnxGroupAddressInfo &other) const
{
    return !operator==(other);
}

/*!
    \internal
*/
QKnxGroupAddressInfo::QKnxGroupAddressInfo(QKnxGroupAddressInfoPrivate &dd)
    : d_ptr(new QKnxGroupAddressInfoPrivate(dd))
{}

/*!
    \relates QKnxGroupAddressInfo

    Writes the \a info object to the \a debug stream and returns a reference to
    the stream.
*/
QDebug operator<<(QDebug debug, const QKnxGroupAddressInfo &info)
{
    QDebugStateSaver saver(debug);
    debug.resetFormat().nospace().noquote();
    debug << "QKnxGroupAddressInfo (installation=" << info.installation()
        << ", name=" << info.name()
        << ", address=" << info.address()
        << ", type=" << info.datapointType()
        << ", description=" << info.description()
        << ')';
    return debug;
}

QT_END_NAMESPACE
