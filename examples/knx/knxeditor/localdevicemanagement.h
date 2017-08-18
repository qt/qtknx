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

#ifndef LOCALDEVICEMANAGEMENT_H
#define LOCALDEVICEMANAGEMENT_H

#include <QWidget>
#include <QKnxNetIpDeviceManagementConnection>
#include <QKnxNetIpServerDiscoveryInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
    class LocalDeviceManagement;
}

class QComboBox;
QT_END_NAMESPACE

class LocalDeviceManagement : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LocalDeviceManagement)

public:
    explicit LocalDeviceManagement(QWidget* parent = nullptr);
    ~LocalDeviceManagement();

    void setNatAware(bool isNatAware);
    void setLocalAddress(const QHostAddress &address);
    void setKnxNetIpServer(const QKnxNetIpServerDiscoveryInfo &server);

private slots:
    void on_mc_currentIndexChanged(int index);
    void on_objectType_currentTextChanged(const QString &type);
    void on_property_currentTextChanged(const QString &property);

    void on_noe_valueChanged(int value);
    void on_startIndex_valueChanged(int value);
    void on_objectInstance_valueChanged(int value);

    void on_manualInput_clicked(bool checked);

private:
    void setupMessageCodeComboBox();
    void handleIoListResponse(const QKnxDeviceManagementFrame &frame);
    int keyToValue(const QMetaObject &object, const QString &key, bool *ok);
    void setupComboBox(const QMetaObject &object, QComboBox *comboBox, const QSet<int> &values = {});

private:
    Ui::LocalDeviceManagement *ui { nullptr };

    QString m_fullCemiFrame;
    bool m_awaitIoListResponse { true };

    QKnxNetIpServerDiscoveryInfo m_server;
    QKnxNetIpDeviceManagementConnection m_management;
};

#endif
