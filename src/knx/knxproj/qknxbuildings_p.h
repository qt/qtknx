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

#ifndef QKNXBUILDINGS_P_H
#define QKNXBUILDINGS_P_H

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

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/private/qknxprojectutils_p.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxGroupAddressRef
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupAddressRef)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString RefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Role; // optional, 255 character max.
    qint32 Puid { 0 };

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxFunction
{
    Q_DECLARE_TR_FUNCTIONS(QKnxFunction)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString Type; // optional, Building, BuildingPart, Floor, Room, DistributionBoard, Stairway, Corridor
    QString Number; // optional, 255 character max.
    QString Comment; // optional
    QString Description; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString DefaultGroupRange; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    qint32 Puid { 0 };

    QVector<QKnxGroupAddressRef> GroupAddressRef; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxBuildingPart
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBuildingPart)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString Type; // Building, BuildingPart, Floor, Room, DistributionBoard, Stairway, Corridor
    QString Usage; // optional
    QString Number; // optional, 255 character max.

    QString Comment; // optional
    QString Description; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString DefaultLine; // optional
    qint32 Puid { 0 };

    QVector<QKnxBuildingPart> BuildingPart; // 0..n
    QVector<QString> DeviceInstanceRef; // 0..n, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QVector<QKnxFunction> Function; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxBuildings
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBuildings)

public:
    QVector<QKnxBuildingPart> BuildingPart; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
