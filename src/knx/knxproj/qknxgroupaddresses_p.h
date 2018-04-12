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

#ifndef QKNXGROUPADDRESSES_P_H
#define QKNXGROUPADDRESSES_P_H

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

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/private/qknxprojectutils_p.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxGroupAddress
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupAddress)

public:
    QKnxGroupAddress() = default;
    QKnxGroupAddress(const QString &id, quint32 address, const QString &name, qint32 puid)
        : Id(id), Address(address), Name(name), Puid(puid)
    {}
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    quint32 Address; // min. value 1, 65535 value max.
    QString Name; // 255 character max.
    bool Unfiltered { false }; // optional
    bool Central { false }; // optional
    bool Global { false }; // optional
    QString DatapointType; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Description; // optional
    QString Comment; // optional
    qint32 Puid;
    QString Key; // optional, 40 character max.
    QString Security { QLatin1String("Auto") }; // optional, Auto, On, Off

    bool parseElement(QXmlStreamReader *reader, bool pedantic);

    bool operator==(const QKnxGroupAddress &other) const
    {
        return Id == other.Id
            && Address == other.Address
            && Name == other.Name
            && Unfiltered == other.Unfiltered
            && Central == other.Central
            && Global == other.Global
            && DatapointType == other.DatapointType
            && Description == other.Description
            && Comment == other.Comment
            && Puid == other.Puid
            && Key == other.Key
            && Security == other.Security;
    }

    bool operator!=(const QKnxGroupAddress &other) const
    {
        return !operator==(other);
    }
};

struct Q_KNX_EXPORT QKnxGroupRange
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupRange)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    quint16 RangeStart;
    quint16 RangeEnd;
    bool Unfiltered { false }; // optional
    QString Description; // optional
    QString Comment; // optional
    qint32 Puid;
    QString Security { QLatin1String("Auto") }; // optional, Auto, On, Off

    QVector<QKnxGroupRange> GroupRange; // 0..65535
    QVector<QKnxGroupAddress> GroupAddress; // 0..65535

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxGroupAddresses
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupAddresses)

public:
    QVector<QKnxGroupRange> GroupRanges; // 0..65535

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
