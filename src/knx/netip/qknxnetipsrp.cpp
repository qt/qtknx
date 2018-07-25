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

using namespace SrpBuilders;

/*!
    \namespace SrpBuilders

    \inmodule QtKnx
    \brief Contains various builders for creating the search request parameters
    (SRP) included in an extended search request.

    The following SRP types are available:

    \list
        \li \l {SrpBuilders::MacAddress}{Select By MAC Address} indicates that
            the KNXnet/IP client is interested only in the response from the
            KNXnet/IP server with the given MAC address.
        \li \l {SrpBuilders::ProgrammingMode}{Select By Programming Mode}
            indicates that the client is interested only in responses from
            servers in which Programming Mode is currently enabled.
        \li \l {SrpBuilders::SupportedFamily}{Select By Service} indicates that
            the client is interested only in responses from servers supporting
            the given service family in at least the given version.
        \li \l {SrpBuilders::RequestDibs}{Request DIBs} indicates that the
            client is interested in the listed description information blocks
            (DIBs).
    \endlist
*/

/*!
    \class SrpBuilders::ProgrammingMode
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The SrpBuilders::ProgrammingMode class provides the means to
    create the \e {Select By Programming Mode} SRP for the extended search
    request.

    The client includes this SRP to indicate that it is interested only in
    responses from KNXnet/IP servers in which Programming Mode is currently
    enabled. If Programming Mode is not enabled in a KNXnet/IP server, then
    the server does not respond to this search request.

    The common way to create this SRP is:

    \code
        auto srpMode = ProgrammingMode()
                       .setMandatory()
                       .create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for SRPs of the type Select By Programming Mode.
*/
ProgrammingMode::ProgrammingMode()
    : d_ptr(new ProgrammingMode::ProgrammingModePrivate)
{}

/*!
    Destroys a Select By Programming Mode SRP.
*/
ProgrammingMode::~ProgrammingMode()
{}

