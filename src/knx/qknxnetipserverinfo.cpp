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

#include "qknxnetipserverinfo.h"
#include "qknxnetipserverinfo_p.h"

QT_BEGIN_NAMESPACE

QKnxNetIpServerInfo::QKnxNetIpServerInfo()
    : d_ptr(new QKnxNetIpServerInfoPrivate)
{}

QKnxNetIpServerInfo::~QKnxNetIpServerInfo()
{}

QString QKnxNetIpServerInfo::deviceName() const
{
    return QString::fromUtf8(d_ptr->hardware.deviceName());
}

QKnxAddress QKnxNetIpServerInfo::individualAddress() const
{
    return d_ptr->hardware.individualAddress();
}

quint16 QKnxNetIpServerInfo::controlEndpointPort() const
{
    return d_ptr->hpai.port();
}

QHostAddress QKnxNetIpServerInfo::controlEndpointAddress() const
{
    return d_ptr->hpai.address();
}

QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions QKnxNetIpServerInfo::supportedServices() const
{
    return d_ptr->services.serviceFamilyIdVersions();
}

QKnxNetIpHpai QKnxNetIpServerInfo::endpoint() const
{
    return d_ptr->hpai;
}

QKnxNetIpDeviceDib QKnxNetIpServerInfo::hardware() const
{
    return d_ptr->hardware;
}

QKnxNetIpServiceFamiliesDib QKnxNetIpServerInfo::services() const
{
    return d_ptr->services;
}

QKnxNetIpServerInfo::QKnxNetIpServerInfo(const QKnxNetIpServerInfo &o)
    : d_ptr(o.d_ptr)
{}

QKnxNetIpServerInfo &
QKnxNetIpServerInfo::operator=(const QKnxNetIpServerInfo &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
QKnxNetIpServerInfo &
QKnxNetIpServerInfo::operator=(QKnxNetIpServerInfo &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}
#endif

bool QKnxNetIpServerInfo::operator==(const QKnxNetIpServerInfo &other) const
{
    return d_ptr == other.d_ptr || [&]() -> bool {
        // TODO: Implement operator== in QKnxNetIpStruct classes and revisit this code.
        return d_ptr->hpai.bytes() == other.d_ptr->hpai.bytes()
            && d_ptr->hardware.bytes() == other.d_ptr->hardware.bytes()
            && d_ptr->services.bytes() == other.d_ptr->services.bytes();
    }();
}

bool QKnxNetIpServerInfo::operator!=(const QKnxNetIpServerInfo &other) const
{
    return !operator==(other);
}

void QKnxNetIpServerInfo::swap(QKnxNetIpServerInfo &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

QKnxNetIpServerInfo::QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai,
    const QKnxNetIpDeviceDib &hardware, QKnxNetIpServiceFamiliesDib services)
    : QKnxNetIpServerInfo()
{
    d_ptr->hpai = hpai;
    d_ptr->hardware = hardware;
    d_ptr->services = services;
}

QKnxNetIpServerInfo::QKnxNetIpServerInfo(QKnxNetIpServerInfoPrivate &dd)
    : d_ptr(new QKnxNetIpServerInfoPrivate(dd))
{}

QT_END_NAMESPACE
