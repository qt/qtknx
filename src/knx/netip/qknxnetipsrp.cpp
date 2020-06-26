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
#include "qknxnetipservicefamiliesdib.h"
#include "qknxnetipsrp.h"
#include "qknxnetipstruct.h"
#include "qknxnetipstructheader.h"

#include <QtCore/qshareddata.h>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSrpProxy

    \since 5.12
    \inmodule QtKnx

    \brief The QKnxNetIpSrpProxy class encapsulates methods to introspect the
    various search request parameter (SRP) structures and provides a collection
    of builders for creating such SRP structures to be included in an extended
    search request.

    \note When using QKnxNetIpSrpProxy, care must be taken to ensure that the
    referenced KNXnet/IP SRP structure outlives the proxy on all code paths,
    lest the proxy ends up referencing deleted data.

    The following SRP types are available:
    \list
        \li \l {QKnxNetIpSrpProxy::MacAddress}{Select By MAC Address}
            indicates that the KNXnet/IP client is interested only in the
            response from the KNXnet/IP server with the given MAC address.
        \li \l {QKnxNetIpSrpProxy::ProgrammingMode}{Select By Programming Mode}
            indicates that the client is interested only in responses from
            servers in which Programming Mode is currently enabled.
        \li \l {QKnxNetIpSrpProxy::SupportedFamily}{Select By Service}
            indicates that the client is interested only in responses from
            servers supporting the given service family in at least the given
            version.
        \li \l {QKnxNetIpSrpProxy::RequestDibs}{Request DIBs} indicates that
            the client is interested in the listed description information blocks
            (DIBs).
    \endlist

    Reading the supported service families can be achieved like this:
    \code
        auto srp = QKnxNetIpSrp::fromBytes(...);

        QKnxNetIpSrpProxy proxy(srp);
        if (!proxy.isValid())
            return;

        if (proxy().searchParameterType() != QKnxNetIp::SearchParameterType::SelectByMACAddress)
            return;

        auto mandatory = proxy.isMandatory();
        auto macAddress = proxy.macAddress();
    \endcode

    \sa programmingModeBuilder(), macAddressBuilder(), supportedFamilyBuilder(),
    requestDibsBuilder()
*/

/*!
    \internal
    \fn QKnxNetIpSrpProxy::QKnxNetIpSrpProxy()
*/

/*!
    \internal
    \fn QKnxNetIpSrpProxy::~QKnxNetIpSrpProxy()
*/

/*!
    \internal
    \fn QKnxNetIpSrpProxy::QKnxNetIpSrpProxy(const QKnxNetIpSrp &&)
*/

/*!
    Constructs a proxy object with the specified KNXnet/IP SRP structure
    \a srp to read the encapsulated search request parameters.
*/
QKnxNetIpSrpProxy::QKnxNetIpSrpProxy(const QKnxNetIpSrp &srp)
    : m_srp(srp)
{}

/*!
    Returns \c true if the KNXnet/IP structure to create the object is a valid
    KNXnet/IP SRP structure; otherwise returns \c false.
*/
bool QKnxNetIpSrpProxy::isValid() const
{
    if (m_srp.code() == QKnxNetIp::SearchParameterType::SelectByProgrammingMode)
        return m_srp.isValid() && m_srp.size() == 2;

    if (m_srp.code() == QKnxNetIp::SearchParameterType::SelectByMACAddress)
        return m_srp.isValid() && m_srp.size() == 8;

    if (m_srp.code() == QKnxNetIp::SearchParameterType::SelectByService)
        return m_srp.isValid() && m_srp.size() == 4;

    if (m_srp.code() == QKnxNetIp::SearchParameterType::RequestDIBs) // must be even sized
        return m_srp.isValid() && (m_srp.size() >= 4) && (m_srp.size() % 2 == 0);

    return false;
}

/*!
    Returns \c true if the mandatory bit is set; otherwise returns \c false.
*/
bool QKnxNetIpSrpProxy::isMandatory() const
{
    return m_srp.header().isMandatory();
}

/*!
    Return the search parameter type from KNXnet/IP structure if the object
    passed during construction was valid, otherwise returns QKnx::NetIp::Unknown.
*/
QKnxNetIp::SearchParameterType QKnxNetIpSrpProxy::searchParameterType() const
{
    if (isValid())
        return m_srp.code();
    return QKnxNetIp::SearchParameterType::Unknown;
}

