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

#ifndef QKNXGROUPADDRESSINFOS_H
#define QKNXGROUPADDRESSINFOS_H

#include <QtCore/qhash.h>
#include <QtCore/qlist.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxgroupaddressinfo.h>
#include <QtKnx/qknxinstallation.h>
#include <QtKnx/qknxprojectroot.h>

QT_BEGIN_NAMESPACE

class QKnxGroupAddressInfosPrivate;

class Q_KNX_EXPORT QKnxGroupAddressInfos
{
public:
    QKnxGroupAddressInfos();
    ~QKnxGroupAddressInfos();

    explicit QKnxGroupAddressInfos(const QString &knxProjectFile, const QString &projectId);

    // TODO: adapt this ReadStatus to want is needed
    enum ReadStatus {
        NoError,
        FileReadError,
        FileNameError,
        FileProjectReadError,
        FileProjectSizeError,
        FileProjectError,
        FileInstallationSizeError,
        FileGroupAddressesSizeError,
        FileGroupRangesSizeError,
        FileGroupAddressSizeError,
        FileOpenError,
        FilePermissionsError,
        FileError
    };

    ReadStatus readStatus() const;

    ReadStatus readFile(const QString &knxProjectFile, const QString &projectId);
    ReadStatus readFile();
    void clearInfos();

    QString knxProjectFileName() const;
    QString knxProjectId() const;
    QList<QString> installationsName() const;

    QMultiHash<QKnxAddress, QKnxGroupAddressInfo>
        groupAddressesFullDescription(const QString &installationName) const;

    QMultiHash<QKnxDatapointType::Type, QKnxAddress>
        datapointTypeGroupAddresses(const QString &installationName) const;

    QKnxDatapointType::Type dptType(const QKnxAddress &groupAddress,
        const QString &installationName) const;

    QList<QKnxAddress> groupAddresses(const QString &installationName) const;

    QList<QKnxAddress> groupAddresses(const QKnxDatapointType::Type &dptType,
        const QString &installationName) const;

    QKnxGroupAddressInfo groupAddressInfo(const QKnxAddress &groupAddress,
        const QString &installationName) const;

    QKnxGroupAddressInfos(const QKnxGroupAddressInfos &o);
    QKnxGroupAddressInfos &operator=(const QKnxGroupAddressInfos &o);

    bool addEntry(const QKnxGroupAddressInfo &list,
        const QString &installationName = QLatin1String(""), bool overwrite = false);

private:
    QKnxGroupAddressInfosPrivate *d { nullptr };
};

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxGroupAddressInfos &groupAddressesInfo);

QT_END_NAMESPACE

#endif // QKNXGROUPADDRESSINFOS_H
