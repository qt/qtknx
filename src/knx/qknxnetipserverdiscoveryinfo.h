/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSERVERDISCOVERYINFO_H
#define QKNXNETIPSERVERDISCOVERYINFO_H

#include <QtCore/qmetatype.h>
#include <QtCore/qshareddata.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

struct QKnxNetIpServerDiscoveryInfoPrivate;

class Q_KNX_EXPORT QKnxNetIpServerDiscoveryInfo final
{
    friend class QKnxNetIpServerDiscoveryAgentPrivate;

public:
    QKnxNetIpServerDiscoveryInfo();
    ~QKnxNetIpServerDiscoveryInfo();

    QString deviceName() const;
    QKnxAddress individualAddress() const;

    quint16 controlEndpointPort() const;
    QHostAddress controlEndpointAddress() const;

    QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions supportedServices() const;

    QKnxNetIpHPAI endpoint() const;
    QKnxNetIpDeviceDIB hardware() const;
    QKnxNetIpServiceFamiliesDIB services() const;

    QKnxNetIpServerDiscoveryInfo(const QKnxNetIpServerDiscoveryInfo &o);
    QKnxNetIpServerDiscoveryInfo &operator=(const QKnxNetIpServerDiscoveryInfo &o);

private:
    QKnxNetIpServerDiscoveryInfo(const QKnxNetIpHPAI &hpai, const QKnxNetIpDeviceDIB &hardware,
        QKnxNetIpServiceFamiliesDIB services);
    explicit QKnxNetIpServerDiscoveryInfo(QKnxNetIpServerDiscoveryInfoPrivate &dd);

    void swap(QKnxNetIpServerDiscoveryInfo other);

private:
    QSharedDataPointer<QKnxNetIpServerDiscoveryInfoPrivate> d_ptr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxNetIpServerDiscoveryInfo)

#endif
