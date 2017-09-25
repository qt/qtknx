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

#ifndef LOCALDEVICEMANAGEMENT_H
#define LOCALDEVICEMANAGEMENT_H

#include <QWidget>
#include <QKnxNetIpDeviceManagementConnection>
#include <QKnxNetIpServerInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
    class LocalDeviceManagement;
}

class QComboBox;
class QTreeWidget;
class QTreeWidgetItem;
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
    void setKnxNetIpServer(const QKnxNetIpServerInfo &server);

public slots:
    void clearLogging();

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
    void updatePropertyTypeCombobox(const QString &type);
    void handleIoListResponse(const QKnxLocalDeviceManagementFrame &frame);
    int keyToValue(const QMetaObject &object, const QString &key, bool *ok);
    void setupComboBox(QComboBox *comboBox, const QMetaObject &object, const QSet<int> &values = {});
    void selectFirstSubitem(QTreeWidget *treeView, QTreeWidgetItem *rootItem, QComboBox *comboBox);

private:
    Ui::LocalDeviceManagement *ui { nullptr };

    QString m_fullCemiFrame;
    bool m_awaitIoListResponse { true };

    QKnxNetIpServerInfo m_server;
    QKnxNetIpDeviceManagementConnection m_management;
};

#endif
