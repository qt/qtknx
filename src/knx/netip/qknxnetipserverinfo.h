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

#ifndef QKNXNETIPSERVERINFO_H
#define QKNXNETIPSERVERINFO_H

#include <QtCore/qmetatype.h>
#include <QtCore/qshareddata.h>

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxnetiptunnelinginfodib.h>

#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

struct QKnxNetIpServerInfoPrivate;

class Q_KNX_EXPORT QKnxNetIpServerInfo final
{
    friend class Discoverer;
    friend class QKnxNetIpServerDiscoveryAgentPrivate;
    friend class QKnxNetIpServerDescriptionAgentPrivate;

public:
    QKnxNetIpServerInfo();
    ~QKnxNetIpServerInfo();

    QString deviceName() const;
    QKnxAddress individualAddress() const;

    quint16 controlEndpointPort() const;
    QHostAddress controlEndpointAddress() const;

    QList<QKnxServiceInfo> supportedServices() const;
    QList<QKnxNetIpTunnelingSlotInfo> tunnelingSlotInfos() const;

    QKnx::MediumStatus mediumStatus() const;
    quint16 maximumLocalApduLength() const;
    quint16 maskVersion() const;

    QKnxNetIpHpai endpoint() const;
    QKnxNetIpDib hardware() const;
    QKnxNetIpDib services() const;
    QKnxNetIpDib tunnelingInfo() const;
    QKnxNetIpDib extendedHardware() const;

    QHostAddress hostAddress() const;
    QNetworkInterface networkInterface() const;

    QKnxNetIpServerInfo(const QKnxNetIpServerInfo &other);
    QKnxNetIpServerInfo &operator=(const QKnxNetIpServerInfo &other);

    QKnxNetIpServerInfo(QKnxNetIpServerInfo &&other) Q_DECL_NOTHROW;
    QKnxNetIpServerInfo &operator=(QKnxNetIpServerInfo &&other) Q_DECL_NOTHROW;

    bool operator==(const QKnxNetIpServerInfo &other) const;
    bool operator!=(const QKnxNetIpServerInfo &other) const;

    void swap(QKnxNetIpServerInfo &other) Q_DECL_NOTHROW;

private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai, const QKnxNetIpDib &hardware,
        QKnxNetIpDib services);

    QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai, const QKnxNetIpDib &hardware,
        const QKnxNetIpDib &services, const QKnxNetIpDib &tunnelingInfo,
        const QKnxNetIpDib &extendedHardware);
#endif

    QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai,
        const QKnxNetIpDib &hardware,
        const QKnxNetIpDib &services,
        const QHostAddress &hostAddress,
        const QNetworkInterface &iinterface,
        const QKnxNetIpDib &tunnelingInfo = {},
        const QKnxNetIpDib &extendedHardware = {});

    explicit QKnxNetIpServerInfo(QKnxNetIpServerInfoPrivate &dd);

private:
    QExplicitlySharedDataPointer<QKnxNetIpServerInfoPrivate> d_ptr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxNetIpServerInfo)

#endif
