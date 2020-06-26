/******************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#ifndef QKNXKEYRING_P_H
#define QKNXKEYRING_P_H

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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qlist.h>
#include <QtCore/qxmlstream.h>

#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

namespace QKnx { namespace Ets { namespace Keyring {

struct Q_KNX_EXPORT QKnxBackbone
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBackbone)

public:
    QString MulticastAddress; // mandatory, pattern 2(2[4-9]|3[0-9])\.((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){2}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    quint16 Latency; // mandatory, min. value 0, 8000 value max.
    QByteArray Key; // mandatory, Base64 encoded, pattern [A-Za-z0-9\+/]{21}[AQgw]==

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxInterface
{
    Q_DECLARE_TR_FUNCTIONS(QKnxInterface)

public:
    QString Type; // mandatory, Backbone, Tunneling, USB

    QString Host; // optional
    QString IndividualAddress; // optional, pattern ((1[0-5]|[0-9])\.){2}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    quint8 UserID; // optional
    QByteArray Password; // optional, Base64 encoded, pattern [A-Za-z0-9\+/]{42}[AEIMQUYcgkosw048]=
    QByteArray Authentication; // optional, Base64 encoded, pattern [A-Za-z0-9\+/]{42}[AEIMQUYcgkosw048]=

    struct Q_KNX_EXPORT QKnxGroup
    {
        Q_DECLARE_TR_FUNCTIONS(QKnxGroup)

    public:
        quint16 Address; // mandatory
        QList<QString> Senders; // mandatory

        bool parseElement(QXmlStreamReader *reader, bool pedantic);
    };
    QList<QKnxGroup> Group; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxGroupAddresses
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupAddresses)

public:
    struct Q_KNX_EXPORT QKnxGroup
    {
        Q_DECLARE_TR_FUNCTIONS(QKnxGroup)

    public:
        quint16 Address; // mandatory
        QByteArray Key; // mandatory, pattern [A-Za-z0-9\+/]{21}[AQgw]==

        bool parseElement(QXmlStreamReader *reader, bool pedantic);
    };
    QList<QKnxGroup> Group; // 1..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxDevice
{
    Q_DECLARE_TR_FUNCTIONS(QKnxDevice)

public:
    QString IndividualAddress; //mandatory, pattern ((1[0-5]|[0-9])\.){2}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    QByteArray ToolKey; // optional, Base64 encoded, pattern [A-Za-z0-9\+/]{21}[AQgw]==
    quint48 SequenceNumber; // optional
    QByteArray ManagementPassword; // optional, Base64 encoded, pattern [A-Za-z0-9\+/]{42}[AEIMQUYcgkosw048]=
    QByteArray Authentication; // optional, Base64 encoded, pattern [A-Za-z0-9\+/]{42}[AEIMQUYcgkosw048]=

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxDevices
{
    Q_DECLARE_TR_FUNCTIONS(QKnxDevices)

public:
    QList<QKnxDevice> Device; // 0..n
    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxKeyring
{
    Q_DECLARE_TR_FUNCTIONS(QKnxKeyring)

public:
    QString Project; // mandatory, 50 character max.
    QString Created; // mandatory, xs:dateTime
    QString CreatedBy; // mandatory
    QByteArray Signature; // mandatory, Base64 encoded, pattern [A-Za-z0-9\+/]{21}[AQgw]==

    QList<QKnxBackbone> Backbone; // 0..1
    QList<QKnxInterface> Interface; // 0..n
    QList<QKnxGroupAddresses> GroupAddresses; // 0..1
    QList<QKnxDevices> Devices; // 0..1

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
    bool validate(QXmlStreamReader *reader, const QKnxByteArray &pwHash) const;
};

}}} // QKnx::Ets::Keyring

QT_END_NAMESPACE

#endif
