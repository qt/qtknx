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

#include "tunneling.h"
#include "ui_tunneling.h"

#include <QMetaEnum>
#include <QMetaType>
#include <QStandardItemModel>
#include <QTreeView>

// -- KnxAddressValidator

KnxAddressValidator::KnxAddressValidator(QLatin1Char delimiter, QObject *parent)
    : QValidator(parent)
    , m_delimiter(delimiter)
    , m_expr(QString::fromLatin1("[^0-9\\%1]").arg(delimiter))
{}

QValidator::State KnxAddressValidator::validate(QString & input, int &) const
{
    auto result = m_expr.match(input);
    if (result.hasMatch())
        return State::Invalid;

    auto n = input.split(m_delimiter);
    if (n.value(0).toInt() > 15 || n.value(1).toInt() > 15 || n.value(2).toInt() > 255)
        return State::Invalid;
    return State::Intermediate;
}


// -- Tunneling

Tunneling::Tunneling(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Tunneling)
{
    ui->setupUi(this);

    setupApciTpciComboBox();
    setupMessageCodeComboBox();
    updateAdditionalInfoTypesComboBox();

    m_frame.setMediumType(QKnx::MediumType::NetIP);

    connect(ui->connectTunneling, &QPushButton::clicked, this, [&]() {
        m_tunnel.setLocalPort(0);
        m_tunnel.connectToHost(m_server.controlEndpointAddress(), m_server.controlEndpointPort());
    });

    connect(&m_tunnel, &QKnxNetIpTunnelConnection::connected, this, [&] {
        ui->connectTunneling->setEnabled(false);
        ui->disconnectTunneling->setEnabled(true);
        ui->tunnelingSendRequest->setEnabled(true);
        ui->sourceAddress->setText(m_tunnel.individualAddress().toString());
        ui->textOuputTunneling->append(tr("Successfully connected to: %1 on port: %2").arg(m_server
            .controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()));
    });

    connect(ui->disconnectTunneling, &QPushButton::clicked, this, [&]() {
        m_tunnel.disconnectFromHost();
    });

    connect(&m_tunnel, &QKnxNetIpTunnelConnection::disconnected, this, [&] {
        ui->connectTunneling->setEnabled(true);
        ui->disconnectTunneling->setEnabled(false);
        ui->tunnelingSendRequest->setEnabled(false);
        ui->textOuputTunneling->append(tr("Successfully disconnected from: %1 on port: %2\n")
            .arg(m_server.controlEndpointAddress().toString()).arg(m_server.controlEndpointPort()));
    });

    connect(ui->tunnelingSendRequest, &QPushButton::clicked, this, [&]() {
        ui->textOuputTunneling->append(tr("Send tunnel frame with cEMI payload: ")
            + ui->cemiFrame->text());
        auto data = QKnxByteArray::fromHex(ui->cemiFrame->text().toLatin1());
        m_tunnel.sendTunnelFrame(QKnxLinkLayerFrame::fromBytes(data, 0, data.size()));
    });

    connect(&m_tunnel, &QKnxNetIpTunnelConnection::receivedTunnelFrame, this,
        [&](QKnxLinkLayerFrame frame) {
        ui->textOuputTunneling->append(tr("Source address: %1").arg(frame.sourceAddress()
            .toString()));
        ui->textOuputTunneling->append(tr("Destination address: %1").arg(frame.destinationAddress()
            .toString()));
        ui->textOuputTunneling->append(tr("Received tunnel frame with cEMI payload: "
            + static_cast<QByteArray> (frame.bytes().toHex())));
    });

    updateControlField();
    updateExtendedControlField();

    connect(ui->frameFormat, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        if (text == "Extended") {
            for (int i = 0; i < ui->additionalInfosList->count(); ++i) {
                auto b = ui->additionalInfosList->item(i)->text().toLatin1();
                m_frame.addAdditionalInfo(QKnxAdditionalInfo::fromBytes(QKnxByteArray::fromHex(b), 0));
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
        auto info = QKnxAdditionalInfo(type, QKnxByteArray::fromHex(ui->additionallnfoData->text()
            .toLatin1()));
        if (info.isValid()) {
            m_frame.addAdditionalInfo(info);
            ui->cemiFrame->setText(static_cast<QByteArray> (m_frame.bytes()).toHex());
            ui->additionalInfosList->addItem(new QListWidgetItem(static_cast<QByteArray> (info
                .bytes())));
        }
    });

    connect(ui->removeAdditionalInfo, &QPushButton::clicked, this, [&]() {
        auto index = ui->additionalInfosList->currentIndex();
        if (index.isValid()) {
            auto currentItem = ui->additionalInfosList->item(index.row());
            auto b = currentItem->text().toLatin1();
            m_frame.removeAdditionalInfo(QKnxAdditionalInfo::fromBytes(QKnxByteArray::fromHex(b),
                0));
            ui->cemiFrame->setText(static_cast<QByteArray> (m_frame.bytes().toHex()));
            delete ui->additionalInfosList->takeItem(index.row());
        }
    });

    connect(ui->mc, &QComboBox::currentTextChanged, this, &Tunneling::updateFrame);
    connect(ui->sourceAddress, &QLineEdit::textChanged, this, &Tunneling::updateFrame);
    connect(ui->destAddress, &QLineEdit::textChanged, this, &Tunneling::updateFrame);
    connect(ui->apci, &QComboBox::currentTextChanged, this, &Tunneling::updateFrame);
    connect(ui->tpci, &QComboBox::currentTextChanged, this, &Tunneling::updateFrame);
    connect(ui->data, &QLineEdit::textChanged, this, &Tunneling::updateFrame);

    connect(ui->manualInput, &QCheckBox::clicked, this, [&](bool checked) {
        bool disable = checked;
        if (!checked)
            disable = ui->frameFormat->currentText() == "Standard";
        ui->additionalInfo->setDisabled(disable);

        if (!checked) {
            disable = ui->tpci->itemData(ui->tpci->currentIndex())
                .value<QKnxTpdu::TransportControlField>() >= QKnxTpdu::TransportControlField::Connect;
        }
        ui->apci->setDisabled(disable);
        ui->data->setDisabled(disable);
    });

    ui->destAddress->setValidator(new KnxAddressValidator(QLatin1Char('/')));
    ui->sourceAddress->setValidator(new KnxAddressValidator(QLatin1Char('.')));
    ui->data->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]+")));
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
    m_frame.setMessageCode(QKnxLinkLayerFrame::MessageCode(ui->mc->currentData().toUInt()));
    m_frame.setControlField(m_ctrl);
    m_frame.setExtendedControlField(m_extCtrl);
    m_frame.setSourceAddress({ QKnxAddress::Type::Individual, ui->sourceAddress->text() });
    m_frame.setDestinationAddress({ m_extCtrl.destinationAddressType(), ui->destAddress->text() });

    auto tpdu = QKnxTpdu { ui->tpci->itemData(ui->tpci->currentIndex())
        .value<QKnxTpdu::TransportControlField>() };

    if (tpdu.transportControlField() < QKnxTpdu::TransportControlField::Connect) {
        ui->apci->setEnabled(true);
        ui->data->setEnabled(true);
        tpdu.setApplicationControlField(ui->apci->itemData(ui->apci->currentIndex())
            .value<QKnxTpdu::ApplicationControlField>());
        tpdu.setData(QKnxByteArray::fromHex(ui->data->text().toLatin1()));
    } else {
        ui->apci->setEnabled(false);
        ui->data->setEnabled(false);
    }
    m_frame.setTpdu(tpdu);
    ui->cemiFrame->setText(static_cast<QByteArray> (m_frame.bytes().toHex()));

    ui->tunnelingSendRequest->setEnabled(m_frame.isValid());
}