/*!
    Returns \c true if the search request was limited to devices in programming
    mode; otherwise returns \c false.
*/
bool QKnxNetIpSrpProxy::programmingModeOnly() const
{
    return isValid() && (m_srp.code() == QKnxNetIp::SearchParameterType::SelectByProgrammingMode);
}

/*!
    Returns an array of bytes that represent the MAC address used as
    search criteria if the object that was passed during construction
    was valid; otherwise returns an empty byte array.
*/
QKnxByteArray QKnxNetIpSrpProxy::macAddress() const
{
    if (isValid() && m_srp.code() == QKnxNetIp::SearchParameterType::SelectByMACAddress)
        return m_srp.constData();
    return {};
}

/*!
    Returns a QKnxServiceInfo structure used as search criteria if the
    object that was passed during construction was valid; otherwise returns
    a \l {default-constructed value}.
*/
QKnxServiceInfo QKnxNetIpSrpProxy::serviceInfo() const
{
    if (isValid() && m_srp.code() == QKnxNetIp::SearchParameterType::SelectByService) {
        const auto &data = m_srp.constData();
        return { QKnxNetIp::ServiceFamily(data.value(0)), data.value(1) };
    }
    return {};
}

/*!
    Returns a list of QKnx::NetIp::DescriptionType enumeration values used as
    search criteria if the object that was passed during construction was valid;
    otherwise returns an empty list.
*/
QList<QKnxNetIp::DescriptionType> QKnxNetIpSrpProxy::descriptionTypes() const
{
    QList<QKnxNetIp::DescriptionType> types;
    if (isValid() && m_srp.code() == QKnxNetIp::SearchParameterType::RequestDIBs) {
        const auto &data = m_srp.constData();
        for (quint16 i = 0; i < m_srp.dataSize(); i++)
            types.append(QKnxNetIp::DescriptionType(data.value(i)));
    }
    return types;
}


/*!
    \class QKnxNetIpSrpProxy::ProgrammingMode

    \since 5.12
    \inmodule QtKnx
    \inheaderfile QKnxNetIpSrpProxy

    \brief The QKnxNetIpSrpProxy::ProgrammingMode class provides the means to
    create the \e {Select By Programming Mode} SRP for the extended search
    request.

    The client includes this SRP to indicate that it is interested only in
    responses from KNXnet/IP servers in which Programming Mode is currently
    enabled. If Programming Mode is not enabled in a KNXnet/IP server, then
    the server does not respond to this search request.

    \note By default the mandatory flag is set to \c true.

    The common way to create this SRP is:

    \code
        auto srpMode = QKnxNetIpSrpProxy::programmingModeBuilder()
            .setMandatory(true)
            .create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for SRPs of the type Select By Programming Mode.
*/
QKnxNetIpSrpProxy::ProgrammingMode::ProgrammingMode()
    : d_ptr(new ProgrammingModePrivate)
{}

