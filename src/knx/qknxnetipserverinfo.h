/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
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

    QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions supportedServices() const;

    QKnxNetIpHpai endpoint() const;
    QKnxNetIpDeviceDib hardware() const;
    QKnxNetIpServiceFamiliesDib services() const;

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
        QKnxNetIpServiceFamiliesDib services);
    explicit QKnxNetIpServerInfo(QKnxNetIpServerInfoPrivate &dd);

private:
    QExplicitlySharedDataPointer<QKnxNetIpServerInfoPrivate> d_ptr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxNetIpServerInfo)

#endif
