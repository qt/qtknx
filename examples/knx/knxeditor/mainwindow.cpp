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

#include <QElapsedTimer>
#include <QNetworkInterface>
#include <QStandardItem>

Ui::MainWindow *MainWindow::s_ui { nullptr };

static QString familieToString(QKnxNetIp::ServiceFamily id)
{
    switch (id) {
    case QKnxNetIp::ServiceFamily::Core:
        return MainWindow::tr("Core");
    case QKnxNetIp::ServiceFamily::DeviceManagement:
        return MainWindow::tr("Device Management");
    case QKnxNetIp::ServiceFamily::IpTunneling:
        return MainWindow::tr("Tunnel");
    case QKnxNetIp::ServiceFamily::IpRouting:
        return MainWindow::tr("Routing");
    case QKnxNetIp::ServiceFamily::RemoteLogging:
        return MainWindow::tr("Remote Logging");
    case QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis:
        return MainWindow::tr("Remote Configuration");
    case QKnxNetIp::ServiceFamily::ObjectServer:
        return MainWindow::tr("Object Server");
    case QKnxNetIp::ServiceFamily::Security:
        return MainWindow::tr("Security");
    default:
        break;
    }
    return MainWindow::tr("Unknown");
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
        if (!ui)
            return;

        ui->scanButton->setEnabled(false);
        ui->checkboxNat->setEnabled(false);

        ui->serverDescription->clear();

        ui->serverBox->clear();
        ui->serverBox->setEnabled(false);

        auto firstItem = new QStandardItem(tr("Select a KNX server(s)"));
        qobject_cast<QStandardItemModel*>(ui->serverBox->model())->appendRow(firstItem);
        firstItem->setSelectable(false);
    });
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::finished, this, [&] {
        if (!ui)
            return;

        ui->scanButton->setEnabled(true);
        ui->checkboxNat->setEnabled(true);

        if (ui->serverBox->count() <= 1)
            ui->serverBox->setItemText(0, tr("Press Scan button to discover KNX server(s)"));
        else if (ui->serverBox->count() == 2)
            ui->serverBox->setCurrentIndex(1);

        ui->serverBox->setEnabled(true);
        newServerSelected(ui->serverBox->currentIndex());
    });
    connect(&m_discoveryAgent, &QKnxNetIpServerDiscoveryAgent::deviceDiscovered, this,
        &MainWindow::showServerAndServices);

    fillLocalIpBox();

    connect(ui->scanButton, &QPushButton::clicked, &m_discoveryAgent,
        QOverload<>::of(&QKnxNetIpServerDiscoveryAgent::start));

    connect(ui->checkboxNat, &QCheckBox::toggled, this, [&](bool checked) {
        if (!ui)
            return;

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
    if (ui->localIpBox->count() == 2) {
        ui->localIpBox->setCurrentIndex(1);
        newIPAddressSelected(ui->localIpBox->currentIndex());
    }

    s_ui = ui;
    qInstallMessageHandler(MainWindow::messageHandler);
}

MainWindow::~MainWindow()
{
    s_ui = nullptr;
    delete ui;
    ui = nullptr;
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
            const auto services = info.supportedServices();
            for (const auto &service : services) {
                value.append(tr("<tr><td class=\"padding\">%1</td></th>")
                    .arg(tr("KNXnet/IP %1, Version: %2").arg(familieToString(service.ServiceFamily))
                        .arg(service.ServiceFamilyVersion)));
            }
            return value;
        }())
    );

    const auto &hpai = info.endpoint();
    const QKnxNetIpHpaiProxy endpoint(hpai);
    if (endpoint.hostProtocol() != QKnxNetIp::HostProtocol::UDP_IPv4)
        return;

    if (endpoint.isValid() && m_server != info) {
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
        ui->outputEdit->append(tr("Selected IP address is not valid"));
        return;
    }

    if (m_discoveryAgent.localAddress() == newAddress)
        return;

    ui->scanButton->setEnabled(true);
    ui->outputEdit->append(tr("Selected IP address: ") + newAddress.toString());

    m_discoveryAgent.stop();
    m_discoveryAgent.setLocalAddress(newAddress);

    ui->tunneling->setLocalAddress(newAddress);
    ui->deviceManagement->setLocalAddress(newAddress);
}

void MainWindow::showServerAndServices(const QKnxNetIpServerInfo &info)
{
    ui->outputEdit->append(tr("Server Endpoint found"));
    ui->outputEdit->append(tr("Server's Multicast Address"));
    ui->outputEdit->append(info.controlEndpointAddress().toString());
    ui->outputEdit->append(tr("Server's Port"));
    ui->outputEdit->append(QString::number(info.controlEndpointPort()));
    ui->outputEdit->append(tr("The following services are supported:"));
    const auto services = info.supportedServices();
    for (const auto service : services) {
        ui->outputEdit->append(tr("    KNXnet/IP %1, Version: %2")
            .arg(familieToString(service.ServiceFamily)).arg(service.ServiceFamilyVersion));
    }

    ui->serverBox->addItem(tr("%1 (%2:%3)").arg(info.deviceName(), info.controlEndpointAddress()
        .toString()).arg(info.controlEndpointPort()), QVariant::fromValue(info));
}

void MainWindow::fillLocalIpBox()
{
    auto firstItem = new QStandardItem(tr("Interface: IP address --Select One--"));
    qobject_cast<QStandardItemModel*>(ui->localIpBox->model())->appendRow(firstItem);
    firstItem->setSelectable(false);

    auto networkInterfaces = QNetworkInterface::allInterfaces();
    for (int i = 0; i < networkInterfaces.size(); i++) {
        auto addressList = networkInterfaces[i].addressEntries();
        for (int j = 0; j < addressList.size(); j++) {
            auto address = addressList[j].ip();
            if (address.isLoopback() || address.toIPv4Address() == 0)
                continue;
            auto ipAddress = address.toString();
            ui->localIpBox->addItem(networkInterfaces[i].name() + ": " + ipAddress, ipAddress);
        }
    }
}

void MainWindow::messageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    class Uptime : public QElapsedTimer
    {
    public:
        Uptime() { start(); }
        quint64 lastEvent { 0 };
    };
    static Uptime uptime;

    auto msec = uptime.elapsed();
    if (s_ui) {
        s_ui->outputEdit->append(QStringLiteral("[elapsed: %1 msec, diff: %2 msec]\n    %3")
            .arg(msec) .arg(msec - uptime.lastEvent).arg(msg));
    }
    uptime.lastEvent = msec;

    if (type == QtFatalMsg) {
        auto oldMsgHandler = qInstallMessageHandler(0);
        qt_message_output(type, ctx, msg);
        qInstallMessageHandler(oldMsgHandler);
    }
}
