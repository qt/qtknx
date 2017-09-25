/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkInterface>
#include <QStandardItem>

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

    connect(ui->checkboxNat, &QCheckBox::toggled, this, [&](bool checked) {
        ui->tunneling->setNatAware(checked);
        m_discoveryAgent.setNatAware(checked);
        ui->deviceManagement->setNatAware(checked);
    });

    connect(ui->localIpBox, QOverload<int>::of(&QComboBox::activated), this,
        &MainWindow::newIPAddressSelected);
    connect(ui->serverBox, QOverload<int>::of(&QComboBox::activated), this,
        &MainWindow::newServerSelected);

    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    connect(ui->actionClear_Output, &QAction::triggered, ui->outputEdit, &QTextEdit::clear);

    connect(ui->actionClear_All, &QAction::triggered, ui->deviceManagement,
        &LocalDeviceManagement::clearLogging);
    connect(ui->actionClear_All, &QAction::triggered, ui->outputEdit, &QTextEdit::clear);
    connect(ui->actionClear_All, &QAction::triggered, ui->tunneling, &Tunneling::clearLogging);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newServerSelected(int serverBoxIndex)
{
    if (serverBoxIndex < 1)
        return;

    auto info = ui->serverBox->itemData(serverBoxIndex).value<QKnxNetIpServerInfo>();

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

        ui->tunneling->setEnabled(true);
        ui->tunneling->setKnxNetIpServer(m_server);

        ui->deviceManagement->setEnabled(true);
        ui->deviceManagement->setKnxNetIpServer(m_server);
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

    if (m_discoveryAgent.localAddress() == newAddress)
        return;

    ui->scanButton->setEnabled(true);
    ui->outputEdit->append("Selected IP address: " + newAddress.toString());

    m_discoveryAgent.stop();
    m_discoveryAgent.setLocalAddress(newAddress);

    ui->tunneling->setLocalAddress(newAddress);
    ui->deviceManagement->setLocalAddress(newAddress);
}

void MainWindow::showServerAndServices(const QKnxNetIpServerInfo &info)
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

    ui->serverBox->addItem(tr("%1 (%2:%3)").arg(info.deviceName(), info.controlEndpointAddress()
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
