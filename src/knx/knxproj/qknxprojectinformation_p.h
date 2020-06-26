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

#ifndef QKNXPROJECTINFORMATION_P_H
#define QKNXPROJECTINFORMATION_P_H

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
#include <QtKnx/private/qknxprojectutils_p.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxHistoryEntry
{
    Q_DECLARE_TR_FUNCTIONS(QKnxHistoryEntry)

public:
    QDateTime Date;
    QString User; // optional, 50 character max.
    QString Text;
    QString Detail; // optional

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxToDoItem
{
    Q_DECLARE_TR_FUNCTIONS(QKnxToDoItem)

public:
    QString Description;
    QString ObjectPath; // optional
    QString Status; // Open, Accomplished

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxProjectTrace
{
    Q_DECLARE_TR_FUNCTIONS(QKnxProjectTrace)

public:
    QDateTime Date;
    QString UserName;
    QString Comment;

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxDeviceCertificate
{
    Q_DECLARE_TR_FUNCTIONS(QKnxDeviceCertificate)

public:
    QByteArray SerialNumber; // Base64 encoded
    QString FDSK; // 40 character max.

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxUserFile
{
    Q_DECLARE_TR_FUNCTIONS(QKnxUserFile)

public:
    QString Filename; // 255 character max.
    QString Comment; // optional

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxProjectInformation
{
    Q_DECLARE_TR_FUNCTIONS(QKnxProjectInformation)

public:
    QString Name; // 50 character max.
    QString GroupAddressStyle; // TwoLevel, ThreeLevel, Free
    QString ProjectNumber; // optional, 50 character max.
    QString ContractNumber; // optional, 50 character max.
    QDateTime LastModified; // optional
    QDateTime ProjectStart; // optional
    QDateTime ProjectEnd; // optional
    quint16 ProjectId { 0 }; // optional, value max. 4095
    QString ProjectPassword; // optional, 20 character max
    QString Comment; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    // optional, None, OperationUsed, Detailed
    QString ProjectTracingLevel { QLatin1String("None") };

    QString ProjectTracingPassword; // optional, 20 character max
    bool Hide16BitGroupsFromLegacyPlugins { false }; // optional
    QString CodePage; // optional, US-ASCII, UTF8, ISO-8859-1 -> ISO-8859-15
    bool BusAccessLegacyMode { false }; // optional
    QString Guid;
    quint32 LastUsedPuid { 0 };
    quint32 DeviceCount { 0 }; // (optional) not declared inside the knx_project.xsd

    QList<QKnxHistoryEntry> HistoryEntries; // 0..n
    QList<QKnxToDoItem> ToDoItems; // 0..n
    QList<QKnxProjectTrace> ProjectTraces; // 0..n
    QList<QKnxDeviceCertificate> DeviceCertificates; // 0..n
    QList<QKnxUserFile> UserFiles; // 0..n, (optional) not declared inside the knx_project.xsd

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
