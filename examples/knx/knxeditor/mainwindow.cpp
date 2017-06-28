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
    tabPageInit();
    ui->serverBox->addItem(QString("Scan to discover KNX server(s)"));
    ui->serverBox->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);

    connect(&m_tunneling, &QKnxNetIpTunnelConnection::connected, [&] {
        successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);});
    connect(&m_management, &QKnxNetIpDeviceManagementConnection::connected, [&] {
        successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);});
    connect(&m_tunneling, &QKnxNetIpTunnelConnection::disconnected, [&] {
        successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);});
    connect(&m_management, &QKnxNetIpDeviceManagementConnection::disconnected, [&] {
        successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);});
    connect(&m_discoveryCommunication, &QKnxNetIpServerDiscoveryAgent::finished,[&] {
        ui->scanButton->setEnabled(true);});
    connect(&m_discoveryCommunication, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered, this,
        &MainWindow::showServerAndServices);

    m_discoveryCommunication.setTimeout(5000);

    fillLocalIpBox();

    connect(ui->localIpBox, SIGNAL(activated(int)),this, SLOT(newIPAddressSelected(int)));
    connect(ui->serverBox, SIGNAL(activated(int)),this, SLOT(newServerSelected(int)));

    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->actionClear_Output,&QAction::triggered, ui->outputEdit, &QTextEdit::clear);
    connect(ui->actionClear_All, &QAction::triggered, [&] {
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
    QKnxNetIpHpai serverControlEndpoint;
    serverControlEndpoint.setHpai(info.endpoint().hostProtocol(),
                        info.controlEndpointAddress(),
                        info.controlEndpointPort());

    ui->outputEdit->append("Server Endpoint found");
    ui->outputEdit->append(serverControlEndpoint.toString());
    ui->outputEdit->append("Server's Multicast Address");
    ui->outputEdit->append(info.controlEndpointAddress().toString());
    ui->outputEdit->append("Server's Port");
    ui->outputEdit->append(QString::number(info.controlEndpointPort()));
    ui->outputEdit->append("The following services are supported");
    auto services = info.supportedServices();
    for (auto it = std::begin(services); it != std::end(services); ++it)
        ui->outputEdit->append(QString::number(quint8(it.key())));

    ui->serverBox->addItem(QString(info.controlEndpointAddress().toString()+": "+
        QString::number(info.controlEndpointPort())),
        QVariant(QVariant::fromValue(info)));
}

void MainWindow::tabPageInit()
{
    ui->deviceManagement->setEnabled(false);
    ui->tunneling->setEnabled(false);
}

void MainWindow::fillLocalIpBox()
{
    ui->localIpBox->addItem(QString("Interface: IPAddress --Select One--"));
    ui->localIpBox->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);
    QStandardItemModel* model =
            qobject_cast<QStandardItemModel*>(ui->localIpBox->model());
    QModelIndex firstIndex = model->index(0, ui->localIpBox->modelColumn(),
            ui->localIpBox->rootModelIndex());
    QStandardItem* firstItem = model->itemFromIndex(firstIndex);
    if (!firstItem)
        return;
    firstItem->setSelectable(false);

    auto networkInterfaces = QNetworkInterface::allInterfaces();
    for (int i=0; i < networkInterfaces.size(); i++) {
        auto addressList = networkInterfaces[i].addressEntries();
        for (int j = 0; j < addressList.size(); j++) {
            auto ipAddress = addressList[j].ip().toString();
            ui->localIpBox->addItem(networkInterfaces[i].name() + ": " + ipAddress, ipAddress);
        }
    }
}

