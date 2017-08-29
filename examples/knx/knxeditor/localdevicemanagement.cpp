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

#include "localdevicemanagement.h"
#include "ui_localdevicemanagement.h"

#include <QKnxDeviceManagementFrameFactory>
#include <QKnxInterfaceObjectPropertyDataType>
#include <QMetaEnum>
#include <QMetaType>
#include <QStandardItemModel>
#include <QTreeView>

LocalDeviceManagement::LocalDeviceManagement(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LocalDeviceManagement)
{
    ui->setupUi(this);

    setupMessageCodeComboBox();
    setupMessageCodeComboBox(QKnxInterfaceObjectType::staticMetaObject, ui->objectType);
    setupMessageCodeComboBox(QKnxInterfaceObjectProperty::staticMetaObject, ui->property);

    connect(ui->connectRequestDeviceManagement, &QPushButton::clicked, this, [&]() {
        m_management.setLocalPort(0);
        m_management.connectToHost(m_server.controlEndpointAddress(), m_server.controlEndpointPort());
    });

    connect(&m_management, &QKnxNetIpDeviceManagementConnection::connected, this, [&] {
        ui->deviceManagementSendRequest->setEnabled(true);
        ui->connectRequestDeviceManagement->setEnabled(false);
        ui->disconnectRequestDeviceManagement->setEnabled(true);
        ui->textOuputDeviceManagement->append("Successful connected to: " + m_server
            .controlEndpointAddress().toString() + " on port: " + QString::number(m_server
                .controlEndpointPort()));

        m_management.sendDeviceManagementFrame(QKnxDeviceManagementFrameFactory::PropertyRead
            ::createRequest(QKnxInterfaceObjectType::System::Device, 1,
                QKnxInterfaceObjectProperty::Device::IoList, 1, 0));
    });

    connect(ui->disconnectRequestDeviceManagement, &QPushButton::clicked, this, [&]() {
        m_management.disconnectFromHost();
    });

    connect(&m_management, &QKnxNetIpDeviceManagementConnection::disconnected, this, [&] {
        m_awaitIoListResponse = true;
        ui->deviceManagementSendRequest->setEnabled(false);
        ui->connectRequestDeviceManagement->setEnabled(true);
        ui->disconnectRequestDeviceManagement->setEnabled(false);
        setupMessageCodeComboBox(QKnxInterfaceObjectType::staticMetaObject, ui->objectType);
        ui->textOuputDeviceManagement->append(QStringLiteral("Successfully disconnected from: %1"
            " on port: %2\n").arg(m_server.controlEndpointAddress().toString())
            .arg(m_server.controlEndpointPort()));
    });

    connect(ui->deviceManagementSendRequest, &QPushButton::clicked, this, [&]() {
        ui->textOuputDeviceManagement->append("Send device management frame with cEMI payload: "
            + ui->m_cemiFrame->text());
        auto data = QByteArray::fromHex(ui->m_cemiFrame->text().toUtf8());
        if (ui->m_cemiData->isEnabled())
            data.append(QByteArray::fromHex(ui->m_cemiData->text().toUtf8()));
        m_management.sendDeviceManagementFrame(QKnxCemiFrame::fromBytes(data, 0, data.size()));
    });

    connect(&m_management, &QKnxNetIpDeviceManagementConnection::receivedDeviceManagementFrame,
        this, [&](QKnxDeviceManagementFrame frame) {
        ui->textOuputDeviceManagement->append(QString::fromUtf8("Received device management "
            "frame with cEMI payload: " + frame.bytes().toHex()));

        if (m_awaitIoListResponse)
            handleIoListResponse(frame);
    });

    ui->m_cemiData->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]+")));
    ui->m_cemiFrame->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]+")));
}

LocalDeviceManagement::~LocalDeviceManagement()
{
    delete ui;
}

void LocalDeviceManagement::setNatAware(bool isNatAware)
{
    m_management.setNatAware(isNatAware);
}

