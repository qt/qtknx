/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QKNXNETIPSECURECONFIGURATION_H
#define QKNXNETIPSECURECONFIGURATION_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxsecurekey.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpSecureConfigurationPrivate;
class Q_KNX_EXPORT QKnxNetIpSecureConfiguration
{
public:
    enum class Type : quint8
    {
        Tunneling = 0x00,
        DeviceManagement = 001
    };

    QKnxNetIpSecureConfiguration();
    ~QKnxNetIpSecureConfiguration();

    static QList<QKnxNetIpSecureConfiguration> fromKeyring(QKnxNetIpSecureConfiguration::Type type,
        const QString &keyring, const QByteArray &password, bool validate);

    static QKnxNetIpSecureConfiguration fromKeyring(QKnxNetIpSecureConfiguration::Type type,
        const QKnxAddress &ia, const QString &keyring, const QByteArray &password, bool validate);

    bool isNull() const;
    bool isValid() const;

    QKnxSecureKey publicKey() const;

    QKnxAddress host() const;
    void setHost(const QKnxAddress &host);

    QKnxSecureKey privateKey() const;
    bool setPrivateKey(const QKnxSecureKey &key);

    QKnxNetIp::SecureUserId userId() const;
    bool setUserId(QKnxNetIp::SecureUserId userId);

    QByteArray userPassword() const;
    void setUserPassword(const QByteArray &userPassword);

    QKnxAddress individualAddress() const;
    bool setIndividualAddress(const QKnxAddress &address);

    QByteArray deviceAuthenticationCode() const;
    bool setDeviceAuthenticationCode(const QByteArray &authenticationCode);

    bool isSecureSessionKeepAliveSet() const;
    void setKeepSecureSessionAlive(bool keepAlive);

    QKnxNetIpSecureConfiguration(const QKnxNetIpSecureConfiguration &other);
    QKnxNetIpSecureConfiguration &operator=(const QKnxNetIpSecureConfiguration &other);

    QKnxNetIpSecureConfiguration(QKnxNetIpSecureConfiguration &&other) Q_DECL_NOTHROW;
    QKnxNetIpSecureConfiguration &operator=(QKnxNetIpSecureConfiguration &&other) Q_DECL_NOTHROW;

    void swap(QKnxNetIpSecureConfiguration &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxNetIpSecureConfiguration &other) const;
    bool operator!=(const QKnxNetIpSecureConfiguration &other) const;

private:
    friend class QKnxNetIpEndpointConnection;
    friend class QKnxNetIpEndpointConnectionPrivate;
    QSharedDataPointer<QKnxNetIpSecureConfigurationPrivate> d;
};
Q_DECLARE_SHARED(QKnxNetIpSecureConfiguration)

QT_END_NAMESPACE

#endif
