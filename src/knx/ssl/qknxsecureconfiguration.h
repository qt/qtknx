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

#ifndef QKNXSECURECONFIGURATION_H
#define QKNXSECURECONFIGURATION_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxsecurekey.h>

QT_BEGIN_NAMESPACE

class QKnxSecureConfigurationPrivate;
class Q_KNX_EXPORT QKnxSecureConfiguration
{
public:
    QKnxSecureConfiguration();
    ~QKnxSecureConfiguration();

    bool isNull() const;
    bool isValid() const;

    QKnxSecureKey publicKey() const;

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

    QKnxSecureConfiguration(const QKnxSecureConfiguration &other);
    QKnxSecureConfiguration &operator=(const QKnxSecureConfiguration &other);

    QKnxSecureConfiguration(QKnxSecureConfiguration &&other) Q_DECL_NOTHROW;
    QKnxSecureConfiguration &operator=(QKnxSecureConfiguration &&other) Q_DECL_NOTHROW;

    void swap(QKnxSecureConfiguration &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxSecureConfiguration &other) const;
    bool operator!=(const QKnxSecureConfiguration &other) const;

private:
    friend class QKnxNetIpEndpointConnection;
    friend class QKnxNetIpEndpointConnectionPrivate;
    QSharedDataPointer<QKnxSecureConfigurationPrivate> d;
};
Q_DECLARE_SHARED(QKnxSecureConfiguration)

QT_END_NAMESPACE

#endif
