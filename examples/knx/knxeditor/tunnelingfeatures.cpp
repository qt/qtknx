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
#include <QtKnx/QKnx>
#include <QtKnx/QKnxUtils>

bool validFeature(const QKnx::NetIp::ServiceType &frameType, const QKnx::InterfaceFeature &feature,
                  const QKnxByteArray &value)
{
    using ServType = QKnx::NetIp::ServiceType;
    if (frameType != ServType::TunnelingFeatureSet)
        return true;

    using FeatureType = QKnx::InterfaceFeature;
    switch (feature) {
        case FeatureType::SupportedEmiType:
        case FeatureType::HostDeviceDescriptorType0:
        case FeatureType::KnxManufacturerCode:
        case FeatureType::IndividualAddress:
        case FeatureType::MaximumApduLength:
            return value.size() == 2;
        case FeatureType::BusConnectionStatus:
        case FeatureType::InterfaceFeatureInfoServiceEnable:
            return value.size() == 1 && ((value.at(0) == 0x01) || (value.at(0) == 0x00));
        case FeatureType::ActiveEmiType:
            return value.size() == 1;
        default:
            break;
    }

    return false;
}

TunnelingFeatures::TunnelingFeatures(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TunnelingFeatures)
{
    ui->setupUi(this);

    ui->tunnelServiceType->setEnabled(false);
    ui->featureIdentifier->setEnabled(false);
    ui->featureValue->setEnabled(false);

    connect(ui->connectTunneling, &QPushButton::clicked, this, [&]() {
        ui->textOuputTunneling->append(tr("Connecting to: %1 on port: %2 protocol: %3")
            .arg(m_server.controlEndpointAddress().toString())
            .arg(m_server.controlEndpointPort()).arg(int(m_protocol)));

        m_tunnel.setLocalPort(0);
        if (ui->secureSessionCheckBox->isChecked()) {
            auto config = m_configs.value(ui->secureSessionCb->currentIndex());
            config.setKeepSecureSessionAlive(true);
            m_tunnel.setSecureConfiguration(config);
            m_tunnel.connectToHostEncrypted(m_server.controlEndpointAddress(),
                m_server.controlEndpointPort());
        } else {
            m_tunnel.connectToHost(m_server.controlEndpointAddress(),
                m_server.controlEndpointPort(), m_protocol);
        }
    });

    connect(ui->tunnelingSend, &QPushButton::clicked, this, [&]() {
        using ServType = QKnx::NetIp::ServiceType;
        ServType type = ServType(quint16(ServType::TunnelingFeatureGet));
        if (ui->tunnelServiceType->currentIndex() == 1)
            type = ServType(quint16(ServType::TunnelingFeatureSet));

        using FeatureType = QKnx::InterfaceFeature;
        FeatureType featureType = FeatureType(quint8(FeatureType::SupportedEmiType)
            + ui->featureIdentifier->currentIndex());

        QKnxByteArray bytes = QKnxByteArray::fromHex(ui->featureValue->text().toUtf8());
        QKnxNetIpFrame frame;

        if (type == ServType::TunnelingFeatureGet)
            m_tunnel.sendTunnelingFeatureGet(featureType);
        else if (type == ServType::TunnelingFeatureSet)
            m_tunnel.sendTunnelingFeatureSet(featureType, bytes);

        ui->textOuputTunneling->append(tr("Status: (%1) Messages sent.").arg(m_tunnel
            .sequenceCount(QKnxNetIpEndpointConnection::SequenceType::Send) + 1));
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
        if (feature == QKnx::InterfaceFeature::InterfaceFeatureInfoServiceEnable
            || feature == QKnx::InterfaceFeature::BusConnectionStatus) {
            auto state = QKnxSwitch::State(value.at(0));
            auto metaEnumState = QMetaEnum::fromType<QKnxSwitch::State>();
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                "Return Code (%2), Value (%3)")
            .arg(QString::fromLatin1(metaEnum.valueToKey(int(feature))))
            .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
            .arg(QString::fromLatin1(metaEnumState.valueToKey(int(state)))));
        } else if (feature == QKnx::InterfaceFeature::IndividualAddress) {
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                "Return Code (%2), Individual Address Value (%3)")
            .arg(QString::fromLatin1(metaEnum.valueToKey(int(feature))))
            .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
            .arg(QKnxAddress(QKnxAddress::Type::Individual, value).toString()));
        } else if (feature == QKnx::InterfaceFeature::KnxManufacturerCode
                   || feature == QKnx::InterfaceFeature::MaximumApduLength) {
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                "Return Code (%2), Value (%3)")
            .arg(QString::fromLatin1(metaEnum.valueToKey(int(feature))))
            .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
            .arg(QKnxUtils::QUint16::fromBytes(value)));
        } else if (feature == QKnx::InterfaceFeature::ActiveEmiType
                   || feature == QKnx::InterfaceFeature::SupportedEmiType) {
            QString str;
            auto  types = QKnx::EmiTypes(value.at(0));
            auto metaEmiType =  QMetaEnum::fromType<QKnx::EmiType>();
            if (types.testFlag(QKnx::EmiType::EMI1))
                str = QLatin1String(metaEmiType.valueToKey(int(QKnx::EmiType::EMI1)));
            if (types.testFlag(QKnx::EmiType::EMI2))
                str += "|" + QLatin1String(metaEmiType.valueToKey(int(QKnx::EmiType::EMI2)));
            if (types.testFlag(QKnx::EmiType::cEMI))
                str += "|" + QLatin1String(metaEmiType.valueToKey(int(QKnx::EmiType::cEMI)));
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                "Return Code (%2), EMI (%3)")
            .arg(QString::fromLatin1(metaEnum.valueToKey(int(feature))))
            .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
            .arg(str.isEmpty() ? QLatin1String(metaEmiType.valueToKey(int(QKnx::EmiType::Unknown)))
                : str));
        } else {
            ui->textOuputTunneling->append(tr("Received Tunneling Feature Response: Feature (%1), "
                    "Return Code (%2), Value: ")
                .arg(metaEnum.valueToKey(int(feature)))
                .arg(QString::fromLatin1(metaReturnCode.valueToKey(int(code))))
                + QLatin1String(value.toByteArray(), value.size()));
        }
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::connected, this, [&] {
        ui->connectTunneling->setEnabled(false);
        ui->disconnectTunneling->setEnabled(true);
        ui->tunnelServiceType->setEnabled(true);
        ui->featureIdentifier->setEnabled(true);
        if (ui->tunnelServiceType->currentText() == "TunnelingFeatureSet") {
            ui->featureValue->setEnabled(true);
            ui->tunnelingSend->setEnabled(!ui->featureValue->text().isEmpty());
        } else {
            ui->featureValue->setEnabled(false);
            ui->tunnelingSend->setEnabled(true);
        }
        ui->textOuputTunneling->append(tr("Successfully connected to: %1 on port: %2").arg(m_server
            .controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()));

        ui->textOuputTunneling->append("Status: Connected.");
    });

    connect(ui->disconnectTunneling, &QPushButton::clicked, this, [&]() {
        m_tunnel.disconnectFromHost();
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::disconnected, this, [&] {
        ui->connectTunneling->setEnabled(true);
        ui->disconnectTunneling->setEnabled(false);
        ui->tunnelingSend->setEnabled(false);
        ui->tunnelServiceType->setEnabled(false);
        ui->featureIdentifier->setEnabled(false);
        ui->featureValue->setEnabled(false);
        ui->textOuputTunneling->append(tr("Successfully disconnected from: %1 on port: %2\n")
            .arg(m_server.controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()));
        ui->textOuputTunneling->append("Status: Disconnected.");
    });

    connect(&m_tunnel, &QKnxNetIpTunnel::errorOccurred, this,
        [&] (QKnxNetIpEndpointConnection::Error, QString errorString) {
            ui->textOuputTunneling->append(errorString);
    });

    connect(ui->tunnelServiceType, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        if (text == QString("TunnelingFeatureSet")) {
            ui->featureValue->setEnabled(true);
            ui->textOuputTunneling->append("Status: Fill in the feature type and value fields.");
        } else {
            ui->featureValue->setEnabled(false);
            ui->textOuputTunneling->append("");
        }
        checkFeatureValue();
    });

    connect(ui->featureValue, &QLineEdit::textChanged, this, [&](const QString &) {
        checkFeatureValue();
    });

    connect(ui->featureIdentifier, &QComboBox::currentTextChanged, this, [&](const QString &) {
        checkFeatureValue();
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

    updateSecureConfigCombo();
    ui->tunnelingSend->setEnabled(false);
    ui->textOuputTunneling->append("Status: Start by clicking connect.");
}

void TunnelingFeatures::setTcpEnable(bool value)
{
    m_protocol = (value ? QKnxNetIp::HostProtocol::TCP_IPv4 : QKnxNetIp::HostProtocol::UDP_IPv4);
}

void TunnelingFeatures::onKeyringChanged(const QList<QKnxNetIpSecureConfiguration> &configs)
{
    m_configs = configs;
    updateSecureConfigCombo();
}

void TunnelingFeatures::checkFeatureValue()
{
    if (!ui->featureValue->isEnabled()) {
        ui->textOuputTunneling->append("");
        ui->tunnelingSend->setEnabled(m_tunnel.state() == QKnxNetIpEndpointConnection::State::Connected);
        return;
    }

    using ServType = QKnx::NetIp::ServiceType;
    ServType type = ServType(quint16(ServType::TunnelingFeatureGet));
    if (ui->tunnelServiceType->currentIndex() == 1)
        type = ServType(quint16(ServType::TunnelingFeatureSet));

    using FeatureType = QKnx::InterfaceFeature;
    FeatureType featureType = FeatureType(quint8(FeatureType::SupportedEmiType)
        + ui->featureIdentifier->currentIndex());

    QKnxByteArray bytes = QKnxByteArray::fromHex(ui->featureValue->text().toUtf8());
    auto text = ui->featureValue->text();
    if (text.isEmpty() || !validFeature(type, featureType, bytes)
        || ((text.size() % 2) != 0)) {
        ui->textOuputTunneling->append("Status: Invalid value entered");
        ui->tunnelingSend->setEnabled(false);
        return;
    }
    ui->textOuputTunneling->append("Status: Valid value entered, click send.");
    ui->tunnelingSend->setEnabled(m_tunnel.state() == QKnxNetIpEndpointConnection::State::Connected);
}

void TunnelingFeatures::updateSecureConfigCombo()
{
    ui->secureSessionCb->clear();

    ui->secureSessionCheckBox->setEnabled(!m_configs.isEmpty());
    ui->secureSessionCheckBox->setChecked(m_protocol == QKnxNetIp::HostProtocol::TCP_IPv4);

    for (int i = 0; i < m_configs.size(); ++i) {
        const auto &config = m_configs[i];
        if (m_server.individualAddress() != config.host())
            continue;

        const auto ia = config.individualAddress();
        ui->secureSessionCb->addItem(tr("User ID: %1 (Individual Address: %2)")
            .arg(config.userId())
            .arg(ia.isValid() ? ia.toString() : tr("No specific address")), i);
    }
}
