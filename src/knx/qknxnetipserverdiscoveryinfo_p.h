/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSERVERDISCOVERYINFO_P_H
#define QKNXNETIPSERVERDISCOVERYINFO_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qshareddata.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxNetIpServerDiscoveryInfoPrivate final : public QSharedData
{
    QKnxNetIpServerDiscoveryInfoPrivate() = default;
    ~QKnxNetIpServerDiscoveryInfoPrivate() = default;

    QKnxNetIpHPAI hpai;
    QKnxNetIpDeviceDIB hardware;
    QKnxNetIpServiceFamiliesDIB services;
};

QT_END_NAMESPACE

#endif
