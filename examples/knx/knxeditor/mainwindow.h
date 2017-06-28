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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPushButton>
#include <QTextEdit>

#include <QKnxNetIpHPAI>
#include <QKnxNetIpServiceFamiliesDIB>

#include <QKnxNetIpServerDiscoveryAgent>
#include <QKnxNetIpServerDiscoveryInfo>

#include <QKnxNetIpDeviceManagementConnection>
#include <QKnxNetIpTunnelConnection>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_scanButton_clicked();
    void on_connectRequestDeviceManagement_clicked();
    void on_disconnectRequestDeviceManagement_clicked();

    void newIPAddressSelected(int localIpBoxIndex);
    void newServerSelected(int serverBoxIndex);

    void showServerAndServices(const QKnxNetIpServerDiscoveryInfo info);
    void successfulConnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service);
    void successfulDisconnection(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service);

    void on_deviceManagementSendRequest_clicked();

    void on_connectRequestTunneling_clicked();

    void on_disconnectRequestTunneling_clicked();

    void on_tunnelingSendRequest_clicked();

private:
    void tabPageInit();
    void fillLocalIpBox();
    void enablingTab();
    void linkToService(QKnxNetIpServiceFamiliesDib::ServiceFamilieId service,
        QPushButton **connectButton, QPushButton **disconnectButton, QTextEdit **output);

private:
    Ui::MainWindow *ui;

    QHostAddress m_hostAddress;
    QKnxNetIpHpai m_serverControlEndpoint;


    QKnxNetIpServerDiscoveryAgent m_discoveryCommunication;
    QKnxNetIpTunnelConnection m_tunneling;
    QKnxNetIpDeviceManagementConnection m_management;

};

#endif // MAINWINDOW_H
