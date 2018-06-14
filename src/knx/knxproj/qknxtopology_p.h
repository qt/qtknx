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

#ifndef QKNXTOPOLOGY_P_H
#define QKNXTOPOLOGY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt KNX API.  It exists for the convenience
// of the Qt KNX implementation.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/private/qknxdeviceinstance_p.h>
#include <QtKnx/private/qknxprojectutils_p.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxBusAccess
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBusAccess)

public:
    QString Name;
    QString Edi;
    QString Parameter;

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxAdditionalGroupAddress
{
    Q_DECLARE_TR_FUNCTIONS(QKnxAdditionalGroupAddress)

public:
    quint16 Address;

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxLine
{
    Q_DECLARE_TR_FUNCTIONS(QKnxLine)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    qint32 Address { 0 }; // optional, min. value 0, 15 value max.
    QString MediumTypeRefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Comment; // optional
    qint32 DomainAddress { 0 }; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString Description; // optional
    qint32 Puid { 0 };

    QVector<QKnxDeviceInstance> DeviceInstance; // 0..n
    QVector<QKnxBusAccess> BusAccess; // 0..1
    QVector<QKnxAdditionalGroupAddress> AdditionalGroupAddresses; // 0..1

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxArea
{
    Q_DECLARE_TR_FUNCTIONS(QKnxArea)

public:
    QString Id; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    qint32 Address { 0 }; // optional, min. value 0, 15 value max.
    QString Comment; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString Description; // optional
    qint32 Puid { 0 };

    QVector<QKnxLine> Line; // 0..16

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxTopology
{
    Q_DECLARE_TR_FUNCTIONS(QKnxTopology)

public:
    QVector<QKnxArea> Area; // 0..16
    QVector<QKnxDeviceInstance> UnassignedDevices; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
