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

#ifndef TUNNELING_H
#define TUNNELING_H

#include <QKnxControlField>
#include <QKnxExtendedControlField>
#include <QKnxNetIpTunnelConnection>
#include <QKnxNetIpServerDiscoveryInfo>
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
    void setKnxNetIpServer(const QKnxNetIpServerDiscoveryInfo &server);

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
    QKnxNetIpServerDiscoveryInfo m_server;
};

#endif
