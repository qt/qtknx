/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tunneling->setEnabled(false);
    ui->deviceManagement->setEnabled(false);

    ui->serverBox->addItem(QString("Scan to discover KNX server(s)"));
    ui->serverBox->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);

    connect(&m_tunneling, &QKnxNetIpTunnelConnection::connected, this, [&] {
        successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);
    });
    connect(&m_management, &QKnxNetIpDeviceManagementConnection::connected, this, [&] {
        successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);
    });
    connect(&m_tunneling, &QKnxNetIpTunnelConnection::disconnected, this, [&] {
        successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);
    });
    connect(&m_management, &QKnxNetIpDeviceManagementConnection::disconnected, this, [&] {
        successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);
    });
    connect(&m_discoveryCommunication, &QKnxNetIpServerDiscoveryAgent::finished, this, [&] {
        ui->scanButton->setEnabled(true);
    });
    connect(&m_discoveryCommunication, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered, this,
        &MainWindow::showServerAndServices);

    m_discoveryCommunication.setTimeout(5000);

    fillLocalIpBox();

    connect(ui->localIpBox, QOverload<int>::of(&QComboBox::activated), this,
        &MainWindow::newIPAddressSelected);
    connect(ui->serverBox, QOverload<int>::of(&QComboBox::activated), this,
        &MainWindow::newServerSelected);

    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    connect(ui->actionClear_Output, &QAction::triggered, ui->outputEdit, &QTextEdit::clear);
    connect(ui->actionClear_All, &QAction::triggered, this, [&] {
        ui->textOuputDeviceManagement->clear();
        ui->textOuputTunneling->clear();
        ui->outputEdit->clear();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showServerAndServices(const QKnxNetIpServerDiscoveryInfo info)
{
    ui->outputEdit->append("Server Endpoint found");
    ui->outputEdit->append(info.endpoint().toString());
    ui->outputEdit->append("Server's Multicast Address");
    ui->outputEdit->append(info.controlEndpointAddress().toString());
    ui->outputEdit->append("Server's Port");
    ui->outputEdit->append(QString::number(info.controlEndpointPort()));
    ui->outputEdit->append("The following services are supported");
    auto services = info.supportedServices();
    for (auto it = std::begin(services); it != std::end(services); ++it)
        ui->outputEdit->append(QString::number(quint8(it.key())));

    ui->serverBox->addItem(QString(info.controlEndpointAddress().toString() + ": "
        + QString::number(info.controlEndpointPort())), QVariant::fromValue(info));
}

void MainWindow::fillLocalIpBox()
{
    auto firstItem = new QStandardItem("Interface: IP address --Select One--");
    qobject_cast<QStandardItemModel*>(ui->localIpBox->model())->appendRow(firstItem);
    firstItem->setSelectable(false);
    firstItem->setData(QBrush(Qt::red), Qt::TextColorRole);

    auto networkInterfaces = QNetworkInterface::allInterfaces();
    for (int i = 0; i < networkInterfaces.size(); i++) {
        auto addressList = networkInterfaces[i].addressEntries();
        for (int j = 0; j < addressList.size(); j++) {
            auto ip = addressList[j].ip();
            if (ip.toIPv4Address() == 0)
                continue;
            auto ipAddress = ip.toString();
            ui->localIpBox->addItem(networkInterfaces[i].name() + ": " + ipAddress, ipAddress);
        }
    }
}

void MainWindow::newServerSelected(int serverBoxIndex)
{
    if (serverBoxIndex < 1)
        return;

    auto info = ui->serverBox->itemData(serverBoxIndex).value<QKnxNetIpServerDiscoveryInfo>();
    if (m_serverControlEndpoint.address().toString() == info.controlEndpointAddress().toString()
        && m_serverControlEndpoint.port() == info.controlEndpointPort()) {
        return;
    }

    if (info.endpoint().hostProtocol() != QKnxNetIp::HostProtocol::IpV4_Udp) {
        qDebug() << "Host Protocol not supported. This Server can't be selected";
        return;
    }

    m_serverControlEndpoint = info.endpoint();

    if (m_serverControlEndpoint.isValid()) {
        if (!(m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnected
            || m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnecting)) {
            m_tunneling.disconnectFromHost();
        }
        if (!(m_management.state() == QKnxNetIpEndpointConnection::State::Disconnected
            || m_management.state() == QKnxNetIpEndpointConnection::State::Disconnecting)) {
            m_management.disconnectFromHost();
        }
        enablingTab();
    }
}

void MainWindow::newIPAddressSelected(int localIpBoxIndex)
{
    if (localIpBoxIndex < 1)
        return;

    auto newAddress = QHostAddress(ui->localIpBox->itemData(localIpBoxIndex).toString());
    if (m_hostAddress == newAddress)
        return;

    if (newAddress.isNull()) {
        ui->outputEdit->append("Selected IP address is not valid");
        return;
    }

    m_hostAddress = newAddress;
    ui->scanButton->setEnabled(true);

    if (!(m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnected
        || m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnecting)) {
        m_tunneling.disconnectFromHost();
    }
    if (!(m_management.state() == QKnxNetIpEndpointConnection::State::Disconnected
        || m_management.state() == QKnxNetIpEndpointConnection::State::Disconnecting)) {
        m_management.disconnectFromHost();
    }

    m_discoveryCommunication.setLocalAddress(m_hostAddress);
    ui->outputEdit->append("Selected IP address: " + m_hostAddress.toString());
}

void MainWindow::on_scanButton_clicked()
{
    ui->scanButton->setEnabled(false);
    ui->serverBox->clear();
    ui->serverBox->addItem(QString("Select a KNX server(s)"));
    ui->serverBox->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);

    m_discoveryCommunication.start();
}

void MainWindow::on_connectRequestDeviceManagement_clicked()
{
    m_management.setLocalAddress(m_hostAddress);
    m_management.setLocalPort(0);
    m_management.connectToHost(m_serverControlEndpoint.address(), m_serverControlEndpoint.port());
}

void MainWindow::on_disconnectRequestDeviceManagement_clicked()
{
    m_management.disconnectFromHost();
}

void MainWindow::on_deviceManagementSendRequest_clicked()
{
    auto data = QByteArray::fromHex(ui->deviceManagementRequestLine->text().toUtf8());
    m_management.sendDeviceManagementFrame(QKnxCemiFrame::fromBytes(data, 0, data.size()));
}

void MainWindow::on_connectRequestTunneling_clicked()
{
    m_tunneling.setLocalAddress(m_hostAddress);
    m_tunneling.setLocalPort(0);
    m_tunneling.connectToHost(m_serverControlEndpoint.address(), m_serverControlEndpoint.port());
}

void MainWindow::on_disconnectRequestTunneling_clicked()
{
    m_tunneling.disconnectFromHost();
}

void MainWindow::on_tunnelingSendRequest_clicked()
{
    auto data = QByteArray::fromHex(ui->tunnelingRequestLine->text().toUtf8());
    m_tunneling.sendTunnelingFrame(QKnxCemiFrame::fromBytes(data, 0, data.size()));
}

void MainWindow::linkToService(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service,
    QPushButton **connect, QPushButton **disconnect, QPushButton **send, QTextEdit **output)
{
    switch (service) {
    case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement:
        *connect = ui->connectRequestDeviceManagement;
        *disconnect = ui->disconnectRequestDeviceManagement;
        *send = ui->deviceManagementSendRequest;
        *output = ui->textOuputDeviceManagement;
        break;
    case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling:
        *connect = ui->connectRequestTunneling;
        *disconnect = ui->disconnectRequestTunneling;
        *send = ui->tunnelingSendRequest;
        *output = ui->textOuputTunneling;
        break;
    default:
        break;
    }
}

void MainWindow::successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service)
{
    QPushButton *connectButton, *disconnectButton, *sendButton;
    QTextEdit *output = nullptr;

    linkToService(service, &connectButton, &disconnectButton, &sendButton, &output);

    output->append("Successful connection");
    connectButton->setEnabled(false);
    connectButton->setMouseTracking(true);
    connectButton->setToolTip("Connected to " + m_serverControlEndpoint.address().toString()
        + " on port: " + QString::number(m_serverControlEndpoint.port()));
    connectButton->setText("Connected");
    QPalette pal = connectButton->palette();
    pal.setColor(QPalette::Button, QColor::fromRgb(143, 207, 22));
    pal.setColor(QPalette::ButtonText, QColor::fromRgb(50, 68, 52));
    connectButton->setAutoFillBackground(true);
    connectButton->setPalette(pal);
    connectButton->update();
    sendButton->setEnabled(true);
    disconnectButton->setEnabled(true);
}