void LocalDeviceManagement::setLocalAddress(const QHostAddress &address)
{
    m_management.disconnectFromHost();
    m_management.setLocalAddress(address);
}

void LocalDeviceManagement::setKnxNetIpServer(const QKnxNetIpServerDiscoveryInfo &server)
{
    m_management.disconnectFromHost();
    m_server = server;

    if (m_management.state() == QKnxNetIpEndpointConnection::State::Disconnected) {
        ui->connectRequestDeviceManagement->setEnabled(true);
        ui->disconnectRequestDeviceManagement->setEnabled(false);
    }
    ui->deviceManagementSendRequest->setEnabled(false);
}

void LocalDeviceManagement::clearLogging()
{
    ui->textOuputDeviceManagement->clear();
}

void LocalDeviceManagement::on_mc_currentIndexChanged(int index)
{
    if (ui->m_cemiFrame->text().size() > 2)
        m_fullCemiFrame = ui->m_cemiFrame->text();

    int maxLength = 10;
    bool dataEnabled = true;
    auto cemiFrame = m_fullCemiFrame;

    quint8 data = ui->mc->itemData(index).toUInt();
    switch (QKnxCemiFrame::MessageCode(data)) {
    case QKnxCemiFrame::MessageCode::PropertyReadRequest:
        dataEnabled = false;
    case QKnxCemiFrame::MessageCode::PropertyWriteRequest:
        maxLength = 14;
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandRequest:
    case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadRequest:
        if (cemiFrame.size() < maxLength) {
            cemiFrame.append(QStringLiteral("%1").arg(ui->noe->value(), 1, 16, QChar('0')));
            cemiFrame.append(QStringLiteral("%1").arg(ui->startIndex->value(), 3, 16, QChar('0')));
        }
        cemiFrame = QStringLiteral("%1").arg(data, 2, 16, QChar('0')) + cemiFrame.mid(2);
        break;
    case QKnxCemiFrame::MessageCode::ResetRequest:
        maxLength = 2;
        dataEnabled = false;
        cemiFrame = QStringLiteral("%1").arg(data, 2, 16, QChar('0'));
        break;
    default:
        break;
    }
    ui->m_cemiFrame->setMaxLength(maxLength);
    ui->m_cemiFrame->setText(cemiFrame);
    ui->m_cemiData->setEnabled(dataEnabled);
}

void LocalDeviceManagement::on_objectType_currentTextChanged(const QString &type)
{
    bool keyExists = false;
    quint16 value = keyToValue(QKnxInterfaceObjectType::staticMetaObject, type, &keyExists);
    if (keyExists) {
        auto text = ui->m_cemiFrame->text();
        ui->m_cemiFrame->setText(text.left(2) + QString("%1").arg(value, 4, 16, QChar('0'))
            + text.mid(6));
    }
}

void LocalDeviceManagement::on_property_currentTextChanged(const QString &property)
{
    bool keyExists = false;
    quint8 value = keyToValue(QKnxInterfaceObjectProperty::staticMetaObject, property, &keyExists);
    if (keyExists) {
        auto text = ui->m_cemiFrame->text();
        ui->m_cemiFrame->setText(text.left(8) + QString("%1").arg(value, 2, 16, QChar('0'))
            + text.mid(10));
    }
}

void LocalDeviceManagement::on_noe_valueChanged(int value)
{
    auto text = ui->m_cemiFrame->text();
    ui->m_cemiFrame->setText(text.left(10) + QString("%1").arg(value, 1, 16, QChar('0'))
        + text.mid(11));
}

void LocalDeviceManagement::on_startIndex_valueChanged(int value)
{
    auto text = ui->m_cemiFrame->text();
    ui->m_cemiFrame->setText(text.left(11) + QString("%1").arg(value, 3, 16, QChar('0')));
}

void LocalDeviceManagement::on_objectInstance_valueChanged(int value)
{
    auto text = ui->m_cemiFrame->text();
    ui->m_cemiFrame->setText(text.left(6) + QString("%1").arg(value, 2, 16, QChar('0'))
        + text.mid(8));
}

