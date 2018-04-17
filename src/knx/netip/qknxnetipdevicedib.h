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

#ifndef QKNXNETIPDEVICEDIB_H
#define QKNXNETIPDEVICEDIB_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnamespace.h>
#include <QtKnx/qknxnetipstruct.h>

#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDeviceDibView final
{
public:
    QKnxNetIpDeviceDibView() = delete;
    ~QKnxNetIpDeviceDibView() = default;

    QKnxNetIpDeviceDibView(const QKnxNetIpDib &&) = delete;
    explicit QKnxNetIpDeviceDibView(const QKnxNetIpDib &dib);

    bool isValid() const;
    QKnxNetIp::DescriptionType descriptionType() const;

    QKnx::MediumType mediumType() const;
    QKnxNetIp::ProgrammingMode deviceStatus() const;
    QKnxAddress individualAddress() const;
    quint16 projectInstallationId() const;
    QKnxByteArray serialNumber() const;
    QHostAddress multicastAddress() const;
    QKnxByteArray macAddress() const;
    QByteArray deviceName() const;

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder &setMediumType(QKnx::MediumType mediumType);
        Builder &setDeviceStatus(QKnxNetIp::ProgrammingMode mode);
        Builder &setIndividualAddress(const QKnxAddress &address);
        Builder &setProjectInstallationId(quint16 projectId);
        Builder &setSerialNumber(const QKnxByteArray &serialNumber);
        Builder &setMulticastAddress(const QHostAddress &multicastAddress);
        Builder &setMacAddress(const QKnxByteArray &macAddress);
        Builder &setDeviceName(const QByteArray &deviceName);

        QKnxNetIpDib create() const;

    private:
        QKnx::MediumType m_mediumType { QKnx::MediumType::Unknown };
        QKnxNetIp::ProgrammingMode m_progMode { QKnxNetIp::ProgrammingMode::Inactive };
        QKnxAddress m_address;
        quint16 m_projectId { 0 };
        QKnxByteArray m_serialNumber;
        QHostAddress m_multicastAddress { QLatin1String(QKnxNetIp::MulticastAddress) };
        QKnxByteArray m_macAddress;
        QByteArray m_deviceName;
    };
    static QKnxNetIpDeviceDibView::Builder builder();

private:
    const QKnxNetIpDib &m_dib;
};

QT_END_NAMESPACE

#endif
