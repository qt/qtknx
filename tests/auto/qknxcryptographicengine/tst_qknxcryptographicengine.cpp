/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxcurve25519.h>
#include <QtKnx/private/qknxcryptographicdata_p.h>
#include <QtTest/qtest.h>

QT_BEGIN_NAMESPACE

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

QT_END_NAMESPACE

class tst_qknxcryptographicengine : public QObject
{
    Q_OBJECT

private slots:
    void testPublicKey()
    {
        QKnxCurve25519PublicKey key;
        QCOMPARE(key.isNull(), true);
        QCOMPARE(key.isValid(), false);
        QCOMPARE(key.bytes(), QKnxByteArray());

        if (QKnxOpenSsl::sslLibraryVersionNumber() < 0x1010000fL)
            return;

        auto bytes = QKnxByteArray::fromHex("bdf099909923143ef0a5de0b3be3687b"
            "c5bd3cf5f9e6f901699cd870ec1ff824");
        key = QKnxCurve25519PublicKey::fromBytes(bytes);
        QCOMPARE(key.isNull(), false);
        QCOMPARE(key.isValid(), true);
        QCOMPARE(key.bytes(), bytes);

        bytes = QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e"
            "5c4507b5ae55161f1078b1dcfb3cb631");
        key = QKnxCurve25519PublicKey::fromBytes(bytes);
        QCOMPARE(key.isNull(), false);
        QCOMPARE(key.isValid(), true);
        QCOMPARE(key.bytes(), bytes);

        // derive public key from private key
        key = { QKnxCurve25519PrivateKey::fromBytes(QKnxByteArray::fromHex("b8fabd62665d8b9e8a9d"
            "8b1f4bca42c8c2789a6110f50e9dd785b3ede883f378")) };
        QCOMPARE(key.isNull(), false);
        QCOMPARE(key.isValid(), true);
        QCOMPARE(key.bytes(), bytes);
    }

    void testPrivateKey()
    {
        if (QKnxOpenSsl::sslLibraryVersionNumber() >= 0x1010000fL) {
            QKnxCurve25519PrivateKey key;
            QCOMPARE(key.isNull(), false);
            QCOMPARE(key.isValid(), true);
            if (QKnxOpenSsl::sslLibraryVersionNumber() > 0x1010008fL)
                QCOMPARE(key.bytes().size(), 32);

            auto bytes = QKnxByteArray::fromHex("b8fabd62665d8b9e8a9d8b1f4bca42c8"
                "c2789a6110f50e9dd785b3ede883f378");
            key = QKnxCurve25519PrivateKey::fromBytes(bytes);
            QCOMPARE(key.isNull(), false);
            QCOMPARE(key.isValid(), true);
            if (QKnxOpenSsl::sslLibraryVersionNumber() > 0x1010008fL)
                QCOMPARE(key.bytes(), bytes);

            bytes = QKnxByteArray::fromHex("68c1744813f4e65cf10cca671caa1336"
                "a796b4ac40cc5cf2655674225c1e5264");
            key = QKnxCurve25519PrivateKey::fromBytes(bytes);
            QCOMPARE(key.isNull(), false);
            QCOMPARE(key.isValid(), true);
            if (QKnxOpenSsl::sslLibraryVersionNumber() > 0x1010008fL)
                QCOMPARE(key.bytes(), bytes);

        } else {
            QKnxCurve25519PrivateKey key;
            QCOMPARE(key.isNull(), true);
            QCOMPARE(key.isValid(), false);
            QCOMPARE(key.bytes(), QKnxByteArray());
        }
    }

    void testSharedSecret()
    {
        if (QKnxOpenSsl::sslLibraryVersionNumber() < 0x1010000fL)
            return;

        auto pubBytes = QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e"
            "5c4507b5ae55161f1078b1dcfb3cb631");
        auto pubKey = QKnxCurve25519PublicKey::fromBytes(pubBytes);

        auto privBytes = QKnxByteArray::fromHex("68c1744813f4e65cf10cca671caa1336"
            "a796b4ac40cc5cf2655674225c1e5264");
        auto privKey = QKnxCurve25519PrivateKey::fromBytes(privBytes);

        auto secret = QKnxCryptographicEngine::sharedSecret(pubKey, privKey);
        QCOMPARE(secret, QKnxByteArray::fromHex("d801525217618f0da90a4ff22148aee0"
            "ff4c19b430e8081223ffe99c81a98b05"));

        pubBytes = QKnxByteArray::fromHex("bdf099909923143ef0a5de0b3be3687b"
            "c5bd3cf5f9e6f901699cd870ec1ff824");
        pubKey = QKnxCurve25519PublicKey::fromBytes(pubBytes);

        privBytes = QKnxByteArray::fromHex("b8fabd62665d8b9e8a9d8b1f4bca42c8"
            "c2789a6110f50e9dd785b3ede883f378");
        privKey = QKnxCurve25519PrivateKey::fromBytes(privBytes);

        secret = QKnxCryptographicEngine::sharedSecret(pubKey, privKey);
        QCOMPARE(secret, QKnxByteArray::fromHex("d801525217618f0da90a4ff22148aee0"
            "ff4c19b430e8081223ffe99c81a98b05"));
    }
};


QTEST_APPLESS_MAIN(tst_qknxcryptographicengine)

#include "tst_qknxcryptographicengine.moc"
