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

#ifndef QKNXPROJECTROOT_P_H
#define QKNXPROJECTROOT_P_H

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
#include <QtCore/qlist.h>
#include <QtKnx/private/qknxinstallation_p.h>
#include <QtKnx/private/qknxprojectinformation_p.h>
#include <QtKnx/private/qknxprojectutils_p.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxAddinData
{
    Q_DECLARE_TR_FUNCTIONS(QKnxAddinData)

public:
    QString AddinId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 50 character max.

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxProject
{
    Q_DECLARE_TR_FUNCTIONS(QKnxProject)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*

    QList<QKnxProjectInformation> ProjectInformation; // 0..1
    QList<QKnxInstallation> Installations; // 1..16
    QList<QKnxUserFile> UserFiles; // 0..n
    QList<QKnxAddinData> AddinData; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxProjectRoot
{
    Q_DECLARE_TR_FUNCTIONS(QKnxProjectRoot)

public:
    QString CreatedBy; // optional
    QString ToolVersion; // optional

    QList<QKnxProject> Project; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