void MainWindow::successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service)
{
    QPushButton *connectButton, *disconnectButton, *sendButton;
    QTextEdit *output = nullptr;

    linkToService(service, &connectButton, &disconnectButton, &sendButton, &output);

    output->append("Successful disconnection");
    output->append("--------------------------------------------------");
    disconnectButton->setEnabled(false);
    connectButton->setEnabled(true);
    connectButton->setToolTip("");
    QPalette pal = disconnectButton->palette();
    connectButton->setEnabled(true);
    connectButton->setText("Connect");
    connectButton->setAutoFillBackground(true);
    connectButton->setPalette(pal);
    connectButton->update();
    sendButton->setEnabled(false);
}

void MainWindow::enablingTab()
{
    ui->deviceManagement->setEnabled(true);
    if (m_management.state() == QKnxNetIpEndpointConnection::State::Disconnected) {
        ui->connectRequestDeviceManagement->setEnabled(true);
        ui->disconnectRequestDeviceManagement->setEnabled(false);
    }
    ui->deviceManagementSendRequest->setEnabled(false);
    ui->tunneling->setEnabled(true);
    if (m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnected) {
        ui->connectRequestTunneling->setEnabled(true);
        ui->disconnectRequestTunneling->setEnabled(false);
    }
    ui->tunnelingSendRequest->setEnabled(false);
}