/*!
    Destroys a Select By Programming Mode SRP.
*/
QKnxNetIpSrpProxy::ProgrammingMode::~ProgrammingMode()
{}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSrpProxy::ProgrammingMode::ProgrammingMode(const ProgrammingMode &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this Programming Mode builder and returns a reference.
*/
QKnxNetIpSrpProxy::ProgrammingMode &
    QKnxNetIpSrpProxy::ProgrammingMode::operator=(const ProgrammingMode &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the mandatory bit to \a value.

    \note The mandatory bit is the first significant bit of the Type Code field.
*/
QKnxNetIpSrpProxy::ProgrammingMode &QKnxNetIpSrpProxy::ProgrammingMode::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates a Select By Programming Mode SRP.
*/
QKnxNetIpSrp QKnxNetIpSrpProxy::ProgrammingMode::create() const
{
    return { { QKnxNetIp::SearchParameterType::SelectByProgrammingMode, 0, d_ptr->m_mandatory } };
}

/*!
    Returns a builder object to create a KNXnet/IP programming mode SRP
    structure.
*/
QKnxNetIpSrpProxy::ProgrammingMode QKnxNetIpSrpProxy::programmingModeBuilder()
{
    return QKnxNetIpSrpProxy::ProgrammingMode();
}


/*!
    \class QKnxNetIpSrpProxy::MacAddress

    \since 5.12
    \inmodule QtKnx
    \inheaderfile QKnxNetIpSrpProxy

    \brief The QKnxNetIpSrpProxy::MacAddress class provides the means to
    create the \e {Select By MAC Address} SRP for the extended search
    request.

    The client includes this SRP to indicate that it is interested only in
    the response from the KNXnet/IP server with the given MAC address.
    If the KNXnet/IP server’s MAC address is different from the given MAC
    address, then it does not respond to this search request.

    \note By default the mandatory flag is set to \c true.

    The common way to create this SRP is:

    \code
        auto macAddress = QKnxByteArray::fromHex("4CCC6AE40000");
        auto srpMac = QKnxNetIpSrpProxy::macAddressBuilder()
            .setMac(macAddress)
            .create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for a Select By MAC Address SRP.
*/
QKnxNetIpSrpProxy::MacAddress::MacAddress()
    : d_ptr(new MacAddressPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpSrpProxy::MacAddress::~MacAddress()
{}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSrpProxy::MacAddress::MacAddress(const QKnxNetIpSrpProxy::MacAddress &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this MAC Address builder and returns a reference.
*/
QKnxNetIpSrpProxy::MacAddress &
    QKnxNetIpSrpProxy::MacAddress::operator=(const MacAddress &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the MAC address \a macAdd to be used by the builder.
*/
QKnxNetIpSrpProxy::MacAddress &
    QKnxNetIpSrpProxy::MacAddress::setMac(const QKnxByteArray &macAdd)
{
    d_ptr->m_macAddress = macAdd;
    return *this;
}

/*!
    Sets the mandatory flag bit to \a value.

    \note The mandatory bit is the first significant bit of the Type Code field.
*/
QKnxNetIpSrpProxy::MacAddress &QKnxNetIpSrpProxy::MacAddress::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates a Select By MAC Address SRP.
*/
QKnxNetIpSrp QKnxNetIpSrpProxy::MacAddress::create() const
{
    return { { QKnxNetIp::SearchParameterType::SelectByMACAddress, 6, d_ptr->m_mandatory },
        d_ptr->m_macAddress };
}

/*!
    Returns a builder object to create a KNXnet/IP MAC address SRP
    structure.
*/
QKnxNetIpSrpProxy::MacAddress QKnxNetIpSrpProxy::macAddressBuilder()
{
    return QKnxNetIpSrpProxy::MacAddress();
}


/*!
    \class QKnxNetIpSrpProxy::SupportedFamily

    \since 5.12
    \inmodule QtKnx
    \inheaderfile QKnxNetIpSrpProxy

    \brief The QKnxNetIpSrpProxy::SupportedFamily class provides the means to
    create a \e {Select By Service} SRP for an extended search request.

    The client includes this SRP to indicate that it is interested
    only in responses from KNXnet/IP servers supporting the given KNXnet/IP
    service family in at least the given version.

    If the KNXnet/IP server does not support the given service family or
    supports the given service family only in a lower version, then it does
    not respond to this search request.

    \note By default the mandatory flag is set to \c true.

    The common way to create this SRP is:

    \code
        auto srpDibs = QKnxNetIpSrpProxy::supportedFamilyBuilder()
            .setServiceInfo({ QKnxNetIp::ServiceFamily::Core, 2 })
            .create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for a Select By Service SRP of an extended search
    request.
*/
QKnxNetIpSrpProxy::SupportedFamily::SupportedFamily()
    : d_ptr(new SupportedFamilyPrivate)
{}

/*!
    Destroys the Select By Service SRP builder.
*/
QKnxNetIpSrpProxy::SupportedFamily::~SupportedFamily()
{}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSrpProxy::SupportedFamily::SupportedFamily(const SupportedFamily &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this Select By Service builder and returns a reference.
*/
QKnxNetIpSrpProxy::SupportedFamily &
    QKnxNetIpSrpProxy::SupportedFamily::operator=(const SupportedFamily &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the supported service families and versions of the KNXnet/IP SRP
    structure to \a info and returns a reference to the SRP builder.
*/
QKnxNetIpSrpProxy::SupportedFamily &
    QKnxNetIpSrpProxy::SupportedFamily::setServiceInfo(const QKnxServiceInfo &info)
{
    d_ptr->m_info = info;
    return *this;
}

/*!
    Sets the mandatory bit flag of the Type Code field to \a value.
*/
QKnxNetIpSrpProxy::SupportedFamily &QKnxNetIpSrpProxy::SupportedFamily::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates the Select By Service SRP.
*/
QKnxNetIpSrp QKnxNetIpSrpProxy::SupportedFamily::create() const
{
    return { { QKnxNetIp::SearchParameterType::SelectByService, 2, d_ptr->m_mandatory },
        { quint8(d_ptr->m_info.ServiceFamily), d_ptr->m_info.ServiceFamilyVersion } };
}

/*!
    Returns a builder object to create a KNXnet/IP supported service family SRP
    structure.
*/
QKnxNetIpSrpProxy::SupportedFamily QKnxNetIpSrpProxy::supportedFamilyBuilder()
{
    return QKnxNetIpSrpProxy::SupportedFamily();
}


/*!
    \class QKnxNetIpSrpProxy::RequestDibs

    \since 5.12
    \inmodule QtKnx
    \inheaderfile QKnxNetIpSrpProxy

    \brief The QKnxNetIpSrpProxy::RequestDibs class provides the means to
    create the \e {Request DIBs} SRP for the extended search request.

    The client includes this SRP to indicate that it is interested in the
    listed device information blocks (DIBs). This SRP does not
    influence the decision of the KNXnet/IP server whether or not to respond
    to the search request.

    This SRP has variable length. If the client is interested in an odd number
    of DIBs, it adds an additional description type
    \l {QKnx::NetIp::DescriptionType} {QKnxNetIp::DescriptionType::Unknown}
    to make the structure length even.

    \note By default the mandatory flag is set to \c true.

    The common way to create this SRP is:

    \code
        auto srpDibs = QKnxNetIpSrpProxy::requestDibsBuilder()
            .setDescriptionTypes({
                QKnxNetIp::DescriptionType::DeviceInfo,
                QKnxNetIp::DescriptionType::SupportedServiceFamilies,
                QKnxNetIp::DescriptionType::ExtendedDeviceInfo,
                QKnxNetIp::DescriptionType::Unknown
            }).create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for a Request DIBs SRP.
*/
QKnxNetIpSrpProxy::RequestDibs::RequestDibs()
    : d_ptr(new RequestDibsPrivate)
{}

/*!
    Destroys the Request DIBs SRP builder.
*/
QKnxNetIpSrpProxy::RequestDibs::~RequestDibs()
{}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSrpProxy::RequestDibs::RequestDibs(const RequestDibs &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this Request DIBs builder and returns a reference.
*/
QKnxNetIpSrpProxy::RequestDibs &
    QKnxNetIpSrpProxy::RequestDibs::operator=(const RequestDibs &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the requested description types of the KNXnet/IP SRP structure to
    \a types and returns a reference to the SRP builder.
*/
QKnxNetIpSrpProxy::RequestDibs &
    QKnxNetIpSrpProxy::RequestDibs::setDescriptionTypes(const QList<QKnxNetIp::DescriptionType> &types)
{
    d_ptr->m_types = types;
    return *this;
}

/*!
    Sets the mandatory flag to \a value.
*/
QKnxNetIpSrpProxy::RequestDibs &QKnxNetIpSrpProxy::RequestDibs::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates a Request Dibs SRP.
*/
QKnxNetIpSrp QKnxNetIpSrpProxy::RequestDibs::create() const
{
    const auto &types = d_ptr->m_types;
    QKnxByteArray bytes((types.size() % 2) == 0 ? types.size() : types.size() + 1, 0x00);
    for (int i = 0; i < types.size(); ++i)
        bytes.set(i, quint8(types[i]));

    return { { QKnxNetIp::SearchParameterType::RequestDIBs, quint16(bytes.size()),
        d_ptr->m_mandatory }, bytes };
}

/*!
    Returns a builder object to create a KNXnet/IP requested DIBs SRP
    structure.
*/
QKnxNetIpSrpProxy::RequestDibs QKnxNetIpSrpProxy::requestDibsBuilder()
{
    return QKnxNetIpSrpProxy::RequestDibs();
}

QT_END_NAMESPACE