void MainWindow::newServerSelected(int serverBoxIndex)
{
    if (serverBoxIndex < 1)
        return;

    QKnxNetIpServerDiscoveryInfo info =
        ui->serverBox->itemData(serverBoxIndex).value<QKnxNetIpServerDiscoveryInfo>();

    if (m_serverControlEndpoint.address().toString() == info.controlEndpointAddress().toString()
    && m_serverControlEndpoint.port() == info.controlEndpointPort()) {
        return;
    }

    if (info.endpoint().hostProtocol() != QKnxNetIp::HostProtocol::IpV4_Udp) {
        qDebug()<<"Host Protocol not supported. This Server can't be selected";
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
    bool firstTime = false;

    if (m_hostAddress == QHostAddress::Null)
        firstTime = true;

    if (localIpBoxIndex > 0) {
        QString hostAddressName = ui->localIpBox->itemData(localIpBoxIndex).toString();
        QString hostAddressOld = m_hostAddress.toString();
        m_hostAddress.setAddress(hostAddressName);
        if (m_hostAddress.isNull()) {
            m_hostAddress.setAddress(hostAddressOld);
            ui->outputEdit->append("Selected Ip Address is not valid");
            return;
        }
        if (firstTime) {
            ui->scanButton->setEnabled(true);
        } else {
            if ( hostAddressOld != hostAddressName) {
                if (!(m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnected ||
                m_tunneling.state() == QKnxNetIpEndpointConnection::State::Disconnecting)) {
                    m_tunneling.disconnectFromHost();
                }
                if (!(m_management.state() == QKnxNetIpEndpointConnection::State::Disconnected ||
                m_management.state() == QKnxNetIpEndpointConnection::State::Disconnecting)) {
                    m_management.disconnectFromHost();
                }
            }
        }
        ui->outputEdit->append("Selected IP Address: " + hostAddressName);
        m_discoveryCommunication.setLocalAddress(m_hostAddress);
    }
}

//************************************************************
// BUTTON SLOTs
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
    QString textRequest = ui->deviceManagementRequestLine->text();
    QByteArray data = QByteArray::fromHex(textRequest.toUtf8());
    QKnxCemiFrame cemi = QKnxCemiFrame::fromBytes(data, 0, data.size());
    m_management.sendDeviceManagementFrame(cemi);
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
    QString textRequest = ui->tunnelingRequestLine->text();
    QByteArray data = QByteArray::fromHex(textRequest.toUtf8());
    QKnxCemiFrame cemi = QKnxCemiFrame::fromBytes(data, 0, data.size());
    m_tunneling.sendTunnelingFrame(cemi);
}

void MainWindow::linkToService(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service,
    QPushButton **connectButton, QPushButton **disconnectButton, QTextEdit **output)
{
    switch (service) {
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement:
            *connectButton = ui->connectRequestDeviceManagement;
            *disconnectButton = ui->disconnectRequestDeviceManagement;
            ui->deviceManagementSendRequest->setEnabled(true);
            *output = ui->textOuputDeviceManagement;
            break;
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling:
            *connectButton = ui->connectRequestTunneling;
            *disconnectButton = ui->disconnectRequestTunneling;
            ui->tunnelingSendRequest->setEnabled(true);
            *output = ui->textOuputTunneling;
            break;
        default:
            return;
    }
    qDebug()<<"connectButton"<<connectButton;
    return;
}


void MainWindow::successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service)
{
    QPushButton *connectButton = nullptr;
    QPushButton *disconnectButton = nullptr;
    QTextEdit *output = nullptr;

    linkToService(service, &connectButton, &disconnectButton, &output);

    output->append("Successful connection");
    connectButton->setEnabled(false);
    connectButton->setMouseTracking(true);
    connectButton->setToolTip("Connected to " + m_serverControlEndpoint.address().toString()
    + " on port: " + QString::number(m_serverControlEndpoint.port()));
    connectButton->setText("Connected");
    QPalette pal = connectButton->palette();
    pal.setColor(QPalette::Button, QColor::fromRgb(143,207,22));
    pal.setColor(QPalette::ButtonText, QColor::fromRgb(50,68,52));
    connectButton->setAutoFillBackground(true);
    connectButton->setPalette(pal);
    connectButton->update();
    // Disconnect Button is enabled
    disconnectButton->setEnabled(true);
}

void MainWindow::successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service)
{
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QTextEdit *output;

    switch (service) {
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement:
            connectButton = ui->connectRequestDeviceManagement;
            disconnectButton = ui->disconnectRequestDeviceManagement;
            ui->deviceManagementSendRequest->setEnabled(false);
            output = ui->textOuputDeviceManagement;
            break;
        case QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling:
            connectButton = ui->connectRequestTunneling;
            disconnectButton = ui->disconnectRequestTunneling;
            ui->tunnelingSendRequest->setEnabled(false);
            output = ui->textOuputTunneling;
            break;
        default:
            return;
     }

    output->append("Successful disconnection");
    output->append("--------------------------------------------------");
    // Disconnect Button cannot be pushed
    disconnectButton->setEnabled(false);
    // Connect Button ready to be pushed
    connectButton->setEnabled(true);
    connectButton->setToolTip("");
    QPalette pal = disconnectButton->palette();
    connectButton->setEnabled(true);
    connectButton->setText("Connect");
    connectButton->setAutoFillBackground(true);
    connectButton->setPalette(pal);
    connectButton->update();
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
