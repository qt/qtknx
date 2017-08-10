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

#include <QKnxNetIpServerDiscoveryAgent>
#include <QKnxNetIpServerDiscoveryInfo>
#include <QKnxNetIpTunnelConnection>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}

class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newServerSelected(int serverBoxIndex);
    void newIPAddressSelected(int localIpBoxIndex);

    void showServerAndServices(const QKnxNetIpServerDiscoveryInfo &info);

    void on_tunnelingSendRequest_clicked();
    void on_connectRequestTunneling_clicked();
    void on_disconnectRequestTunneling_clicked();

private:
    void fillLocalIpBox();
    void enablingTunnelingTab();

private:
    Ui::MainWindow *ui { nullptr };
    QKnxNetIpTunnelConnection m_tunneling;

    QKnxNetIpServerDiscoveryInfo m_server;
    QKnxNetIpServerDiscoveryAgent m_discoveryAgent;
};

#endif // MAINWINDOW_H
