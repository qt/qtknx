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

#include <QKnxNetIpHPAI>
#include <QKnxNetIpServerDiscoveryInfo>
#include <QNetworkInterface>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTextEdit>

static QString familieToString(QKnxNetIpServiceFamiliesDib::ServiceFamilieId id)
{
    switch (id) {
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core:
            return "Core";
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement:
            return "Device Management";
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling:
            return "Tunnel";
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpRouting:
            return "Routing";
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteLogging:
            return "Remote Logging";
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteConfigAndDiagnosis:
            return "Remote Configuration";
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::ObjectServer:
            return "Object Server";
        default:
            break;
    }
    return "Unknown";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tunneling->setEnabled(false);
    ui->deviceManagement->setEnabled(false);

    ui->serverBox->addItem(tr("Press Scan button to discover KNX server(s)"));

    connect(&m_tunneling, &QKnxNetIpTunnelConnection::connected, this, [&] {
        onConnected(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);
    });
    connect(&m_management, &QKnxNetIpDeviceManagementConnection::connected, this, [&] {
        onConnected(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);
    });
    connect(&m_tunneling, &QKnxNetIpTunnelConnection::disconnected, this, [&] {
        onDisconnected(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);
    });
    connect(&m_management, &QKnxNetIpDeviceManagementConnection::disconnected, this, [&] {
        onDisconnected(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);
    });

    m_discoveryAgent.setTimeout(5000);
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::started, this, [&] {
        ui->scanButton->setEnabled(false);
        ui->checkboxNat->setEnabled(false);

        ui->serverDescription->clear();

        ui->serverBox->clear();
        ui->serverBox->addItem(QString("Select a KNX server(s)"));
    });
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::finished, this, [&] {
        ui->scanButton->setEnabled(true);
        ui->checkboxNat->setEnabled(true);

        if (ui->serverBox->count() <= 1)
            ui->serverBox->setItemText(0, tr("Press Scan button to discover KNX server(s)"));
    });
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered, this,
        &MainWindow::showServerAndServices);

    fillLocalIpBox();

    connect(ui->scanButton, &QPushButton::clicked, &m_discoveryAgent,
        QOverload<>::of(&QKnxNetIpServerDiscoveryAgent::start));

    connect(ui->checkboxNat, &QCheckBox::toggled, this, [&] (bool checked) {
        m_tunneling.setNatAware(checked);
        m_management.setNatAware(checked);
        m_discoveryAgent.setNatAware(checked);
    });

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

void MainWindow::showServerAndServices(const QKnxNetIpServerDiscoveryInfo &info)
{
    ui->outputEdit->append("Server Endpoint found");
    ui->outputEdit->append(info.endpoint().toString());
    ui->outputEdit->append("Server's Multicast Address");
    ui->outputEdit->append(info.controlEndpointAddress().toString());
    ui->outputEdit->append("Server's Port");
    ui->outputEdit->append(QString::number(info.controlEndpointPort()));
    ui->outputEdit->append("The following services are supported:");
    auto services = info.supportedServices();
    for (auto it = std::begin(services); it != std::end(services); ++it) {
       ui->outputEdit->append(tr("    KNXnet/IP %1, Version: %2").arg(familieToString(it.key()))
           .arg(it.value()));
    }

    ui->serverBox->addItem(tr("%1 (%2:%3").arg(info.deviceName(), info.controlEndpointAddress()
        .toString()).arg(info.controlEndpointPort()), QVariant::fromValue(info));
}

void MainWindow::fillLocalIpBox()
{
    auto firstItem = new QStandardItem("Interface: IP address --Select One--");
    qobject_cast<QStandardItemModel*>(ui->localIpBox->model())->appendRow(firstItem);
    firstItem->setSelectable(false);

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

    ui->serverDescription->setText(tr("<html><head><style> th { text-align: left; } td.padding { "
            "padding-left: 10px; } </style></head> <body>"
            "   <table style=\"width:100%\">"
            "       <th>Device Information</th>"
            "           <tr><td class=\"padding\">Individual address: %1</td></tr>"
            "           <tr><td class=\"padding\">Server control endpoint: %2:%3</td></tr>"
            "           <tr></tr>"
            "       <tr><th>Supported services:</th></tr>"
            "           %4"
            "       </table>"
            "   </table>"
            "</body></html>")
        .arg(info.individualAddress().toString())
        .arg(info.controlEndpointAddress().toString()).arg(info.controlEndpointPort())
        .arg([&info]() -> QString {
            QString value;
            auto services = info.supportedServices();
            for (auto it = services.constBegin(); it != services.constEnd(); ++it) {
                value.append(tr("<tr><td class=\"padding\">%1</td></th>")
                    .arg(tr("KNXnet/IP %1, Version: %2")
                        .arg(familieToString(it.key())).arg(it.value())));
            }
            return value;
        }())
    );

    const auto &endpoint = info.endpoint();
    if (endpoint.hostProtocol() != QKnxNetIp::HostProtocol::IpV4_Udp) {
        qDebug() << "Host Protocol not supported. This Server can't be selected.";
        return;
    }

    if (info.endpoint().isValid() && m_server != info) {
        m_server = info;
        m_tunneling.disconnectFromHost();
        m_management.disconnectFromHost();
        enablingTab();
    }
}

void MainWindow::newIPAddressSelected(int localIpBoxIndex)
{
    if (localIpBoxIndex < 1)
        return;

    auto newAddress = QHostAddress(ui->localIpBox->itemData(localIpBoxIndex).toString());
    if (newAddress.isNull()) {
        ui->outputEdit->append("Selected IP address is not valid");
        return;
    }

    if (m_tunneling.localAddress() == newAddress)
        return;

    ui->scanButton->setEnabled(true);

    m_discoveryAgent.stop();
    m_tunneling.disconnectFromHost();
    m_management.disconnectFromHost();

    m_tunneling.setLocalAddress(newAddress);
    m_management.setLocalAddress(newAddress);
    m_discoveryAgent.setLocalAddress(newAddress);
    ui->outputEdit->append("Selected IP address: " + newAddress.toString());
}

void MainWindow::on_connectRequestDeviceManagement_clicked()
{
    m_management.setLocalPort(0);
    m_management.connectToHost(m_server.controlEndpointAddress(), m_server.controlEndpointPort());
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
    m_tunneling.setLocalPort(0);
    m_tunneling.connectToHost(m_server.controlEndpointAddress(), m_server.controlEndpointPort());
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

    ui->checkboxNat->setDisabled(m_tunneling.state() == QKnxNetIpTunnelConnection::State::Connected
        || m_management.state() == QKnxNetIpDeviceManagementConnection::State::Connected);
}

void MainWindow::onConnected(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service)
{
    QPushButton *connectButton, *disconnectButton, *sendButton;
    QTextEdit *output = nullptr;

    linkToService(service, &connectButton, &disconnectButton, &sendButton, &output);

    output->append("Successful connection");
    connectButton->setEnabled(false);
    connectButton->setMouseTracking(true);
    connectButton->setToolTip("Connected to " + m_server.controlEndpointAddress().toString()
        + " on port: " + QString::number(m_server.controlEndpointPort()));
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

void MainWindow::onDisconnected(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service)
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
