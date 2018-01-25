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

#include "qknxdeviceconfiguration.h"

QT_BEGIN_NAMESPACE

class QKnxDeviceConfigurationPrivate : public QSharedData
{
public:
    QKnxDeviceConfigurationPrivate() = default;
    ~QKnxDeviceConfigurationPrivate() = default;

    QKnx::MediumType m_mt { QKnx::MediumType::Unknown };
    QKnxAddress m_ia { QKnxAddress::Type::Individual, 0x0000u };
};


QKnxDeviceConfiguration::QKnxDeviceConfiguration()
    : d_ptr(new QKnxDeviceConfigurationPrivate)
{}

QKnxDeviceConfiguration::~QKnxDeviceConfiguration()
{}

QKnx::MediumType QKnxDeviceConfiguration::mediumType() const
{
    return d_ptr->m_mt;
}

QKnxAddress QKnxDeviceConfiguration::individualAddress() const
{
    return d_ptr->m_ia;
}

void QKnxDeviceConfiguration::setIndividalAddress(const QKnxAddress &ia)
{
    d_ptr->m_ia = ia;
}

QKnxDeviceConfiguration::QKnxDeviceConfiguration(const QKnxDeviceConfiguration &other)
    : d_ptr(other.d_ptr)
{}

QKnxDeviceConfiguration &QKnxDeviceConfiguration::operator=(const QKnxDeviceConfiguration &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS

void QKnxDeviceConfiguration::swap(QKnxDeviceConfiguration &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

QKnxDeviceConfiguration &
    QKnxDeviceConfiguration::operator=(QKnxDeviceConfiguration &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

#endif

QKnxDeviceConfiguration::QKnxDeviceConfiguration(QKnxDeviceConfigurationPrivate &dd)
    : d_ptr(new QKnxDeviceConfigurationPrivate(dd))
{}

QT_END_NAMESPACE
