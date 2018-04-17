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

#ifndef QKNXINSTALLATION_P_H
#define QKNXINSTALLATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QZipReader class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/private/qknxbuildings_p.h>
#include <QtKnx/private/qknxgroupaddresses_p.h>
#include <QtKnx/private/qknxprojectutils_p.h>
#include <QtKnx/private/qknxtopology_p.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxTrade
{
    Q_DECLARE_TR_FUNCTIONS(QKnxTrade)

public:
    QString Id; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString Number; // optional, 255 character max.
    QString Comment; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString Description; // optional
    qint32 Puid;

    QVector<QKnxTrade> Trade; // 0..n
    QVector<QString> DeviceInstanceRef; // 0..n, non-colonized name, pattern [\i-[:]][\c-[:]]*

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxSplitInfo
{
    Q_DECLARE_TR_FUNCTIONS(QKnxSplitInfo)

public:
    QString ObjectPath;
    QString Cookie;

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxInstallation
{
    Q_DECLARE_TR_FUNCTIONS(QKnxInstallation)

public:
    QString Name; // 50 character max.
    quint16 InstallationId; // optional, value max. 15
    quint32 BCUKey { 4294967295 }; //optional

    // pattern ((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    QString IPRoutingMulticastAddress { QLatin1String("224.0.23.12") }; // optional

    quint8 MulticastTTL { 16 }; // optional
    QString IPRoutingBackboneKey; // optional, 40 character max.
    quint16 IPRoutingLatencyTolerance; // optional
    float IPSyncLatencyFraction { 0.1f }; // optional
    QString DefaultLine; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString IPRoutingBackboneSecurity { QLatin1String("Auto") }; // optional, Auto, On, Off
    QString SplitType; // optional, None, Master, Split

    QVector<QKnxTopology> Topology; // 1
    QVector<QKnxBuildings> Buildings; // 0..n
    QVector<QKnxGroupAddresses> GroupAddresses; // 1

    QVector<QKnxTrade> Trades; // 0..n
    QVector<QKnxBusAccess> BusAccess; // 0..1
    QVector<QKnxSplitInfo> SplitInfos; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
