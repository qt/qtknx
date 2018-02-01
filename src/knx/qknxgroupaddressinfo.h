/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#ifndef QKNXGROUPADDRESSINFO_H
#define QKNXGROUPADDRESSINFO_H

#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxgroupaddresses.h>

QT_BEGIN_NAMESPACE

class QKnxGroupAddressInfoPrivate;
class Q_KNX_EXPORT QKnxGroupAddressInfo
{
public:
    QKnxGroupAddressInfo();
    ~QKnxGroupAddressInfo();

    QKnxGroupAddressInfo(const QKnxGroupAddress &groupAddressFromParser);
    QKnxGroupAddressInfo(const QString &id, const QString &address, const QString &name,
        const QString &description, const QString &dataPointType, const QString &puid);

    QString id() const;
    QString address() const;
    QString name() const;
    QString description() const;
    QString dataPointNumber() const;
    QString puid() const;
    QKnxAddress groupAddress() const;
    QKnxDatapointType::Type dataPointType() const;

    QKnxGroupAddressInfo(const QKnxGroupAddressInfo &o);
    QKnxGroupAddressInfo &operator=(const QKnxGroupAddressInfo &o);
    QKnxGroupAddressInfo &operator=(QKnxGroupAddressInfo &&other) Q_DECL_NOTHROW;

    bool operator==(const QKnxGroupAddressInfo &other) const;
    bool operator!=(const QKnxGroupAddressInfo &other) const;

    void swap(QKnxGroupAddressInfo &other) Q_DECL_NOTHROW;

private:
    explicit QKnxGroupAddressInfo(QKnxGroupAddressInfoPrivate &dd);

private:
    QExplicitlySharedDataPointer<QKnxGroupAddressInfoPrivate> d;
};

QT_END_NAMESPACE

#endif // QKNXGROUPADDRESSINFO_H
