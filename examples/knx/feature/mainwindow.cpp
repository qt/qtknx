/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "deviceitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < ui->communication->columnCount(); ++i)
        ui->communication->resizeColumnToContents(i);

    setupInterfaces();
    populateServiceTypesComboBox();
    populateInterfaceFeaturesComboBox();

    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    connect(ui->actionClear, &QAction::triggered, ui->communication, &QTreeWidget::clear);

    connect(&m_tunnel, &QKnxNetIpTunnel::connected, this, &MainWindow::onConnected);
    connect(&m_tunnel, &QKnxNetIpTunnel::disconnected, this, &MainWindow::onDisconnected);
    connect(&m_tunnel, &QKnxNetIpTunnel::tunnelingFeatureInfoReceived, this,
        &MainWindow::onFeatureInfoReceived);
    connect(&m_tunnel, &QKnxNetIpTunnel::tunnelingFeatureResponseReceived, this,
        &MainWindow::onFeatureResponseReceived);
    connect(&m_tunnel, &QKnxNetIpTunnel::errorOccurred, this, &MainWindow::onErrorOccurred);

    connect(ui->communication, &QTreeWidget::currentItemChanged, this,
        [&](QTreeWidgetItem *current, QTreeWidgetItem * /* previuos */) {
            m_current = current;
    });

    m_discoveryAgent.setTimeout(-1);
    m_discoveryAgent.setSearchFrequency(6);
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered, this,
        &MainWindow::onDeviceDiscovered);
    m_discoveryAgent.setDiscoveryMode(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2);

    m_discoveryAgent.start();
}

MainWindow::~MainWindow()
{
    m_discoveryAgent.stop();

    delete ui;
    ui = nullptr;
    delete m_device;
}

void MainWindow::onConnected()
{
    toggleUi(true);
    ui->connection->setText(tr("Disconnect"));

    m_last = new QTreeWidgetItem(ui->communication, m_last);
    m_last->setText(0, tr("Successfully connected to: %1 (%2 : %3)")
        .arg(m_device->info().deviceName())
        .arg(m_device->info().controlEndpointAddress().toString())
        .arg(m_device->info().controlEndpointPort()));
    m_last->setFirstColumnSpanned(true);
}

void MainWindow::onDisconnected()
{
    toggleUi(false);
    ui->connection->setText(tr("Connect"));

    m_last = new QTreeWidgetItem(ui->communication, m_last);
    m_last->setText(0, tr("Successfully disconnected from: %1 (%2 : %3)")
        .arg(m_device->info().deviceName())
        .arg(m_device->info().controlEndpointAddress().toString())
        .arg(m_device->info().controlEndpointPort()));
    m_last->setFirstColumnSpanned(true);
}

void MainWindow::onDeviceDiscovered(QKnxNetIpServerInfo info)
{
    if (ui->devices->findText(info.deviceName()) == -1)
        qobject_cast<QStandardItemModel*>(ui->devices->model())->appendRow(new DeviceItem(info));

    if (m_tunnel.state() == QKnxNetIpTunnel::State::Disconnected)
        ui->devices->setEnabled(bool(ui->devices->count()));
}

void MainWindow::onErrorOccurred(QKnxNetIpEndpointConnection::Error error, QString errorString)
{
    m_last = new QTreeWidgetItem(ui->communication, m_last);
    const auto metaEnum = QMetaEnum::fromType<QKnxNetIpEndpointConnection::Error>();
    m_last->setText(0, tr("Error occurred: %1 (%2).").arg(errorString)
        .arg(metaEnum.valueToKey(int(error))));
    m_last->setFirstColumnSpanned(true);
}

void MainWindow::on_actionImport_triggered()
{
    const auto fileName = QFileDialog::getOpenFileName(this, tr("Import keyring file"),
        QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).value(0),
        tr("KNX keyring file (*.knxkeys)"));

    if (fileName.isEmpty())
        return;

    bool ok;
    const auto password = QInputDialog::getText(this, tr("Import keyring file"),
        tr("Keyring file password:"), QLineEdit::Normal, {}, &ok);
    if (!ok || password.isEmpty())
        return;

    m_secureConfigs = QKnxNetIpSecureConfiguration::fromKeyring(QKnxNetIpSecureConfiguration
        ::Type::DeviceManagement, fileName, password.toUtf8(), true);
    for (auto &config : m_secureConfigs)
        config.setIndividualAddress({});

    m_secureConfigs += QKnxNetIpSecureConfiguration::fromKeyring(QKnxNetIpSecureConfiguration
        ::Type::Tunneling, fileName, password.toUtf8(), true);

    on_devices_currentIndexChanged(ui->devices->currentIndex());
}