void Tunneling::updateControlField()
{
    m_ctrl.setFrameFormat(QKnxControlField::FrameFormat(ui->frameFormat->currentIndex()));
    m_ctrl.setRepeat(QKnxControlField::Repeat(ui->repeat->currentIndex()));
    m_ctrl.setBroadcast(QKnxControlField::Broadcast(ui->broadcast->currentIndex()));
    m_ctrl.setPriority(QKnxControlField::Priority(ui->priority->currentIndex()));
    m_ctrl.setAcknowledge(QKnxControlField::Acknowledge(ui->acknowledge->currentIndex()));
    m_ctrl.setConfirm(QKnxControlField::Confirm(ui->confirm->currentIndex()));

    updateFrame();
}

void Tunneling::updateExtendedControlField()
{
    m_extCtrl.setDestinationAddressType(QKnxAddress::Type(ui->destAddrType->currentText() == "Group"));
    m_extCtrl.setHopCount(ui->hopCount->value());
    m_extCtrl.setFormat(QKnxExtendedControlField::ExtendedFrameFormat(ui->extFrameFormat->currentIndex()));

    updateFrame();
}

void Tunneling::on_manualInput_clicked(bool checked)
{
    ui->cemiFrame->setReadOnly(!checked);
    ui->cemiFrame->setMaxLength(SHRT_MAX);
    ui->cemiFrame->setFocus();
}

void Tunneling::setupApciTpciComboBox()
{
    int index = QKnxTpdu::staticMetaObject.indexOfEnumerator("ApplicationControlField");
    auto typeEnum = QKnxTpdu::staticMetaObject.enumerator(index);
    if (!typeEnum.isValid())
        return;

    QRegularExpression regexp("[A-Z]");
    for (auto i = 0; i < typeEnum.keyCount() - 1; ++i) {
        auto string = QString::fromLatin1(typeEnum.key(i));
        auto index = string.lastIndexOf(regexp);
        if (index > 0) string.insert(index, QLatin1Char('_'));
        ui->apci->addItem(QStringLiteral("A_") + string, typeEnum.value(i));
    }

    index = QKnxTpdu::staticMetaObject.indexOfEnumerator("TransportControlField");
    typeEnum = QKnxTpdu::staticMetaObject.enumerator(index);
    if (!typeEnum.isValid())
        return;

    regexp = QRegularExpression("(?=[A-Z])");
    for (auto i = 0; i < typeEnum.keyCount() - 1; ++i) {
        auto string = QString::fromLatin1(typeEnum.key(i)).split(regexp, QString::SkipEmptyParts);
        ui->tpci->addItem("T_" + string.join(QLatin1Char('_')), typeEnum.value(i));
    }
}

void Tunneling::setupMessageCodeComboBox()
{
    ui->mc->addItem("L_Data.req",
        quint8(QKnxLinkLayerFrame::MessageCode::DataRequest));
    ui->mc->addItem("L_Raw.req",
        quint8(QKnxLinkLayerFrame::MessageCode::RawRequest));
    ui->mc->addItem("M_Reset.req",
        quint8(QKnxLinkLayerFrame::MessageCode::ResetRequest));
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
