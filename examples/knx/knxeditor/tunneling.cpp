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

#include "tunneling.h"
#include "ui_tunneling.h"

#include <QKnxDeviceManagementFrameFactory>
#include <QKnxInterfaceObjectPropertyDataType>
#include <QMetaEnum>
#include <QMetaType>
#include <QStandardItemModel>
#include <QTreeView>

Tunneling::Tunneling(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Tunneling)
{
    ui->setupUi(this);
    setupMessageCodeComboBox();
    updateAdditionalInfoTypesComboBox();

    connect(ui->connectTunneling, &QPushButton::clicked, this, [&]() {
        m_tunnel.setLocalPort(0);
        m_tunnel.connectToHost(m_server.controlEndpointAddress(), m_server.controlEndpointPort());
    });

    connect(&m_tunnel, &QKnxNetIpTunnelConnection::connected, this, [&] {
        ui->connectTunneling->setEnabled(false);
        ui->disconnectTunneling->setEnabled(true);
        ui->tunnelingSendRequest->setEnabled(true);
        ui->textOuputTunneling->append("Successful connected to: " + m_server
            .controlEndpointAddress().toString() + " on port: " + QString::number(m_server
                .controlEndpointPort()));
    });

    connect(ui->disconnectTunneling, &QPushButton::clicked, this, [&]() {
        m_tunnel.disconnectFromHost();
    });

    connect(&m_tunnel, &QKnxNetIpTunnelConnection::disconnected, this, [&] {
        ui->connectTunneling->setEnabled(true);
        ui->disconnectTunneling->setEnabled(false);
        ui->tunnelingSendRequest->setEnabled(false);
        ui->textOuputTunneling->append(QStringLiteral("Successfully disconnected from: %1"
            " on port: %2\n").arg(m_server.controlEndpointAddress().toString())
            .arg(m_server.controlEndpointPort()));
    });

    connect(ui->tunnelingSendRequest, &QPushButton::clicked, this, [&]() {
        ui->textOuputTunneling->append("Send tunnel frame with cEMI payload: "
            + ui->m_cemiFrame->text());
        auto data = QByteArray::fromHex(ui->m_cemiFrame->text().toLatin1());
        m_tunnel.sendTunnelFrame(QKnxCemiFrame::fromBytes(data, 0, data.size()));
    });

    connect(&m_tunnel, &QKnxNetIpTunnelConnection::receivedTunnelFrame, this,
        [&](QKnxTunnelFrame frame) {
        ui->textOuputTunneling->append(QString::fromLatin1("Source address: %1").arg(frame
            .sourceAddress().toString()));
        ui->textOuputTunneling->append(QString::fromLatin1("Destination address: %1").arg(frame
            .destionationAddress().toString()));
        ui->textOuputTunneling->append(QString::fromLatin1("Received tunnel frame with cEMI "
            "payload: " + frame.bytes().toHex()));
    });

    updateControlField();
    updateExtendedControlField();

    connect(ui->mc, &QComboBox::currentTextChanged, this, &Tunneling::updateFrame);

    connect(ui->frameType, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        if (text == "Extended") {
            for (int i = 0; i < ui->additionalInfosList->count(); ++i) {
                auto b = ui->additionalInfosList->item(i)->text().toLatin1();
                m_frame.addAdditionalInfo(QKnxAdditionalInfo::fromBytes(QByteArray::fromHex(b), 0));
            }
            ui->additionalInfo->setEnabled(true);
        } else {
            m_frame.clearAdditionalInfos();
            ui->additionalInfo->setEnabled(false);
        }
        updateControlField();
    });
    connect(ui->repeat, &QComboBox::currentTextChanged, this, &Tunneling::updateControlField);
    connect(ui->broadcast, &QComboBox::currentTextChanged, this, &Tunneling::updateControlField);
    connect(ui->priority, &QComboBox::currentTextChanged, this, &Tunneling::updateControlField);
    connect(ui->acknowledge, &QComboBox::currentTextChanged, this, &Tunneling::updateControlField);
    connect(ui->confirm, &QComboBox::currentTextChanged, this, &Tunneling::updateControlField);

    connect(ui->destAddrType, (&QComboBox::currentTextChanged), this, [&](const QString &text) {
        QLatin1Char delimiter(text == "Group" ? '/' : '.');
        ui->destAddress->setValidator(new KnxAddressValidator(delimiter));
        ui->destAddress->setText(ui->destAddress->text().replace(QRegularExpression("\\/|\\."),
            QString(delimiter)));
        updateExtendedControlField();
    });
    using overload = void (QSpinBox::*)(int);
    connect(ui->hopCount, static_cast<overload>(&QSpinBox::valueChanged), this,
        &Tunneling::updateExtendedControlField);
    connect(ui->extFrameFormat, &QComboBox::currentTextChanged, this,
        &Tunneling::updateExtendedControlField);

    connect(ui->additionalInfoTypes, &QComboBox::currentTextChanged, this, [&]() {
        bool fixed = true;
        auto type = QKnxAdditionalInfo::Type(ui->additionalInfoTypes->currentData().toUInt());
        int size = QKnxAdditionalInfo::expectedDataSize(type, &fixed);
        ui->additionallnfoData->setMaxLength((fixed ? size : 252) * 2);
    });

    connect(ui->addAdditionalInfo, &QPushButton::clicked, this, [&]() {
        auto type = QKnxAdditionalInfo::Type(ui->additionalInfoTypes->currentData().toUInt());
        auto info = QKnxAdditionalInfo(type, QByteArray::fromHex(ui->additionallnfoData->text()
            .toLatin1()));
        if (info.isValid()) {
            m_frame.addAdditionalInfo(info);
            ui->m_cemiFrame->setText(m_frame.bytes().toHex());
            ui->additionalInfosList->addItem(new QListWidgetItem(info.bytes().toHex()));
        }
    });

    connect(ui->removeAdditionalInfo, &QPushButton::clicked, this, [&]() {
        auto index = ui->additionalInfosList->currentIndex();
        if (index.isValid()) {
            auto currentItem = ui->additionalInfosList->item(index.row());
            auto b = currentItem->text().toLatin1();
            m_frame.removeAdditionalInfo(QKnxAdditionalInfo::fromBytes(QByteArray::fromHex(b), 0));
            ui->m_cemiFrame->setText(m_frame.bytes().toHex());
            delete ui->additionalInfosList->takeItem(index.row());
        }
    });

    connect(ui->sourceAddress, &QLineEdit::textChanged, this, &Tunneling::updateFrame);
    connect(ui->destAddress, &QLineEdit::textChanged, this, &Tunneling::updateFrame);
    connect(ui->npdu, &QLineEdit::textChanged, this, &Tunneling::updateFrame);

    connect(ui->manualInput, &QCheckBox::clicked, this, [&](bool checked) {
        if (!checked)
            checked = ui->frameType->currentText() == "Standard";
        ui->additionalInfo->setDisabled(checked);
    });

    ui->destAddress->setValidator(new KnxAddressValidator(QLatin1Char('/')));
    ui->sourceAddress->setValidator(new KnxAddressValidator(QLatin1Char('.')));
    ui->npdu->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]+")));
    ui->additionallnfoData->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]+")));
}