void LocalDeviceManagement::on_manualInput_clicked(bool checked)
{
    ui->m_cemiData->setEnabled(checked);
    ui->m_cemiFrame->setReadOnly(!checked);
    ui->m_cemiFrame->setMaxLength(SHRT_MAX);
    ui->m_cemiFrame->setFocus();

    if (checked) {
        if (ui->mc->currentIndex() != ui->mc->count() -1)
            m_fullCemiFrame = ui->m_cemiFrame->text();
    } else {
        ui->m_cemiFrame->setText(m_fullCemiFrame);
        on_mc_currentIndexChanged(ui->mc->currentIndex());
    }
}

void LocalDeviceManagement::setupMessageCodeComboBox()
{
    ui->mc->addItem("M_PropRead.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest));
    ui->mc->addItem("M_PropWrite.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::PropertyWriteRequest));
    ui->mc->addItem("M_FuncPropCommand.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::FunctionPropertyCommandRequest));
    ui->mc->addItem("M_FuncPropStateRead.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::FunctionPropertyStateReadRequest));
    ui->mc->addItem("M_Reset.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::ResetRequest));
}

void LocalDeviceManagement::handleIoListResponse(const QKnxDeviceManagementFrame &frame)
{
    if (frame.objectType() != QKnxInterfaceObjectType::System::Device
        || frame.property() != QKnxInterfaceObjectProperty::Device::IoList)
        return;

    m_awaitIoListResponse = true;

    auto dataTypes = QKnxInterfaceObjectPropertyDataType::fromProperty(QKnxInterfaceObjectProperty::Device::IoList);
    if (!dataTypes.value(0).isValid())
        return;

    auto data = frame.data();
    quint8 expectedDataSize = dataTypes[0].size();
    if (frame.startIndex() == 0) {
        if (data.size() == expectedDataSize) {
            m_management.sendDeviceManagementFrame(QKnxDeviceManagementFrameFactory::PropertyRead
                ::createRequest(QKnxInterfaceObjectType::System::Device, 1,
                    QKnxInterfaceObjectProperty::Device::IoList, data.toHex().toUShort(nullptr,
                        16), 1));
        }
    } else {
        if ((data.size() % expectedDataSize) == 0) {
            QSet<int> values;
            for (int i = 0; i < data.size(); i += expectedDataSize)
                values.insert(data.mid(i, expectedDataSize).toHex().toUShort(nullptr, 16));
            ui->objectType->clear();
            setupMessageCodeComboBox(QKnxInterfaceObjectType::staticMetaObject, ui->objectType, values);
        }
    }
}

int LocalDeviceManagement::keyToValue(const QMetaObject &object, const QString &key, bool *ok)
{
    auto enumCount = object.enumeratorCount();
    for (auto i = 0; i < enumCount; ++i) {
        int value = object.enumerator(i).keyToValue(key.toLatin1(), ok);
        if (value != -1 && ok)
            return value;
    }
    return -1;
}

void LocalDeviceManagement::setupMessageCodeComboBox(const QMetaObject &object, QComboBox *comboBox,
    const QSet<int> &values)
{
    auto model = new QStandardItemModel;
    auto enumCount = object.enumeratorCount();
    for (auto i = 0; i < enumCount; ++i) {
        auto typeEnum = object.enumerator(i);
        auto topLevelItem = new QStandardItem(typeEnum.name());
        for (auto a = 0; a < typeEnum.keyCount(); ++a) {
            auto subItem = new QStandardItem(typeEnum.key(a));
            subItem->setEnabled(values.isEmpty() ? true : values.contains(typeEnum.value(a)));
            topLevelItem->insertRow(a, subItem);
        }
        model->setItem(i, topLevelItem);
        topLevelItem->setSelectable(false);
    }
    comboBox->setModel(model);

    auto treeView = new QTreeView;
    treeView->setHeaderHidden(true);
    treeView->setUniformRowHeights(true);
    comboBox->setView(treeView);
    treeView->expand(model->index(0, 0));
}
