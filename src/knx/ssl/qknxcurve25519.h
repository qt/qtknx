/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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
******************************************************************************/

#ifndef QKNXCURVE25519_H
#define QKNXCURVE25519_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxnetipframe.h>

QT_BEGIN_NAMESPACE

class QKnxCurve25519KeyData;
class QKnxCurve25519PrivateKey;

class Q_KNX_EXPORT QKnxCurve25519PublicKey final
{
public:
    QKnxCurve25519PublicKey();
    ~QKnxCurve25519PublicKey();

    QKnxCurve25519PublicKey(const QKnxCurve25519PrivateKey &key);

    bool isNull() const;
    bool isValid() const;

    QKnxByteArray bytes() const;
    static QKnxCurve25519PublicKey fromBytes(const QKnxByteArray &data, quint16 index = 0);

    QKnxCurve25519PublicKey(const QKnxCurve25519PublicKey &other);
    QKnxCurve25519PublicKey &operator=(const QKnxCurve25519PublicKey &other);

private:
    friend class QKnxCryptographicEngine;
    QSharedDataPointer<QKnxCurve25519KeyData> d_ptr;
};

class Q_KNX_EXPORT QKnxCurve25519PrivateKey final
{
public:
    QKnxCurve25519PrivateKey();
    ~QKnxCurve25519PrivateKey();

    bool isNull() const;
    bool isValid() const;

    QKnxByteArray bytes() const;
    static QKnxCurve25519PrivateKey fromBytes(const QKnxByteArray &data, quint16 index = 0);

    QKnxCurve25519PrivateKey(const QKnxCurve25519PrivateKey &other);
    QKnxCurve25519PrivateKey &operator=(const QKnxCurve25519PrivateKey &other);

private:
    friend class QKnxCurve25519PublicKey;
    friend class QKnxCryptographicEngine;
    QSharedDataPointer<QKnxCurve25519KeyData> d_ptr;
};

class Q_KNX_EXPORT QKnxCryptographicEngine final
{
public:
    enum class Mode : quint8 {
        Decrypt = 0x00,
        Encrypt = 0x01
    };

    QKnxCryptographicEngine() = delete;
    ~QKnxCryptographicEngine() = default;

    static QKnxByteArray sharedSecret(const QKnxCurve25519PublicKey &pub,
                                      const QKnxCurve25519PrivateKey &priv);

    static QKnxByteArray sessionKey(const QKnxByteArray &sharedSecret);
    static QKnxByteArray sessionKey(const QKnxCurve25519PublicKey &pub,
                                    const QKnxCurve25519PrivateKey &priv);

    static QKnxByteArray userPasswordHash(const QByteArray &password);
    static QKnxByteArray deviceAuthenticationCodeHash(const QByteArray &password);

    static QKnxByteArray messageAuthenticationCode(QKnxCryptographicEngine::Mode mode,
                                                   quint16 secureSessionId,
                                                   const QKnxNetIpFrameHeader &header,
                                                   const QKnxCurve25519PublicKey &client,
                                                   const QKnxCurve25519PublicKey &server,
                                                   const QKnxByteArray &deviceAuthenticationCode);

    static QKnxByteArray pkcs5Pbkdf2HmacSha256(const QByteArray &password, const QKnxByteArray &salt,
        qint32 iterations, quint8 derivedKeyLength);
};

QT_END_NAMESPACE

#endif

/*
b752be246459260f6b0c4801fbd5a67599f83b4057b3ef1e79e469ac17234e15
b752be246459260f6b0c4801fbd5a67599f83b4057b3ef1e79e469ac17234e15

061009520038 0001 b752be246459260f6b0c4801fbd5a67599f83b4057b3ef1e79e469ac17234e15
*/
