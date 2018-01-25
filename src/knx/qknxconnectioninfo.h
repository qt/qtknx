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

#ifndef QKNXCONNECTIONINFO_H
#define QKNXCONNECTIONINFO_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnamespace.h>

QT_BEGIN_NAMESPACE

class QKnxConnectionInfoPrivate;
class Q_KNX_EXPORT QKnxConnectionInfo
{
public:
    QKnxConnectionInfo();
    virtual ~QKnxConnectionInfo();

    virtual QKnx::MediumType mediumType() const;

    QKnxConnectionInfo(const QKnxConnectionInfo &other);
    QKnxConnectionInfo &operator=(const QKnxConnectionInfo &other);

#ifdef Q_COMPILER_RVALUE_REFS
    void swap(QKnxConnectionInfo &other) Q_DECL_NOTHROW;
    QKnxConnectionInfo &operator=(QKnxConnectionInfo &&other) Q_DECL_NOTHROW;
#endif

private:
    explicit QKnxConnectionInfo(QKnxConnectionInfoPrivate &dd);

private:
    QSharedDataPointer<QKnxConnectionInfoPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif
