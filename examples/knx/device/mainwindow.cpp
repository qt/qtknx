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

    auto treeWidget = new QTreeWidget;
    treeWidget->setHeaderHidden(true);
    treeWidget->setUniformRowHeights(true);

    ui->propertyId->setModel(treeWidget->model());
    ui->propertyId->setView(treeWidget);
    on_interfaceObjectTypes_currentTextChanged(QStringLiteral("General"));

    populateInterfaceObjectTypesComboBox();

    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    connect(ui->actionClear, &QAction::triggered, ui->communication, &QTreeWidget::clear);

    connect(&m_management, &QKnxNetIpDeviceManagement::connected, this, &MainWindow::onConnected);
    connect(&m_management, &QKnxNetIpDeviceManagement::disconnected, this, &MainWindow::onDisconnected);
    connect(&m_management, &QKnxNetIpDeviceManagement::frameReceived, this, &MainWindow::populateFrame);
    connect(&m_management, &QKnxNetIpDeviceManagement::errorOccurred, this, &MainWindow::onErrorOccurred);

    connect(ui->communication, &QTreeWidget::currentItemChanged, this,
        [&](QTreeWidgetItem *current, QTreeWidgetItem * /* previuos */) {
            m_current = current;
    });

    m_discoveryAgent.setTimeout(-1);
    m_discoveryAgent.setSearchFrequency(6);
    m_discoveryAgent.setDiscoveryMode(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1
        | QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2);
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered,
        this, &MainWindow::onDeviceDiscovered);

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

    if (m_management.state() == QKnxNetIpTunnel::State::Disconnected)
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
    on_devices_currentIndexChanged(ui->devices->currentIndex());
}

void MainWindow::on_devices_currentIndexChanged(int  index)
{
    delete m_device;
    m_device = nullptr;
    ui->secureConfigs->clear();

    if (index >= 0) {
        const auto model = qobject_cast<QStandardItemModel*>(ui->devices->model());
        m_device = static_cast<DeviceItem *> (model->item(ui->devices->currentIndex()))->clone();
    }

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
    } else {
        m_device = new DeviceItem({});
    }

    ui->secureConfigs->setEnabled(bool(ui->secureConfigs->count())
        && ui->secureSession->isChecked());
    ui->secureSession->setEnabled(bool(ui->secureConfigs->count()));
}

void MainWindow::on_interfaceObjectTypes_currentTextChanged(const QString &type)
{
    auto treeWidget = qobject_cast<QTreeWidget *>(ui->propertyId->view());
    if (!treeWidget)
        return;

    // keep the 'General' property top level item
    auto topLevelItem = treeWidget->takeTopLevelItem(0);
    ui->propertyId->clear();
    treeWidget->addTopLevelItem(topLevelItem);

    int index = QKnxInterfaceObjectProperty::staticMetaObject.indexOfEnumerator(type.toLatin1());
    if (index >= 0) {
        const auto typeEnum = QKnxInterfaceObjectProperty::staticMetaObject.enumerator(index);
        topLevelItem = new QTreeWidgetItem(treeWidget, { typeEnum.name() });
        for (auto a = 0; a < typeEnum.keyCount(); ++a) {
            auto item = new QTreeWidgetItem({ typeEnum.key(a) });
            item->setData(0, Qt::UserRole, typeEnum.value(a));
            topLevelItem->addChild(item);
        }
        topLevelItem->setFlags(topLevelItem->flags() &~Qt::ItemIsSelectable);
    }

    // select first sub item
    treeWidget->expandItem(topLevelItem);
    treeWidget->setCurrentItem(topLevelItem, 0);
    ui->propertyId->setRootModelIndex(treeWidget->currentIndex());
    ui->propertyId->setCurrentIndex(0);
    treeWidget->setCurrentItem(treeWidget->invisibleRootItem(), 0);
    ui->propertyId->setRootModelIndex(treeWidget->currentIndex());
}

void MainWindow::on_sendRead_clicked()
{
    const auto frame = QKnxDeviceManagementFrame::propertyReadBuilder()
        .setObjectType(ui->interfaceObjectTypes->currentData().toInt())
        .setObjectInstance(quint8(ui->objectInstance->value()))
        .setProperty(quint16(ui->propertyId->currentData().toUInt()))
        .setNumberOfElements(quint8(ui->numberOfElements->value()))
        .setStartIndex(quint16(ui->startIndex->value()))
        .createRequest();

    populateFrame(frame);
    m_management.sendFrame(frame);
}

void MainWindow::on_sendWrite_clicked()
{
    const auto frame = QKnxDeviceManagementFrame::propertyWriteBuilder()
        .setObjectType(ui->interfaceObjectTypes->currentData().toInt())
        .setObjectInstance(quint8(ui->objectInstance->value()))
        .setProperty(quint16(ui->propertyId->currentData().toUInt()))
        .setNumberOfElements(quint8(ui->numberOfElements->value()))
        .setStartIndex(quint16(ui->startIndex->value()))
        .createRequest(QKnxByteArray::fromHex(ui->data->text().toLatin1()));

    populateFrame(frame);
    m_management.sendFrame(frame);
}

void MainWindow::on_connection_clicked()
{
    if (ui->devices->count() <= 0)
        return;

    if (m_management.state() == QKnxNetIpTunnel::State::Connected)
        return m_management.disconnectFromHost();

    const auto list = ui->interfaces->currentData().toStringList();
    m_management.setLocalAddress(QHostAddress(list.first()));
    m_management.setSerialNumber(QKnxByteArray::fromHex(list.last().toLatin1()));

    m_last = new QTreeWidgetItem(ui->communication, m_last);
    m_last->setText(0, tr("Establish connection to: %1 (%2 : %3)")
        .arg(m_device->info().deviceName())
        .arg(m_device->info().controlEndpointAddress().toString())
        .arg(m_device->info().controlEndpointPort()));
    m_last->setFirstColumnSpanned(true);

    if (ui->secureSession->isChecked()) {
        auto secureConfiguration = m_secureConfigs.value(ui->secureConfigs->currentData().toInt());
        secureConfiguration.setKeepSecureSessionAlive(true);
        m_management.setSecureConfiguration(secureConfiguration);
        m_management.connectToHostEncrypted(m_device->info().controlEndpointAddress(),
            m_device->info().controlEndpointPort());
    } else {
        m_management.connectToHost(m_device->info().controlEndpointAddress(),
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

    connect(ui->interfaces, &QComboBox::currentIndexChanged, this, [&](int i) {
        if (i < 0)
            return;
        m_discoveryAgent.stop();
        ui->devices->clear();
        m_discoveryAgent.setLocalAddress(QHostAddress(ui->interfaces->currentData()
            .toStringList().first()));
        m_discoveryAgent.start();
    });
}

void MainWindow::toggleUi(bool value)
{
    ui->interfaceObjectTypes->setEnabled(value);
    ui->objectInstance->setEnabled(value);
    ui->propertyId->setEnabled(value);
    ui->numberOfElements->setEnabled(value);
    ui->startIndex->setEnabled(value);
    ui->sendRead->setEnabled(value);
    ui->data->setEnabled(value);
    ui->sendWrite->setEnabled(value);
    ui->interfaces->setDisabled(value);
    ui->devices->setDisabled(value && bool(ui->devices->count()));
}

void MainWindow::populateInterfaceObjectTypesComboBox()
{
    const auto typeEnum = QMetaEnum::fromType<QKnxInterfaceObjectType::System>();
    for (int i = 0; i < typeEnum.keyCount(); ++i)
        ui->interfaceObjectTypes->addItem(typeEnum.key(i), typeEnum.value(i));
}

void MainWindow::populateFrame(const QKnxDeviceManagementFrame &frame)
{
    m_last = new QTreeWidgetItem(ui->communication, m_last);

    if (frame.isNegativeConfirmation()) {
        const auto metaEnum = QMetaEnum::fromType<QKnxNetIpCemiServer::Error>();
        m_last->setText(0, tr("Received negative confirmation: Error (%1)")
            .arg(metaEnum.valueToKey(int(frame.error()))));
        return m_last->setFirstColumnSpanned(true);
    }

    auto metaEnum = QMetaEnum::fromType<QKnxDeviceManagementFrame::MessageCode>();
    m_last->setText(1, metaEnum.valueToKey(int(frame.messageCode())));

    metaEnum = QMetaEnum::fromType<QKnxInterfaceObjectType::System>();
    const auto type = metaEnum.valueToKey(int(frame.objectType()));
    m_last->setText(2, QString::fromLatin1(type));

    m_last->setText(3, QString::number(frame.objectInstance()));

    int index = QKnxInterfaceObjectProperty::staticMetaObject.indexOfEnumerator(type);
    if (index >= 0) {
        metaEnum = QKnxInterfaceObjectProperty::staticMetaObject.enumerator(index);
        m_last->setText(4, metaEnum.valueToKey(int(frame.property())));
    } else {
        m_last->setText(4, QString::number(int(frame.property())));
    }

    m_last->setText(5, QString::number(frame.numberOfElements()));
    m_last->setText(6, QString::number(frame.startIndex()));

    auto bytes = frame.data().toHex();
    m_last->setText(7, QString::fromLatin1(bytes.toByteArray(), bytes.size()));
    bytes = frame.bytes().toHex();
    m_last->setText(8, QString::fromLatin1(bytes.toByteArray(), bytes.size()));

    if (m_current) {
        const auto region = ui->communication->viewport()->visibleRegion();
        const auto currentRect = ui->communication->visualItemRect(m_current);
        if (region.contains({ currentRect.left(), currentRect.top() - currentRect.height() })
            && region.contains(ui->communication->visualItemRect(m_last))) {
                ui->communication->scrollToItem(m_last);
        }
    } else {
        ui->communication->scrollToItem(m_last);
    }
}
