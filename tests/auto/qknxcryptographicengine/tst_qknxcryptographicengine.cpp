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
#include <QtCore/qloggingcategory.h>
#include <QtKnx/qknxcurve25519.h>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxnetiproutingindication.h>
#include <QtKnx/qknxnetipsecurewrapper.h>
#include <QtKnx/qknxnetipsessionauthenticate.h>
#include <QtKnx/qknxnetipsessionresponse.h>
#include <QtKnx/qknxnetipsessionstatus.h>
#include <QtKnx/qknxnetiptimernotify.h>
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
    void initTestCase()
    {
        QLoggingCategory::setFilterRules("qt.network.ssl=false");
    }

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
            if (QKnxOpenSsl::sslLibraryVersionNumber() >= 0x1010101fL)
                QCOMPARE(key.bytes().size(), 32);

            auto bytes = QKnxByteArray::fromHex("b8fabd62665d8b9e8a9d8b1f4bca42c8"
                "c2789a6110f50e9dd785b3ede883f378");
            key = QKnxCurve25519PrivateKey::fromBytes(bytes);
            QCOMPARE(key.isNull(), false);
            QCOMPARE(key.isValid(), true);
            if (QKnxOpenSsl::sslLibraryVersionNumber() >= 0x1010101fL)
                QCOMPARE(key.bytes(), bytes);

            bytes = QKnxByteArray::fromHex("68c1744813f4e65cf10cca671caa1336"
                "a796b4ac40cc5cf2655674225c1e5264");
            key = QKnxCurve25519PrivateKey::fromBytes(bytes);
            QCOMPARE(key.isNull(), false);
            QCOMPARE(key.isValid(), true);
            if (QKnxOpenSsl::sslLibraryVersionNumber() >= 0x1010101fL)
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

    void testSessionKey()
    {
        auto sharedSecret = QKnxByteArray::fromHex("d801525217618f0da90a4ff22148aee0"
            "ff4c19b430e8081223ffe99c81a98b05");
        auto result = QKnxCryptographicEngine::sessionKey(sharedSecret);
        QCOMPARE(result, QKnxByteArray::fromHex("289426c2912535ba98279a4d1843c487"));
    }

    void PKCS5_PBKDF2_HMAC()
    {
        // session authenticate
        auto result = QKnxCryptographicEngine::userPasswordHash({ "secret" });
        QCOMPARE(result, QKnxByteArray::fromHex("03fcedb66660251ec81a1a716901696a"));

        // session response
        result = QKnxCryptographicEngine::deviceAuthenticationCodeHash({ "trustme" });
        QCOMPARE(result, QKnxByteArray::fromHex("e158e4012047bd6cc41aafbc5c04c1fc"));
    }

    void testMessageAuthenticationCode()
    {
        if (QKnxOpenSsl::sslLibraryVersionNumber() < 0x1010000fL)
            return;

        /* This test more or less follows KNX AN156 - Annex A */

        auto clientPublicKey = QKnxCurve25519PublicKey::fromBytes(QKnxByteArray::fromHex("0aa227b4"
            "fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));

        auto serverPrivateKey = QKnxCurve25519PrivateKey::fromBytes(QKnxByteArray::fromHex("68c174"
            "4813f4e65cf10cca671caa1336a796b4ac40cc5cf2655674225c1e5264"));
        auto serverPublicKey = QKnxCurve25519PublicKey::fromBytes(QKnxByteArray::fromHex("bdf09990"
            "9923143ef0a5de0b3be3687bc5bd3cf5f9e6f901699cd870ec1ff824"));

        QCOMPARE(serverPublicKey.bytes(), QKnxCurve25519PublicKey(serverPrivateKey).bytes());

        /* Session Response */

        const QByteArray deviceAuthenticationPassword { "trustme" };
        auto deviceAuthenticationCode =
            QKnxCryptographicEngine::deviceAuthenticationCodeHash(deviceAuthenticationPassword);

        QCOMPARE(deviceAuthenticationCode, QKnxByteArray::fromHex("e158e4012047bd6cc41aafbc5c04c1fc"));

        auto knxNetIpSecureHeader = QKnxNetIpFrameHeader::fromBytes(QKnxByteArray::fromHex("061009520038"));
        quint16 secureSessionIdentifier = 0x0001;
        auto XOR_X_Y = QKnxCryptographicEngine::XOR(clientPublicKey.bytes(), serverPublicKey.bytes());

        QCOMPARE(XOR_X_Y,
            QKnxByteArray::fromHex("b752be246459260f6b0c4801fbd5a67599f83b4057b3ef1e79e469ac17234e15"));

        auto mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(deviceAuthenticationCode,
            knxNetIpSecureHeader, secureSessionIdentifier, XOR_X_Y);
        QCOMPARE(mac, QKnxByteArray::fromHex("da3dc6af79896aa6ee7573d69950c283"));

        auto encMac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(deviceAuthenticationCode,
            mac);
        QCOMPARE(encMac, QKnxByteArray::fromHex("a922505aaa436163570bd5494c2df2a3"));

        auto decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(deviceAuthenticationCode, encMac);
        QCOMPARE(decMac, mac);

        auto sharedSecret = QKnxCryptographicEngine::sharedSecret(clientPublicKey, serverPrivateKey);
        QCOMPARE(sharedSecret,
            QKnxByteArray::fromHex("d801525217618f0da90a4ff22148aee0ff4c19b430e8081223ffe99c81a98b05"));

        auto sessionKey = QKnxCryptographicEngine::sessionKey(sharedSecret);
        QCOMPARE(sessionKey, QKnxByteArray::fromHex("289426c2912535ba98279a4d1843c487"));

        /* Session Authenticate */

        const QByteArray password { "secret" };
        auto passwordHash = QKnxCryptographicEngine::userPasswordHash(password);

        QCOMPARE(passwordHash, QKnxByteArray::fromHex("03fcedb66660251ec81a1a716901696a"));

        knxNetIpSecureHeader = QKnxNetIpFrameHeader::fromBytes(QKnxByteArray::fromHex("061009530018"));

        mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(passwordHash,
            knxNetIpSecureHeader, QKnxNetIp::SecureUserId::Management, XOR_X_Y);
        QCOMPARE(mac, QKnxByteArray::fromHex("741669f5e32bff6fa2edf51c52d4bd8f"));

        encMac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(passwordHash, mac);
        QCOMPARE(encMac, QKnxByteArray::fromHex("1f1d59ea9f12a152e5d9727f08462cde"));

        decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(passwordHash, encMac);
        QCOMPARE(decMac, mac);

        auto sequenceNumber = 0x000000000000;
        auto serialNumber = QKnxByteArray::fromHex("00fa12345678");
        auto messageTag = 0xaffe;
        knxNetIpSecureHeader = QKnxNetIpFrameHeader::fromBytes(QKnxByteArray::fromHex("06100950003e"));

        auto frame = QKnxNetIpSessionAuthenticateProxy::builder()
            .setUserId(QKnxNetIp::SecureUserId::Management)
            .setMessageAuthenticationCode(encMac)
            .create();

        mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(sessionKey,
            knxNetIpSecureHeader, secureSessionIdentifier, frame.bytes(), sequenceNumber,
            serialNumber, messageTag);
        QCOMPARE(mac, QKnxByteArray::fromHex("602280d0896beaa7106e7248f67f2eef"));

        encMac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(sessionKey, mac,
            sequenceNumber, serialNumber, messageTag);
        QCOMPARE(encMac, QKnxByteArray::fromHex("52dba8e7e4bd80bd7d868a3ae78749de"));

        decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(sessionKey, encMac,
            sequenceNumber, serialNumber, messageTag);
        QCOMPARE(decMac, mac);

        auto encData = QKnxCryptographicEngine::encryptSecureWrapperPayload(sessionKey,
            frame, sequenceNumber, serialNumber, messageTag);
        QCOMPARE(encData, QKnxByteArray::fromHex("7915a4f36e6e4208d28b4a207d8f35c0d138c26a7b5e7169"));

        auto decData = QKnxCryptographicEngine::decryptSecureWrapperPayload(sessionKey,
            encData, sequenceNumber, serialNumber, messageTag);
        QCOMPARE(decData, frame.bytes());

        /* Session Status */

        serialNumber = QKnxByteArray::fromHex("00faaaaaaaaa");
        knxNetIpSecureHeader = QKnxNetIpFrameHeader::fromBytes(QKnxByteArray::fromHex("06100950002e"));

        frame = QKnxNetIpSessionStatusProxy::builder()
            .setStatus(QKnxNetIp::SecureSessionStatus::AuthenticationSuccess)
            .create();

        mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(sessionKey,
            knxNetIpSecureHeader, secureSessionIdentifier, frame.bytes(), sequenceNumber,
            serialNumber, messageTag);
        QCOMPARE(mac, QKnxByteArray::fromHex("a8ed2796a566cd60b91a4de5c1144cbc"));

        encMac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(sessionKey, mac,
            sequenceNumber, serialNumber, messageTag);
        QCOMPARE(encMac, QKnxByteArray::fromHex("a373c3e0b4bde4497c395e4b1c2f46a1"));

        decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(sessionKey, encMac,
            sequenceNumber, serialNumber, messageTag);
        QCOMPARE(decMac, mac);

        encData = QKnxCryptographicEngine::encryptSecureWrapperPayload(sessionKey,
            frame, sequenceNumber, serialNumber, messageTag);
        QCOMPARE(encData, QKnxByteArray::fromHex("26156db5c749888f"));

        decData = QKnxCryptographicEngine::decryptSecureWrapperPayload(sessionKey,
            encData, sequenceNumber, serialNumber, messageTag);
        QCOMPARE(decData, frame.bytes());

        /* Routing Indication */

        frame = QKnxNetIpRoutingIndicationProxy::builder()
            .setCemi(QKnxLinkLayerFrame::builder()
                .setMedium(QKnx::MediumType::NetIP)
                .setData(QKnxByteArray::fromHex("2900bcd011590ade010081"))
                .createFrame())
            .create();

        auto backboneKey = QKnxByteArray::fromHex("000102030405060708090a0b0c0d0e0f");
        quint48 timerValue = 211938428830917;
        serialNumber = QKnxByteArray::fromHex("00fa12345678");
        knxNetIpSecureHeader = QKnxNetIpFrameHeader::fromBytes(QKnxByteArray::fromHex("061009500037"));
        secureSessionIdentifier = 0x0000;

        mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(backboneKey,
            knxNetIpSecureHeader, secureSessionIdentifier, frame.bytes(), timerValue,
            serialNumber, messageTag);
        QCOMPARE(mac, QKnxByteArray::fromHex("bd0a294b952554b23539204c2271d26b"));

        encMac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(backboneKey, mac,
            timerValue, serialNumber, messageTag);
        QCOMPARE(encMac, QKnxByteArray::fromHex("7212a03aaae49da85689774c1d2b4da4"));

        decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(backboneKey, encMac,
            timerValue, serialNumber, messageTag);
        QCOMPARE(decMac, mac);

        encData = QKnxCryptographicEngine::encryptSecureWrapperPayload(backboneKey,
            frame, timerValue, serialNumber, messageTag);
        QCOMPARE(encData, QKnxByteArray::fromHex("b7ee7e8a1c2f7bbabec775fd6e10d0bc4b"));

        decData = QKnxCryptographicEngine::decryptSecureWrapperPayload(backboneKey,
            encData, timerValue, serialNumber, messageTag);
        QCOMPARE(decData, frame.bytes());

        /* Timer Notify */

        knxNetIpSecureHeader = QKnxNetIpFrameHeader::fromBytes(QKnxByteArray::fromHex("061009550024"));

        mac = QKnxCryptographicEngine::calculateMessageAuthenticationCode(backboneKey,
            knxNetIpSecureHeader, secureSessionIdentifier, {}, timerValue,
            serialNumber, messageTag);
        QCOMPARE(mac, QKnxByteArray::fromHex("21631241bc1f784d6d03da070580464a"));

        encMac = QKnxCryptographicEngine::encryptMessageAuthenticationCode(backboneKey, mac,
            timerValue, serialNumber, messageTag);
        QCOMPARE(encMac, QKnxByteArray::fromHex("ee7b9b3083deb1570eb38d073adad985"));

        decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(backboneKey, encMac,
            timerValue, serialNumber, messageTag);
        QCOMPARE(decMac, mac);

    }

    void testSecureWrapperFrame()
    {
        auto sessionAuthenticate = QKnxNetIpSessionAuthenticateProxy::builder()
            .setUserId(QKnxNetIp::SecureUserId::Management)
            .setMessageAuthenticationCode(QKnxByteArray::fromHex("1f1d59ea9f12a152e5d9727f08462cde"))
            .create();

        quint48 sequenceNumber = 0x000000000000;
        auto serialNumber = QKnxByteArray::fromHex("00fa12345678");
        quint16 messageTag = 0xaffe;
        quint16 secureSessionIdentifier = 0x0001;
        auto sessionKey = QKnxByteArray::fromHex("289426c2912535ba98279a4d1843c487");

        auto secureWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
            .setSecureSessionId(secureSessionIdentifier)
            .setSequenceNumber(sequenceNumber)
            .setSerialNumber(serialNumber)
            .setMessageTag(messageTag)
            .setEncapsulatedFrame(sessionAuthenticate)
            .create(sessionKey);
        QCOMPARE(secureWrapper.isValid(), true);

        const QKnxNetIpSecureWrapperProxy proxy(secureWrapper);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.secureSessionId(), secureSessionIdentifier);
        QCOMPARE(proxy.sequenceNumber(), sequenceNumber);
        QCOMPARE(proxy.serialNumber(), serialNumber);
        QCOMPARE(proxy.messageTag(), messageTag);
        QCOMPARE(proxy.messageAuthenticationCode(),
            QKnxByteArray::fromHex("52dba8e7e4bd80bd7d868a3ae78749de"));
        QCOMPARE(proxy.encapsulatedFrame(),
            QKnxByteArray::fromHex("7915a4f36e6e4208d28b4a207d8f35c0d138c26a7b5e7169"));
    }

    void testTimerNotifyFrame()
    {
        quint48 timerValue = 211938428830917;
        auto serialNumber = QKnxByteArray::fromHex("00fa12345678");
        quint16 messageTag = 0xaffe;
        auto mac = QKnxByteArray::fromHex("ee7b9b3083deb1570eb38d073adad985");

        auto timerNotify = QKnxNetIpTimerNotifyProxy::builder()
            .setTimerValue(timerValue)
            .setSerialNumber(serialNumber)
            .setMessageTag(messageTag)
            .setMessageAuthenticationCode(mac)
            .create();

        QKnxNetIpTimerNotifyProxy proxy(timerNotify);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.timerValue(), timerValue);
        QCOMPARE(proxy.serialNumber(), serialNumber);
        QCOMPARE(proxy.messageTag(), messageTag);
        QCOMPARE(proxy.messageAuthenticationCode(), mac);

        quint16 sessionId = 0x0000;
        auto backboneKey = QKnxByteArray::fromHex("000102030405060708090a0b0c0d0e0f");

        auto secureTimerNotify = QKnxNetIpTimerNotifyProxy::secureBuilder()
            .setTimerValue(timerValue)
            .setSerialNumber(serialNumber)
            .setMessageTag(messageTag)
            .create(backboneKey, sessionId);

        QKnxNetIpTimerNotifyProxy proxy2(secureTimerNotify);
        QCOMPARE(proxy2.isValid(), true);
        QCOMPARE(proxy2.timerValue(), timerValue);
        QCOMPARE(proxy2.serialNumber(), serialNumber);
        QCOMPARE(proxy2.messageTag(), messageTag);
        QCOMPARE(proxy2.messageAuthenticationCode(), mac);
    }

    void testSessionResponseFrame()
    {
        auto clientPublicKey = QKnxCurve25519PublicKey::fromBytes(QKnxByteArray::fromHex("0aa227b4"
            "fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));
        auto serverPublicKey = QKnxCurve25519PublicKey::fromBytes(QKnxByteArray::fromHex("bdf09990"
            "9923143ef0a5de0b3be3687bc5bd3cf5f9e6f901699cd870ec1ff824"));

        quint16 secureSessionIdentifier = 0x0001;
        const QByteArray deviceAuthenticationPassword { "trustme" };
        auto mac = QKnxByteArray::fromHex("a922505aaa436163570bd5494c2df2a3");

        auto sessionResponse = QKnxNetIpSessionResponseProxy::builder()
            .setSecureSessionId(secureSessionIdentifier)
            .setPublicKey(serverPublicKey.bytes())
            .setMessageAuthenticationCode(mac)
            .create();

        QKnxNetIpSessionResponseProxy proxy(sessionResponse);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.secureSessionId(), secureSessionIdentifier);
        QCOMPARE(proxy.publicKey(), serverPublicKey.bytes());
        QCOMPARE(proxy.messageAuthenticationCode(), mac);

        sessionResponse = QKnxNetIpSessionResponseProxy::secureBuilder()
            .setSecureSessionId(secureSessionIdentifier)
            .setPublicKey(serverPublicKey.bytes())
            .create(deviceAuthenticationPassword, clientPublicKey.bytes());

        QKnxNetIpSessionResponseProxy proxy2(sessionResponse);
        QCOMPARE(proxy2.isValid(), true);
        QCOMPARE(proxy2.secureSessionId(), secureSessionIdentifier);
        QCOMPARE(proxy2.publicKey(), serverPublicKey.bytes());
        QCOMPARE(proxy2.messageAuthenticationCode(), mac);
    }

    void testSessionAuthenticateFrame()
    {

        auto clientPublicKey = QKnxCurve25519PublicKey::fromBytes(QKnxByteArray::fromHex("0aa227b4"
            "fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));
        auto serverPublicKey = QKnxCurve25519PublicKey::fromBytes(QKnxByteArray::fromHex("bdf09990"
            "9923143ef0a5de0b3be3687bc5bd3cf5f9e6f901699cd870ec1ff824"));

        const QByteArray password { "secret" };
        auto mac = QKnxByteArray::fromHex("1f1d59ea9f12a152e5d9727f08462cde");

        auto sessionAuth = QKnxNetIpSessionAuthenticateProxy::builder()
            .setUserId(QKnxNetIp::SecureUserId::Management)
            .setMessageAuthenticationCode(mac)
            .create();

        QKnxNetIpSessionAuthenticateProxy proxy(sessionAuth);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.userId(), QKnxNetIp::SecureUserId::Management);
        QCOMPARE(proxy.messageAuthenticationCode(), mac);

        auto sessionAuth2 = QKnxNetIpSessionAuthenticateProxy::secureBuilder()
            .setUserId(QKnxNetIp::SecureUserId::Management)
            .create(password, clientPublicKey.bytes(), serverPublicKey.bytes());

        QKnxNetIpSessionAuthenticateProxy proxy2(sessionAuth2);
        QCOMPARE(proxy2.isValid(), true);
        QCOMPARE(proxy2.userId(), QKnxNetIp::SecureUserId::Management);
        QCOMPARE(proxy2.messageAuthenticationCode(), mac);
    }
};

QTEST_APPLESS_MAIN(tst_qknxcryptographicengine)

#include "tst_qknxcryptographicengine.moc"
