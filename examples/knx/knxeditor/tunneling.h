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

#ifndef TUNNELING_H
#define TUNNELING_H

#include <QKnxControlField>
#include <QKnxExtendedControlField>
#include <QKnxNetIpTunnelConnection>
#include <QKnxNetIpServerInfo>
#include <QKnxTunnelFrame>
#include <QValidator>
#include <QRegularExpression>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Tunneling;
}

class QComboBox;
QT_END_NAMESPACE

class KnxAddressValidator : public QValidator
{
    Q_OBJECT

public:
    KnxAddressValidator(QLatin1Char delimiter, QObject *parent = nullptr)
        : QValidator(parent)
        , m_delimiter(delimiter)
        , m_expr(QString::fromLatin1("^(\\d{0,2})\\%1(\\d{0,2})\\%1(\\d{0,3})$")
            .arg(delimiter))
    {}

    QValidator::State validate(QString &input, int &) const override
    {
        if (input.isEmpty())
            return State::Intermediate;

        QStringList number = input.split(m_delimiter);
        if (number.value(0).length() > 2)
            return State::Invalid;
        if (number.value(1).length() > 2)
            return State::Invalid;
        if (number.value(2).length() > 3)
            return State::Invalid;

        auto match = m_expr.match(input);
        if (!match.hasMatch())
            return State::Invalid;

        if (match.captured(1).toInt() <= 15 && match.captured(2).toInt() <= 15 &&
            match.captured(3) <= 255) {
            return State::Acceptable;
        }
        return State::Invalid;
    }

private:
    QChar m_delimiter;
    QRegularExpression m_expr;
};

class Tunneling : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Tunneling)

public:
    explicit Tunneling(QWidget* parent = nullptr);
    ~Tunneling();

    void setNatAware(bool isNatAware);
    void setLocalAddress(const QHostAddress &address);
    void setKnxNetIpServer(const QKnxNetIpServerInfo &server);

public slots:
    void clearLogging();

private slots:
    void updateFrame();
    void updateControlField();
    void updateExtendedControlField();
    void on_manualInput_clicked(bool checked);

private:
    void setupMessageCodeComboBox();
    void updateAdditionalInfoTypesComboBox();

private:
    Ui::Tunneling *ui { nullptr };

    QKnxControlField m_ctrl;
    QKnxExtendedControlField m_extCtrl;

    QKnxTunnelFrame m_frame;
    QKnxNetIpTunnelConnection m_tunnel;
    QKnxNetIpServerInfo m_server;
};

#endif