void MainWindow::on_devices_currentIndexChanged(int /* index */)
{
    delete m_device;
    ui->secureConfigs->clear();

    const auto model = qobject_cast<QStandardItemModel*>(ui->devices->model());
    m_device = static_cast<DeviceItem *> (model->item(ui->devices->currentIndex()))->clone();

    if (m_device) {
        const auto deviceInfo = m_device->info();
        for (int i = 0; i < m_secureConfigs.size(); ++i) {
            const auto &config = m_secureConfigs[i];
            if (deviceInfo.individualAddress() != config.host())
                continue;

            const auto ia = config.individualAddress();
            ui->secureConfigs->addItem(tr("User ID: %1 (Individual Address: %2)")
                .arg(config.userId())
                .arg(ia.isValid() ? ia.toString() : tr("No specific address")), i);
        }
    }

    ui->secureConfigs->setEnabled(bool(ui->secureConfigs->count())
        && ui->secureSession->isChecked());
    ui->secureSession->setEnabled(bool(ui->secureConfigs->count()));
}

void MainWindow::on_serviceTypes_currentIndexChanged(int index)
{
    bool enable = m_tunnel.state() == QKnxNetIpTunnel::State::Connected;
    const auto type = ui->serviceTypes->itemData(index).value<QKnxNetIp::ServiceType>();

    ui->sendRead->setEnabled(enable && type == QKnxNetIp::ServiceType::TunnelingFeatureGet);
    ui->sendWrite->setEnabled(enable && type == QKnxNetIp::ServiceType::TunnelingFeatureSet);
    ui->data->setEnabled(enable && type == QKnxNetIp::ServiceType::TunnelingFeatureSet);
}

void MainWindow::on_sendRead_clicked()
{
    const auto feature = ui->interfaceFeatures->itemData(ui->interfaceFeatures->currentIndex())
        .value<QKnx::InterfaceFeature>();

    m_tunnel.sendTunnelingFeatureGet(feature);
    populateFrame(QKnxNetIp::ServiceType::TunnelingFeatureGet, feature, {});
}

void MainWindow::on_sendWrite_clicked()
{
    const auto feature = ui->interfaceFeatures->itemData(ui->interfaceFeatures->currentIndex())
        .value<QKnx::InterfaceFeature>();

    const auto data = QKnxByteArray::fromHex(ui->data->text().toLatin1());
    m_tunnel.sendTunnelingFeatureSet(feature, data);
    populateFrame(QKnxNetIp::ServiceType::TunnelingFeatureSet, feature, data);
}

void MainWindow::on_connection_clicked()
{
    if (ui->devices->count() <= 0)
        return;

    if (m_tunnel.state() == QKnxNetIpTunnel::State::Connected)
        return m_tunnel.disconnectFromHost();

    const auto list = ui->interfaces->currentData().toStringList();
    m_tunnel.setLocalAddress(QHostAddress(list.first()));
    m_tunnel.setSerialNumber(QKnxByteArray::fromHex(list.last().toLatin1()));

    m_last = new QTreeWidgetItem(ui->communication, m_last);
    m_last->setText(0, tr("Establish connection to: %1 (%2 : %3)")
        .arg(m_device->info().deviceName())
        .arg(m_device->info().controlEndpointAddress().toString())
        .arg(m_device->info().controlEndpointPort()));
    m_last->setFirstColumnSpanned(true);

    if (ui->secureSession->isChecked()) {
        auto secureConfiguration = m_secureConfigs.value(ui->secureConfigs->currentData().toInt());
        secureConfiguration.setKeepSecureSessionAlive(true);
        m_tunnel.setSecureConfiguration(secureConfiguration);
        m_tunnel.connectToHostEncrypted(m_device->info().controlEndpointAddress(),
            m_device->info().controlEndpointPort());
    } else {
        m_tunnel.connectToHost(m_device->info().controlEndpointAddress(),
            m_device->info().controlEndpointPort(), QKnxNetIp::HostProtocol::UDP_IPv4);
    }
}

void MainWindow::setupInterfaces()
{
    auto firstItem = new QStandardItem(tr("Interface: --Select One--"));
    qobject_cast<QStandardItemModel*>(ui->interfaces->model())->appendRow(firstItem);
    firstItem->setSelectable(false);

    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const auto &iface : interfaces) {
        const auto addressEntries = iface.addressEntries();
        for (int j = 0; j < addressEntries.size(); j++) {
            const auto ip = addressEntries[j].ip();
            if (ip.isLoopback() || ip.toIPv4Address() == 0)
                continue;
            ui->interfaces->addItem(iface.name() + ": " + ip.toString(),
                QStringList { ip.toString(), iface.hardwareAddress().remove(QLatin1String(":")) });
        }
    }
    ui->interfaces->setCurrentIndex(bool(ui->interfaces->count()));

    connect(ui->interfaces, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int i) {
        if (i < 0)
            return;
        m_discoveryAgent.stop();
        m_discoveryAgent.setLocalAddress(QHostAddress(ui->interfaces->currentData()
            .toStringList().first()));
        m_discoveryAgent.start();
    });
}

void MainWindow::toggleUi(bool value)
{
    ui->serviceTypes->setEnabled(value);
    ui->interfaceFeatures->setEnabled(value);
    const auto type = ui->serviceTypes->itemData(ui->serviceTypes->currentIndex())
        .value<QKnxNetIp::ServiceType>();
    ui->sendRead->setEnabled(value && type == QKnxNetIp::ServiceType::TunnelingFeatureGet);
    ui->sendWrite->setEnabled(value && type == QKnxNetIp::ServiceType::TunnelingFeatureSet);
    ui->data->setEnabled(value && type == QKnxNetIp::ServiceType::TunnelingFeatureSet);
    ui->interfaces->setDisabled(value);
    ui->devices->setDisabled(value && bool(ui->devices->count()));
}

