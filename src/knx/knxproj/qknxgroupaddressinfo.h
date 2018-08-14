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

#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxdatapointtype.h>

QT_BEGIN_NAMESPACE

class QKnxGroupAddressInfoPrivate;
class Q_KNX_EXPORT QKnxGroupAddressInfo final
{
public:
    QKnxGroupAddressInfo();
    ~QKnxGroupAddressInfo();

    QKnxGroupAddressInfo(const QString &installation, const QString &name, quint16 address,
                         QKnxDatapointType::Type type, const QString &description = {});
    QKnxGroupAddressInfo(const QString &installation, const QString &name, quint16 address,
                         const QString &datapointType, const QString &description = {});

    QKnxGroupAddressInfo(const QString &installation, const QString &name, const QKnxAddress &addr,
                         QKnxDatapointType::Type typ, const QString &description = {});
    QKnxGroupAddressInfo(const QString &installation, const QString &name, const QKnxAddress &addr,
                         const QString &datapointType, const QString &description = {});

    bool isValid() const;

    QString installation() const;
    void setInstallation(const QString &installation);

    QString name() const;
    void setName(const QString &name);

    QKnxAddress address() const;
    void setAddress(const QKnxAddress &address);

    QKnxDatapointType::Type datapointType() const;
    void setDatapointType(QKnxDatapointType::Type type);

    QString description() const;
    void setDescription(const QString &description);

    QKnxGroupAddressInfo(const QKnxGroupAddressInfo &other);
    QKnxGroupAddressInfo &operator=(const QKnxGroupAddressInfo &other);

    QKnxGroupAddressInfo(QKnxGroupAddressInfo &&other) Q_DECL_NOTHROW;
    QKnxGroupAddressInfo &operator=(QKnxGroupAddressInfo &&other) Q_DECL_NOTHROW;

    void swap(QKnxGroupAddressInfo &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxGroupAddressInfo &other) const;
    bool operator!=(const QKnxGroupAddressInfo &other) const;

private:
    explicit QKnxGroupAddressInfo(QKnxGroupAddressInfoPrivate &dd);

private:
    QSharedDataPointer<QKnxGroupAddressInfoPrivate> d_ptr;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxGroupAddressInfo &info);

QT_END_NAMESPACE

#endif // QKNXGROUPADDRESSINFO_H
