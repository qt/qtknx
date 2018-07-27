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

#ifndef QKNXNETIPEXTENDEDDEVICEDIB_H
#define QKNXNETIPEXTENDEDDEVICEDIB_H

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpExtendedDeviceDibBuilderPrivate;
class Q_KNX_EXPORT QKnxNetIpExtendedDeviceDibProxy final
{
public:
    QKnxNetIpExtendedDeviceDibProxy() = delete;
    ~QKnxNetIpExtendedDeviceDibProxy() = default;

    QKnxNetIpExtendedDeviceDibProxy(const QKnxNetIpDib &&) = delete;
    explicit QKnxNetIpExtendedDeviceDibProxy(const QKnxNetIpDib &dib);

    bool isValid() const;
    QKnxNetIp::DescriptionType descriptionType() const;

    QKnx::MediumStatus mediumStatus() const;
    quint16 maximumLocalApduLength() const;
    quint16 deviceDescriptorType0() const;

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder();
        ~Builder();

        Builder &setMediumStatus(QKnx::MediumStatus status);
        Builder &setMaximumLocalApduLength(quint16 length);
        Builder &setDeviceDescriptorType0(quint16 type);

        QKnxNetIpDib create() const;

        Builder(const Builder &other);
        Builder &operator=(const Builder &other);

    private:
        QSharedDataPointer<QKnxNetIpExtendedDeviceDibBuilderPrivate> d_ptr;
    };
    static QKnxNetIpExtendedDeviceDibProxy::Builder builder();

private:
    const QKnxNetIpDib &m_dib;
};

QT_END_NAMESPACE

#endif