void MainWindow::populateServiceTypesComboBox()
{
    const auto typeEnum = QMetaEnum::fromType<QKnxNetIp::ServiceType>();
    ui->serviceTypes->addItem(tr("Tunneling Feature Get"), typeEnum.keyToValue("TunnelingFeatureGet"));
    ui->serviceTypes->addItem(tr("Tunneling Feature Set"), typeEnum.keyToValue("TunnelingFeatureSet"));
}

void MainWindow::populateInterfaceFeaturesComboBox()
{
    const auto typeEnum = QMetaEnum::fromType<QKnx::InterfaceFeature>();
    for (int i = 0; i < typeEnum.keyCount(); ++i) {
        if (!QKnx::isInterfaceFeature(QKnx::InterfaceFeature(typeEnum.value(i))))
            continue;
        ui->interfaceFeatures->addItem(typeEnum.key(i), typeEnum.value(i));
    }
}

void MainWindow::setText(QKnx::InterfaceFeature feature, const QKnxByteArray &data)
{
    const auto hex = data.toByteArray().toHex();
    m_last->setText(5, QStringLiteral("0x") + QLatin1String(hex, hex.size()));

    QString value;
    switch (feature) {
    case QKnx::InterfaceFeature::ActiveEmiType:
    case QKnx::InterfaceFeature::SupportedEmiType: {
        QStringList types;
        const auto emi = QKnx::EmiTypes(hex.toUShort(nullptr, 16));
        const auto metaEnum = QMetaEnum::fromType<QKnx::EmiType>();
        if (emi.testFlag(QKnx::EmiType::EMI1))
            types += QLatin1String(metaEnum.valueToKey(int(QKnx::EmiType::EMI1)));
        if (emi.testFlag(QKnx::EmiType::EMI2))
            types += QLatin1String(metaEnum.valueToKey(int(QKnx::EmiType::EMI2)));
        if (emi.testFlag(QKnx::EmiType::cEMI))
            types += QLatin1String(metaEnum.valueToKey(int(QKnx::EmiType::cEMI)));
        value = types.join(QLatin1Char('|'));
    } break;

    case QKnx::InterfaceFeature::HostDeviceDescriptorType0:
        value = QString::number(hex.toUInt(nullptr, 16));
    break;

    case QKnx::InterfaceFeature::BusConnectionStatus: {
        const auto metaEnum = QMetaEnum::fromType<QKnxState::State>();
        value = metaEnum.valueToKey(int(QKnxState::State(data.value(0))));
    } break;

    case QKnx::InterfaceFeature::MaximumApduLength:
        value = QString::number(hex.toUShort(nullptr, 16));
    break;

    case QKnx::InterfaceFeature::KnxManufacturerCode: {
        const auto id = hex.toUShort(nullptr, 16);
        value = QKnx::Ets::Manufacturers::fromId(id, QString::number(id));
    } break;

    case QKnx::InterfaceFeature::IndividualAddress:
        value = QKnxAddress(QKnxAddress::Type::Individual, data).toString();
    break;

    case QKnx::InterfaceFeature::InterfaceFeatureInfoServiceEnable: {
        const auto metaEnum = QMetaEnum::fromType<QKnxEnable::State>();
        value = metaEnum.valueToKey(int(QKnxEnable::State(data.value(0))));
    } break;

    default:
        break;
    }
    m_last->setText(3, value);
}

void MainWindow::populateFrame(QKnxNetIp::ServiceType type, QKnx::InterfaceFeature feature,
    const QKnxByteArray &value, int returnCode)
{
    m_last = new QTreeWidgetItem(ui->communication, m_last);

    auto metaEnum = QMetaEnum::fromType<QKnxNetIp::ServiceType>();
    m_last->setText(1, metaEnum.valueToKey(int(type)));

    metaEnum = QMetaEnum::fromType<QKnx::InterfaceFeature>();
    m_last->setText(2, metaEnum.valueToKey(int(feature)));

    if (type != QKnxNetIp::ServiceType::TunnelingFeatureGet)
        setText(feature, value);

    metaEnum = QMetaEnum::fromType<QKnx::ReturnCode>();
    if (const auto code = metaEnum.valueToKey(returnCode))
        m_last->setText(4, code);
}

void MainWindow::onFeatureResponseReceived(QKnx::InterfaceFeature feature, QKnx::ReturnCode code,
    const QKnxByteArray &value)
{
    populateFrame(QKnxNetIp::ServiceType::TunnelingFeatureResponse, feature, value, int(code));
}

void MainWindow::onFeatureInfoReceived(QKnx::InterfaceFeature feature, const QKnxByteArray &value)
{
    populateFrame(QKnxNetIp::ServiceType::TunnelingFeatureInfo, feature, value);
}