Tunneling::~Tunneling()
{
    delete ui;
}

void Tunneling::setNatAware(bool isNatAware)
{
    m_tunnel.setNatAware(isNatAware);
}

void Tunneling::setLocalAddress(const QHostAddress &address)
{
    m_tunnel.disconnectFromHost();
    m_tunnel.setLocalAddress(address);
}

void Tunneling::setKnxNetIpServer(const QKnxNetIpServerInfo &server)
{
    m_tunnel.disconnectFromHost();
    m_server = server;

    if (m_tunnel.state() == QKnxNetIpEndpointConnection::State::Disconnected) {
        ui->connectTunneling->setEnabled(true);
        ui->disconnectTunneling->setEnabled(false);
    }
    ui->tunnelingSendRequest->setEnabled(false);
}

void Tunneling::clearLogging()
{
    ui->textOuputTunneling->clear();
}

void Tunneling::updateFrame()
{
    m_frame.setMessageCode(QKnxCemiFrame::MessageCode(ui->mc->currentData().toUInt()));
    m_frame.setControlField(m_ctrl);
    m_frame.setExtendedControlField(m_extCtrl);
    m_frame.setSourceAddress({ QKnxAddress::Type::Individual, ui->sourceAddress->text() });
    m_frame.setDestionationAddress({ m_extCtrl.destinationAddressType(), ui->destAddress->text() });

    auto npdu = QByteArray::fromHex(ui->npdu->text().toLatin1());
    m_frame.setNetworkLayerProtocolDataUnit(QKnxNpdu::fromBytes(npdu, 0, npdu.size()));
    ui->m_cemiFrame->setText(m_frame.bytes().toHex());
}

void Tunneling::updateControlField()
{
    m_ctrl.setFrameType(QKnxControlField::FrameType(ui->frameType->currentIndex()));
    m_ctrl.setRepeat(QKnxControlField::Repeat(ui->repeat->currentIndex()));
    m_ctrl.setBroadcast(QKnxControlField::Broadcast(ui->broadcast->currentIndex()));
    m_ctrl.setPriority(QKnxControlField::Priority(ui->priority->currentIndex()));
    m_ctrl.setAcknowledge(QKnxControlField::Acknowledge(ui->acknowledge->currentIndex()));
    m_ctrl.setConfirm(QKnxControlField::Confirm(ui->confirm->currentIndex()));

    updateFrame();
}

void Tunneling::updateExtendedControlField()
{
    m_extCtrl.setDestinationAddressType(QKnxAddress::Type(ui->destAddrType->currentIndex()));
    m_extCtrl.setHopCount(ui->hopCount->value());
    m_extCtrl.setFormat(QKnxExtendedControlField::ExtendedFrameFormat(ui->extFrameFormat->currentIndex()));

    updateFrame();
}

void Tunneling::on_manualInput_clicked(bool checked)
{
    ui->m_cemiFrame->setReadOnly(!checked);
    ui->m_cemiFrame->setMaxLength(SHRT_MAX);
    ui->m_cemiFrame->setFocus();
}

void Tunneling::setupMessageCodeComboBox()
{
    ui->mc->addItem("L_Data.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::DataRequest));
    ui->mc->addItem("L_Raw.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::RawRequest));
    ui->mc->addItem("L_Poll_Data.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::PollDataRequest));
    ui->mc->addItem("T_Data_Connected.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::DataConnectedRequest));
    ui->mc->addItem("T_Data_Individual.req",
        quint8(QKnxDeviceManagementFrame::MessageCode::DataIndividualRequest));
}

void Tunneling::updateAdditionalInfoTypesComboBox()
{
    int index = QKnxAdditionalInfo::staticMetaObject.indexOfEnumerator("Type");
    auto typeEnum = QKnxAdditionalInfo::staticMetaObject.enumerator(index);
    if (!typeEnum.isValid())
        return;

    auto model = static_cast<QStandardItemModel*> (ui->additionalInfoTypes->model());
    for (int row = 0; row < model->rowCount(); ++row) {
        auto data = model->itemData(model->index(row, 0));
        data.insert(Qt::UserRole, typeEnum.value(row));
        model->setItemData(model->index(row, 0), data);
    }
    model->item(0)->setEnabled(false);
    model->item(model->rowCount() - 1)->setEnabled(false);
}
