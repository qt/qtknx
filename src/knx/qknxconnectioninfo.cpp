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

#include "qknxconnectioninfo.h"

QT_BEGIN_NAMESPACE

class QKnxConnectionInfoPrivate : public QSharedData
{
public:
    QKnxConnectionInfoPrivate() = default;
    ~QKnxConnectionInfoPrivate() = default;

    QKnx::MediumType m_mt { QKnx::MediumType::Unknown };
};


QKnxConnectionInfo::QKnxConnectionInfo()
    : d_ptr(new QKnxConnectionInfoPrivate)
{}

QKnxConnectionInfo::~QKnxConnectionInfo()
{}

QKnx::MediumType QKnxConnectionInfo::mediumType() const
{
    return d_ptr->m_mt;
}

QKnxConnectionInfo::QKnxConnectionInfo(const QKnxConnectionInfo &other)
    : d_ptr(other.d_ptr)
{}

QKnxConnectionInfo &QKnxConnectionInfo::operator=(const QKnxConnectionInfo &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

void QKnxConnectionInfo::swap(QKnxConnectionInfo &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxConnectionInfo::QKnxConnectionInfo(QKnxConnectionInfo &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}

QKnxConnectionInfo &QKnxConnectionInfo::operator=(QKnxConnectionInfo &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxConnectionInfo::operator==(const QKnxConnectionInfo &other) const
{
    return d_ptr == other.d_ptr || (d_ptr->m_mt == other.d_ptr->m_mt);
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxConnectionInfo::operator!=(const QKnxConnectionInfo &other) const
{
    return !operator==(other);
}

QKnxConnectionInfo::QKnxConnectionInfo(QKnxConnectionInfoPrivate &dd)
    : d_ptr(new QKnxConnectionInfoPrivate(dd))
{}

QT_END_NAMESPACE
