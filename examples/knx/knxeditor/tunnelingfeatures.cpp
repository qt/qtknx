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

#include "tunnelingfeatures.h"
#include "ui_tunnelingfeatures.h"

#include <QtCore/QMetaEnum>
#include <QtCore/QMetaType>
#include <QtKnx/QKnx1Bit>

TunnelingFeatures::TunnelingFeatures(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TunnelingFeatures)
{
    ui->setupUi(this);

    connect(ui->connectTunneling, &QPushButton::clicked, this, [&]() {
        ui->textOuputTunneling->append(tr("Connecting to: %1 on port: %2 protocol: %3").arg(m_server
            .controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()).arg(int(m_protocol)));
        m_tunnel.setLocalPort(0);
        m_tunnel.connectToHost(m_server.controlEndpointAddress(),
            m_server.controlEndpointPort(),
            m_protocol);
    });

    connect(ui->tunnelingSend, &QPushButton::clicked, this, [&]() {
        using ServType = QKnx::NetIp::ServiceType;
        ServType type = ServType(quint16(ServType::TunnelingFeatureGet)
            + ui->tunnelServiceType->currentIndex());
        using FeatureType = QKnx::InterfaceFeature;
        FeatureType featureType = FeatureType(quint8(FeatureType::SupportedEmiType)
            + ui->featureIdentifier->currentIndex());
        QKnx1Bit featureEnable(true);
        QKnxNetIpFrame frame;
        if (type == ServType::TunnelingFeatureGet)
            m_tunnel.sendTunnelingFeatureGet(featureType);
        else if (type == ServType::TunnelingFeatureSet)
            m_tunnel.sendTunnelingFeatureSet(featureType, featureEnable.bytes());
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::tunnelingFeatureInfoReceived, this,
        [&](QKnx::InterfaceFeature feature, const QKnxByteArray &value) {

        auto metaEnum = QMetaEnum::fromType<QKnx::InterfaceFeature>();
        if (feature == QKnx::InterfaceFeature::InterfaceFeatureInfoServiceEnable) {
            auto state = QKnxSwitch::State(value.at(0));
            auto metaEnumState = QMetaEnum::fromType<QKnxSwitch::State>();
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Info: Feature (%1), "
                "Value (%2)")
            .arg(QString::fromLatin1(metaEnum.valueToKey(int(feature))))
            .arg(QString::fromLatin1(metaEnumState.valueToKey(int(state)))));
        } else {
            tr("Received Tunneling Feature Info: Feature (%1), Value: ")
                .arg(metaEnum.valueToKey(int(feature)))
                + QLatin1String(value.toByteArray(), value.size());
        }
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::tunnelingFeatureResponseReceived, this,
        [&](QKnx::InterfaceFeature feature, QKnx::ReturnCode code, const QKnxByteArray &value) {

        auto metaEnum = QMetaEnum::fromType<QKnx::InterfaceFeature>();
        auto metaReturnCode = QMetaEnum::fromType<QKnx::ReturnCode>();
        if (feature == QKnx::InterfaceFeature::InterfaceFeatureInfoServiceEnable) {
            auto state = QKnxSwitch::State(value.at(0));
            auto metaEnumState = QMetaEnum::fromType<QKnxSwitch::State>();
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                "Return Code(%2), Value (%3)")
            .arg(QString::fromLatin1(metaEnum.valueToKey(int(feature))))
            .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
            .arg(QString::fromLatin1(metaEnumState.valueToKey(int(state)))));
        } else {
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                    "Return Code(%2), Value: ")
                .arg(metaEnum.valueToKey(int(feature)))
                .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
                + QLatin1String(value.toByteArray(), value.size()));
        }
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::connected, this, [&] {
        ui->connectTunneling->setEnabled(false);
        ui->disconnectTunneling->setEnabled(true);
        ui->tunnelingSend->setEnabled(true);

        ui->textOuputTunneling->append(tr("Successfully connected to: %1 on port: %2").arg(m_server
            .controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()));
    });

    connect(ui->disconnectTunneling, &QPushButton::clicked, this, [&]() {
        m_tunnel.disconnectFromHost();
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::disconnected, this, [&] {
        ui->connectTunneling->setEnabled(true);
        ui->disconnectTunneling->setEnabled(false);
        ui->tunnelingSend->setEnabled(false);
        ui->textOuputTunneling->append(tr("Successfully disconnected from: %1 on port: %2\n")
            .arg(m_server.controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()));
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::errorOccurred, this,
        [&] (QKnxNetIpEndpointConnection::Error, QString errorString) {
            ui->textOuputTunneling->append(errorString);
    });
}

TunnelingFeatures::~TunnelingFeatures()
{
    delete ui;
}

void TunnelingFeatures::setNatAware(bool isNatAware)
{
    m_tunnel.setNatAware(isNatAware);
}

void TunnelingFeatures::setLocalAddress(const QHostAddress &address)
{
    m_tunnel.disconnectFromHost();
    m_tunnel.setLocalAddress(address);
}

void TunnelingFeatures::setKnxNetIpServer(const QKnxNetIpServerInfo &server)
{
    m_tunnel.disconnectFromHost();
    m_server = server;

    if (m_tunnel.state() == QKnxNetIpEndpointConnection::State::Disconnected) {
        ui->connectTunneling->setEnabled(true);
        ui->disconnectTunneling->setEnabled(false);
    }
    ui->tunnelingSend->setEnabled(false);
}

void TunnelingFeatures::setTcpEnable(bool value)
{
    m_protocol = (value ? QKnxNetIp::HostProtocol::TCP_IPv4 : QKnxNetIp::HostProtocol::UDP_IPv4);
}
