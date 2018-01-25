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

#ifndef QKNXPROJECTROOT_H
#define QKNXPROJECTROOT_H

#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxinstallation.h>
#include <QtKnx/qknxprojectinformation.h>
#include <QtKnx/qknxprojectutils.h>

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

    QVector<QKnxProjectInformation> ProjectInformation; // 0..1
    QVector<QKnxInstallation> Installations; // 1..16
    QVector<QKnxUserFile> UserFiles; // 0..n
    QVector<QKnxAddinData> AddinData; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxProjectRoot
{
    Q_DECLARE_TR_FUNCTIONS(QKnxProjectRoot)

public:
    QString CreatedBy; // optional
    QString ToolVersion; // optional

    QVector<QKnxProject> Project; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
