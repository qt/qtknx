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
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

struct QKnxNetIpServerInfoPrivate;

class Q_KNX_EXPORT QKnxNetIpServerInfo final
{
    friend class QKnxNetIpServerDiscoveryAgentPrivate;
    friend class QKnxNetIpServerDescriptionAgentPrivate;

public:
    QKnxNetIpServerInfo();
    ~QKnxNetIpServerInfo();

    QString deviceName() const;
    QKnxAddress individualAddress() const;

    quint16 controlEndpointPort() const;
    QHostAddress controlEndpointAddress() const;

    QVector<QKnxServiceInfo> supportedServices() const;

    QKnxNetIpHpai endpoint() const;
    QKnxNetIpDeviceDib hardware() const;
    QKnxNetIpDib services() const;

    QKnxNetIpServerInfo(const QKnxNetIpServerInfo &o);
    QKnxNetIpServerInfo &operator=(const QKnxNetIpServerInfo &o);

#ifdef Q_COMPILER_RVALUE_REFS
    QKnxNetIpServerInfo &operator=(QKnxNetIpServerInfo &&other) Q_DECL_NOTHROW;
#endif

    bool operator==(const QKnxNetIpServerInfo &other) const;
    bool operator!=(const QKnxNetIpServerInfo &other) const;

    void swap(QKnxNetIpServerInfo &other) Q_DECL_NOTHROW;

private:
    QKnxNetIpServerInfo(const QKnxNetIpHpai &hpai, const QKnxNetIpDeviceDib &hardware,
        QKnxNetIpDib services);
    explicit QKnxNetIpServerInfo(QKnxNetIpServerInfoPrivate &dd);

private:
    QExplicitlySharedDataPointer<QKnxNetIpServerInfoPrivate> d_ptr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxNetIpServerInfo)

#endif
