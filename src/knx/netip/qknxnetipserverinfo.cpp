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

#include "qknxnetipdevicedib.h"
#include "qknxnetipserverinfo.h"
#include "qknxnetipserverinfo_p.h"
#include "qknxnetiphpai.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpServerInfo

    \inmodule QtKnx
    \brief The QKnxNetIpServerInfo class stores information about a KNXnet/IP
    server.

    It contains the information provided by the server during its discovery
    (using for example \l QKnxNetIpServerDiscoveryAgent) that the client needs
    to establish a connection.
*/

QKnxNetIpServerInfo::QKnxNetIpServerInfo()
    : d_ptr(new QKnxNetIpServerInfoPrivate)
{}

QKnxNetIpServerInfo::~QKnxNetIpServerInfo()
{}

QString QKnxNetIpServerInfo::deviceName() const
{
    return QString::fromUtf8(QKnxNetIpDeviceDibView(d_ptr->hardware).deviceName());
}

QKnxAddress QKnxNetIpServerInfo::individualAddress() const
{
    return QKnxNetIpDeviceDibView(d_ptr->hardware).individualAddress();
}

quint16 QKnxNetIpServerInfo::controlEndpointPort() const
{
    return QKnxNetIpHpaiView(d_ptr->hpai).port();
}

QHostAddress QKnxNetIpServerInfo::controlEndpointAddress() const
{
    return QKnxNetIpHpaiView(d_ptr->hpai).hostAddress();
}

QVector<QKnxServiceInfo> QKnxNetIpServerInfo::supportedServices() const
{
    return QKnxNetIpServiceFamiliesDibView(d_ptr->services).serviceInfos();
}

QKnxNetIpHpai QKnxNetIpServerInfo::endpoint() const
{
    return d_ptr->hpai;
}

QKnxNetIpDib QKnxNetIpServerInfo::hardware() const
{
    return d_ptr->hardware;
}

QKnxNetIpDib QKnxNetIpServerInfo::services() const
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
        return d_ptr->hpai == other.d_ptr->hpai
            && d_ptr->hardware == other.d_ptr->hardware
            && d_ptr->services == other.d_ptr->services;
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

QKnxNetIpServerInfo::QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai, const QKnxNetIpDib &hardware,
        QKnxNetIpDib services)
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
