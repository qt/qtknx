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

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxgroupaddressinfo.h>

QT_BEGIN_NAMESPACE

class QKnxGroupAddressInfosPrivate;
class Q_KNX_EXPORT QKnxGroupAddressInfos final
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupAddressInfos)

public:
    enum class Status : quint8
    {
        NoError,
        FileError,
        ProjectError,
        ParseError
    };

    QKnxGroupAddressInfos();
    ~QKnxGroupAddressInfos();

    explicit QKnxGroupAddressInfos(const QString &projectFile);

    QString projectFile() const;
    void setProjectFile(const QString &projectFile);

    bool parse();
    void clear();

    Status status() const;
    QString errorString() const;

    QList<QString> projectIds() const;
    QString projectName(const QString &projectId) const;

    QList<QString> installations(const QString &projectId) const;

    qint32 infoCount(const QString &projectId, const QString &installation = {}) const;

    QList<QKnxGroupAddressInfo> addressInfos(const QString &projectId,
        const QString &installation = {}) const;
    QList<QKnxGroupAddressInfo> addressInfos(const QKnxAddress &address,
        const QString &projectId, const QString &installation = {}) const;
    QList<QKnxGroupAddressInfo> addressInfos(QKnxDatapointType::Type type,
        const QString &projectId, const QString &installation = {}) const;

    void add(const QKnxGroupAddressInfo &info, const QString &projectId);
    void add(const QString &name, const QKnxAddress &address, QKnxDatapointType::Type type,
        const QString &description, const QString &projectId, const QString &installation = {});
    void add(const QString &name, const QKnxAddress &address, const QString &datapointType,
        const QString &description, const QString &projectId, const QString &installation = {});

    void remove(const QKnxAddress &address, const QString &projectId,
        const QString &installation = {});
    void remove(const QKnxGroupAddressInfo &info, const QString &projectId);

    QKnxGroupAddressInfos(const QKnxGroupAddressInfos &other);
    QKnxGroupAddressInfos &operator=(const QKnxGroupAddressInfos &other);

    QKnxGroupAddressInfos(QKnxGroupAddressInfos &&other) Q_DECL_NOTHROW;
    QKnxGroupAddressInfos &operator=(QKnxGroupAddressInfos &&other) Q_DECL_NOTHROW;

    void swap(QKnxGroupAddressInfos &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxGroupAddressInfos &other) const;
    bool operator!=(const QKnxGroupAddressInfos &other) const;

private:
    explicit QKnxGroupAddressInfos(QKnxGroupAddressInfosPrivate &dd);

private:
    QSharedDataPointer<QKnxGroupAddressInfosPrivate> d_ptr;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxGroupAddressInfos &infos);

QT_END_NAMESPACE

#endif // QKNXGROUPADDRESSINFOS_H