/*!
    Constructs a copy of \a other.
*/
ProgrammingMode::ProgrammingMode(const ProgrammingMode &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this Programming Mode builder and returns a reference.
*/
ProgrammingMode &ProgrammingMode::operator=(const ProgrammingMode &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the mandatory bit to \a value.

    \note The mandatory bit is the first significant bit of the Type Code field.
*/
ProgrammingMode &ProgrammingMode::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates a Select By Programming Mode SRP.
*/
QKnxNetIpSrp ProgrammingMode::create() const
{
    auto header = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>
                  (QKnxNetIp::SearchParameterType::SelectByProgrammingMode, 0);
    header.setMandatory(d_ptr->m_mandatory);
    return QKnxNetIpSrp(header);
}

/*!
    \class SrpBuilders::MacAddress
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The SrpBuilders::MacAddress class provides the means to
    create the \e {Select By MAC Address} SRP for the extended search
    request.

    The client includes this SRP to indicate that it is interested only in
    the response from the KNXnet/IP server with the given MAC address.
    If the KNXnet/IP server’s MAC address is different from the given MAC
    address, then it does not respond to this search request.

    The common way to create this SRP is:

    \code
        auto macAddress = QKnxByteArray::fromHex("4CCC6AE40000");
        auto srpMac = MacAddress()
                      .setMandatory()
                      .setMac(macAddress)
                      .create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for a Select By MAC Address SRP.
*/
MacAddress::MacAddress()
    : d_ptr(new MacAddressPrivate)
{}

/*!
    Destroys the builder.
*/
MacAddress::~MacAddress()
{}

/*!
    Constructs a copy of \a other.
*/
MacAddress::MacAddress(const MacAddress &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this Mac Address builder and returns a reference.
*/
MacAddress &MacAddress::operator=(const MacAddress &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the MAC address \a macAdd to be used by the builder.
*/
MacAddress &
MacAddress::setMac(const QKnxByteArray &macAdd)
{
    d_ptr->m_macAddress = macAdd;
    return *this;
}

/*!
    Sets the mandatory flag bit to \a value.

    \note The mandatory bit is the first significant bit of the Type Code field.
*/
MacAddress &MacAddress::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates a Select By MAC Address SRP.
*/
QKnxNetIpSrp MacAddress::create() const
{
    auto header = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>
                  (QKnxNetIp::SearchParameterType::SelectByMACAddress, 6);
    header.setMandatory(d_ptr->m_mandatory);
    return QKnxNetIpSrp(header, d_ptr->m_macAddress);
}

/*!
    \class SrpBuilders::SupportedFamily
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The SrpBuilders::SupportedFamily class provides the means to
    create a \e {Select By Service} SRP for an extended search request.

    The client includes this SRP to indicate that it is interested
    only in responses from KNXnet/IP servers supporting the given KNXnet/IP
    service family in at least the given version.

    If the KNXnet/IP server does not support the given service family or
    supports the given service family only in a lower version, then it does
    not respond to this search request.

    The common way to create this SRP is:

    \code
        QVector<QKnxServiceInfo> families;
        families.append({ QKnxNetIp::ServiceFamily::Core, 9 });
        families.append({ QKnxNetIp::ServiceFamily::DeviceManagement, 10 });
        families.append({ QKnxNetIp::ServiceFamily::IpTunneling, 11 });

        auto srpDibs = SupportedFamily()
            .setMandatory()
            .setServiceInfos(families)
            .create();
    \endcode

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    Constructs a builder for a Select By Service SRP of an extended search
    request.
*/
SupportedFamily::SupportedFamily()
    : d_ptr(new SupportedFamilyPrivate)
{}

/*!
    Destroys the Select By Service SRP builder.
*/
SupportedFamily::~SupportedFamily()
{}

SupportedFamily::SupportedFamily(const SupportedFamily &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this Select By Service builder and returns a reference.
*/
SupportedFamily &SupportedFamily::operator=(const SupportedFamily &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the supported service families and versions of the KNXnet/IP SRP
    structure to \a infos and returns a reference to the SRP builder.
*/
SupportedFamily &SupportedFamily::setServiceInfos(const QVector<QKnxServiceInfo> &infos)
{
    d_ptr->m_infos = infos;
    return *this;
}

/*!
    Sets the mandatory bit flag of the Type Code field to \a value.
*/
SupportedFamily &SupportedFamily::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates the Select By Service SRP.
*/
QKnxNetIpSrp SupportedFamily::create() const
{
    auto header = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>
                  (QKnxNetIp::SearchParameterType::SelectByServiceSRP, 2);
    header.setMandatory(d_ptr->m_mandatory);

    QKnxByteArray bytes;
    for (const auto &info : qAsConst(d_ptr->m_infos))
        bytes += { quint8(info.ServiceFamily), info.ServiceFamilyVersion };

    return QKnxNetIpSrp(header, bytes);
}

/*!
    \class SrpBuilders::RequestDibs
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The SrpBuilders::RequestDibs class provides the means to
    create the \e {Request DIBs} SRP for the extended search request.

    The client includes this SRP to indicate that it is interested in the
    listed description type information blocks (DIBs). This SRP does not
    influence the decision of the KNXnet/IP server whether or not to respond
    to the search request.

    This SRP has variable length. If the client is interested in an odd number
    of DIBs, it adds an additional description type
    \l {QKnx::NetIp::DescriptionType} {QKnxNetIp::DescriptionType::Unknown}
    to make the structure length even.

    The common way to create this SRP is:

    \code
        auto srpDibs = RequestDibs()
            .setMandatory()
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
RequestDibs::RequestDibs()
    : d_ptr(new RequestDibs::RequestDibsPrivate)
{}

/*!
    Destroys the Request DIBs SRP builder.
*/
RequestDibs::~RequestDibs()
{}

RequestDibs::RequestDibs(const RequestDibs &other)
    : d_ptr(other.d_ptr)
{}

RequestDibs &RequestDibs::operator=(const RequestDibs &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Sets the requested description types of the KNXnet/IP SRP structure to
    \a types and returns a reference to the SRP builder.
*/
RequestDibs &RequestDibs::setDescriptionTypes(const QVector<QKnxNetIp::DescriptionType> &types)
{
    d_ptr->m_types = types;
    return *this;
}

/*!
    Sets the mandatory flag to \a value.
*/
RequestDibs &RequestDibs::setMandatory(bool value)
{
    d_ptr->m_mandatory = value;
    return *this;
}

/*!
    Creates a Request Dibs SRP.
*/
QKnxNetIpSrp RequestDibs::create() const
{
    const auto &types = d_ptr->m_types;
    QKnxByteArray bytes((types.size() % 2) == 0 ? types.size() : types.size() + 1, 0x00);
    for (int i = 0; i < types.size(); ++i)
        bytes.set(i, quint8(types[i]));

    auto header = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>
                  (QKnxNetIp::SearchParameterType::RequestDIBs, bytes.size());
    header.setMandatory(d_ptr->m_mandatory);

    return QKnxNetIpSrp(header, bytes);
}

QT_END_NAMESPACE
