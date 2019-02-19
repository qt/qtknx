/******************************************************************************
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
******************************************************************************/

#include "qknxcryptographicengine.h"
#include "qknxnetipsessionauthenticate.h"
#include "qknxnetipsecurewrapper.h"
#include "qknxnetipsessionrequest.h"
#include "qknxnetipsessionresponse.h"
#include "qknxnetipsessionstatus.h"
#include "qknxnetiptimernotify.h"

#include <QtCore/qcryptographichash.h>
#include <QtCore/qmutex.h>

#include <QtNetwork/qpassworddigestor.h>
#include <QtNetwork/private/qtnetworkglobal_p.h>

#if QT_CONFIG(opensslv11)
# include <QtKnx/private/qsslsocket_openssl_symbols_p.h>
# include <QtKnx/private/qsslsocket_openssl11_symbols_p.h>
#endif

QT_BEGIN_NAMESPACE

class QKnxOpenSsl
{
public:
    static bool supportsSsl();
    static long sslLibraryVersionNumber();

protected:
    static bool ensureLibraryLoaded();

private:
    static bool s_libraryLoaded;
    static bool s_libraryEnabled;
};
bool QKnxOpenSsl::s_libraryLoaded = false;
bool QKnxOpenSsl::s_libraryEnabled = false;

Q_GLOBAL_STATIC(QKnxOpenSsl, qt_QKnxOpenSsl)
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, qt_knxOpenSslInitMutex, (QMutex::Recursive))

/*!
    \internal
*/
bool QKnxOpenSsl::supportsSsl()
{
#if QT_CONFIG(opensslv11)
    if (!q_resolveOpenSslSymbols())
        return false;

    const QMutexLocker locker(qt_knxOpenSslInitMutex);
    if (!s_libraryLoaded) {
        s_libraryLoaded = true;

        // Initialize OpenSSL.
        if (q_OPENSSL_init_ssl(0, nullptr) != 1)
            return false;
        q_SSL_load_error_strings();
        q_OpenSSL_add_all_algorithms();

        // Initialize OpenSSL's random seed.
        if (!q_RAND_status()) {
            qWarning("Random number generator not seeded, disabling SSL support");
            return false;
        }

        if (q_EVP_PKEY_type(NID_X25519) == NID_undef) {
            qWarning("The X25519 algorithm is not supported, disabling SSL support");
            return false;
        }
        s_libraryEnabled = true;
    }
    return s_libraryEnabled;
#else
    Q_UNUSED(qt_knxOpenSslInitMutex)
    return false;
#endif
}

/*!
    \internal
*/
long QKnxOpenSsl::sslLibraryVersionNumber()
{
#if QT_CONFIG(opensslv11)
    if (supportsSsl())
        return q_OpenSSL_version_num();
#endif
    return 0;
}


/*!
    \class QKnxCryptographicEngine

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-general-classes

    \brief The QKnxCryptographicEngine class provides the means to handle all
    KNXnet/IP security related tasks.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    \section2 Calculating Message Authentication Codes

    The computeMessageAuthenticationCode() function can be used to compute a
    message authentication code (MAC) for a KNXnet/IP secure frame. The fields
    that are used to compute the MAC depend on the type of the frame, such as
    \e {session response frame}, \e {session authentication frame}, or
    \e {timer notify frame}.

    The example code shows how to compute the MAC for the most common secure
    frames:

    \code
        auto dummyMac = QKnxByteArray(16, 000); // dummy to get a valid header

        // Session Response Frame

        quint16 secureSessionIdentifier = 0x0001;
        auto responseBuilder = QKnxNetIpSessionResponseProxy::builder();

        // create an intermediate frame to fetch a valid frame header
        auto netIpFrame = responseBuilder
            .setSecureSessionId(secureSessionIdentifier)
            .setPublicKey(serverPublicKey)
            .setMessageAuthenticationCode(dummyMac)
            .create();

        auto deviceAuthenticationHash =
            QKnxCryptographicEngine::deviceAuthenticationCodeHash({ "trustme" });
        auto XOR_X_Y = QKnxCryptographicEngine::XOR(clientPublicKey.bytes(), serverPublicKey.bytes());

        auto mac = QKnxCryptographicEngine::computeMessageAuthenticationCode(deviceAuthenticationHash,
            netIpFrame.header(), secureSessionIdentifier, XOR_X_Y);

        // create the final frame including the computed MAC
        netIpFrame = responseBuilder.
            .setMessageAuthenticationCode(mac)
            .create();


        // Session Authenticate Frame

        auto authenticateBuilder = QKnxNetIpSessionAuthenticateProxy::builder()'

        // create an intermediate frame to fetch a valid frame header
        netIpFrame = authenticateBuilder
            .setUserId(QKnxNetIp::SecureUserId::Management)
            .setMessageAuthenticationCode(dummyMac)
            .create();

        auto passwordHash = QKnxCryptographicEngine::userPasswordHash({ "secret" });

        mac = QKnxCryptographicEngine::computeMessageAuthenticationCode(passwordHash,
            netIpFrame.header(), userId, XOR_X_Y);

        // create the final frame including the computed MAC
        netIpFrame = responseBuilder.
            .setMessageAuthenticationCode(mac)
            .create();


        // Timer Notify Frame

        quint48 timerValue = 211938428830917;
        auto serialNumber = QKnxByteArray::fromHex("00fa12345678");
        quint16 messageTag = quint16(QRandomGenerator::global()->generate();

        auto timerNotifyBuilder = QKnxNetIpTimerNotifyProxy::builder();

        // create an intermediate frame to fetch a valid frame header
        netIpFrame = timerNotifyBuilder
            .setTimerValue(timerValue)
            .setSerialNumber(serialNumber)
            .setMessageTag(messageTag)
            .setMessageAuthenticationCode(dummyMac)
            .create();

        QKnxByteArray dummyPayload;
        quint16 dummySession = 0x0000;
        auto backboneKey = QKnxByteArray::fromHex("000102030405060708090a0b0c0d0e0f");

        mac = QKnxCryptographicEngine::computeMessageAuthenticationCode(backboneKey,
            netIpFrame.header(), dummySession, dummyPayload, timerValue, serialNumber, messageTag);

        // create the final frame including the computed MAC
        netIpFrame = responseBuilder.
            .setMessageAuthenticationCode(mac)
            .create();
    \endcode
*/

/*!
    \internal
    \fn QKnxCryptographicEngine::QKnxCryptographicEngine()
*/

/*!
    \internal
    \fn QKnxCryptographicEngine::~QKnxCryptographicEngine()
*/

/*!
    Determines if cryptography support is available. Returns \c true on success;
    \c false otherwise.
*/
bool QKnxCryptographicEngine::supportsCryptography()
{
    return qt_QKnxOpenSsl->supportsSsl();
}

/*
    Returns the OpenSSL version number of the OpenSSL library if OpenSSL is
    available and used to provide cryptographic support; or \c 0 in any other
    case.
*/
long QKnxCryptographicEngine::sslLibraryVersionNumber()
{
    return qt_QKnxOpenSsl->sslLibraryVersionNumber();
}

/*!
    Returns the session key calculated from the given private key
    \a privateKey and the peer's public key \a peerPublicKey if OpenSSL is
    available and no error occurs;
    otherwise returns a \l {default-constructed value} which can be empty.
*/
QKnxByteArray QKnxCryptographicEngine::sessionKey(const QKnxSecureKey &privateKey,
                                                  const QKnxSecureKey &peerPublicKey)
{
    return sessionKey(QKnxSecureKey::sharedSecret(privateKey, peerPublicKey));
}

/*!
    \overload sessionKey()
*/
QKnxByteArray QKnxCryptographicEngine::sessionKey(const QKnxByteArray &privateKey,
                                                  const QKnxByteArray &peerPublicKey)
{
    return sessionKey(QKnxSecureKey::sharedSecret(privateKey, peerPublicKey));
}

/*!
    Returns the session key computed from the given secret \a sharedSecret.
*/
QKnxByteArray QKnxCryptographicEngine::sessionKey(const QKnxByteArray &sharedSecret)
{
    if (sharedSecret.isEmpty())
        return {};

    return QKnxByteArray::fromByteArray(QCryptographicHash::hash(sharedSecret.toByteArray(),
        QCryptographicHash::Sha256)).mid(0, 16);
}

/*!
    Returns the password hash derived from the user chosen password \a password.

    \note The salt used in the Password-Based Key Derivation Function (PBKDF2)
    function is set to \e {user-password.1.secure.ip.knx.org}.
*/
QKnxByteArray QKnxCryptographicEngine::userPasswordHash(const QByteArray &password)
{
    const auto hash = QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Algorithm::Sha256,
        password, QByteArray("user-password.1.secure.ip.knx.org"), 0x10000, 16);
    return QKnxByteArray::fromByteArray(hash);
}

/*!
    Returns the device authentication code hash derived from the user chosen
    password \a password.

    \note The salt used in the Password-Based Key Derivation Function (PBKDF2)
    function is set to \e {device-authentication-code.1.secure.ip.knx.org}.
*/
QKnxByteArray QKnxCryptographicEngine::deviceAuthenticationCodeHash(const QByteArray &password)
{
    const auto hash = QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Algorithm::Sha256,
        password, "device-authentication-code.1.secure.ip.knx.org", 0x10000, 16);
    return QKnxByteArray::fromByteArray(hash);
}

/*!
    Performs a bytewise XOR operation on the arguments \a left and \a right.
    If the arguments are not equal in size, the function uses only the shorter
    array for the operation. If \a adjust is set to \c true, the arrays are made
    equal by padding them with \c 0x00 bytes.
*/
QKnxByteArray QKnxCryptographicEngine::XOR(const QKnxByteArray &left, const QKnxByteArray &right,
    bool adjust)
{
    QKnxByteArray result(adjust ? qMax(left.size(), right.size()) : qMin(left.size(), right.size()),
        Qt::Uninitialized);
    for (int i = result.size() - 1; i >= 0; --i)
        result.set(i, left.value(i, 0x00) ^ right.value(i, 0x00));
    return result;
}

namespace QKnxPrivate
{
    static QKnxByteArray b0(quint48 sequence, const QKnxByteArray &serial, quint16 tag, quint16 len)
    {
        return QKnxUtils::QUint48::bytes(sequence) + serial + QKnxUtils::QUint16::bytes(tag)
            + QKnxUtils::QUint16::bytes(len);
    }

    static QKnxByteArray ctr0(quint48 sequence, const QKnxByteArray &serial, quint16 tag)
    {
        return QKnxPrivate::b0(sequence, serial, tag, 0xff00);
    }

    static QKnxByteArray encrypt(const QKnxByteArray &key, const QKnxByteArray &data)
    {
#if QT_CONFIG(opensslv11)
        if (!qt_QKnxOpenSsl->supportsSsl())
            return {};

        QSharedPointer<EVP_CIPHER_CTX> ctxPtr(q_EVP_CIPHER_CTX_new(), q_EVP_CIPHER_CTX_free);
        if (ctxPtr.isNull())
            return {};
        q_EVP_CIPHER_CTX_reset(ctxPtr.data());

        const auto ctx = ctxPtr.data();
        const auto c = q_EVP_aes_128_cbc();
        if (q_EVP_CipherInit_ex(ctx, c, nullptr, nullptr, nullptr, 0x01) <= 0)
            return {};

        if (q_EVP_CIPHER_CTX_set_padding(ctx, 0) <= 0)
            return {};

        Q_ASSERT(q_EVP_CIPHER_CTX_iv_length(ctx) == 16);
        Q_ASSERT(q_EVP_CIPHER_CTX_key_length(ctx) == 16);

        static const quint8 iv[16] { 0x00 };
        if (q_EVP_CipherInit_ex(ctx, nullptr, nullptr, key.constData(), iv, 0x01) <= 0)
            return {};

        int outl, offset = 0;
        QKnxByteArray out(data.size() + q_EVP_CIPHER_block_size(c), 0x00);
        if (q_EVP_CipherUpdate(ctx, out.data(), &outl, data.constData(), data.size()) <= 0)
            return {};
        offset += outl;

        if (q_EVP_CipherFinal_ex(ctx, out.data() + offset, &outl) <= 0)
            return {};
        offset += outl;

        return out.mid(offset - 16, 16);
#else
        Q_UNUSED(key)
        Q_UNUSED(data)
        return {};
#endif
    }

    static QKnxByteArray processMAC(const QKnxByteArray &key, const QKnxByteArray &mac,
        quint48 sequenceNumber, const QKnxByteArray &serialNumber, quint16 messageTag)
    {
        if (key.isEmpty() || mac.isEmpty())
            return {};

        auto Ctr0 = QKnxPrivate::ctr0(sequenceNumber,
            (serialNumber.isEmpty() ? QKnxByteArray(6, 0x00) : serialNumber), messageTag);

        return QKnxCryptographicEngine::XOR(QKnxPrivate::encrypt(key, Ctr0), mac);
    }

    static QKnxByteArray processPayload(const QKnxByteArray &key, const QKnxByteArray &payload,
        quint48 sequenceNumber, const QKnxByteArray &serialNumber, quint16 messageTag)
    {
        if (key.isEmpty() || payload.isEmpty())
            return {};

        auto Ctr0 = QKnxPrivate::ctr0(sequenceNumber,
            (serialNumber.isEmpty() ? QKnxByteArray(6, 0x00) : serialNumber), messageTag);

        QKnxByteArray ctrArray;
        for (int i = 0; i < (payload.size() + 15) >> 4; ++i) {
            Ctr0.set(15, Ctr0.at(15) + 1);
            ctrArray += QKnxPrivate::encrypt(key, Ctr0);
        }

        return QKnxCryptographicEngine::XOR(ctrArray, payload, false);
    }
}

/*!
    Computes a message authentication code (MAC) using the given \a key,
    \a header, and \a id for the given \a data. Returns an array of bytes that
    represent the computed MAC or an empty byte array in case of an error.

    \note The \a sequenceNumber, \a serialNumber, and \a messageTag values
    are required to compute a valid MAC for KNXnet/IP secure wrapper frames.
    For all other types of secure frames, the possibly given values are ignored
    and \c 0 is used instead. For timer notify frames, \e {default-constructed}
    values are used instead of the \a id and \a data values.

    For an example of using this function, see
    \l {Calculating Message Authentication Codes}.
*/
QKnxByteArray QKnxCryptographicEngine::computeMessageAuthenticationCode(const QKnxByteArray &key,
    const QKnxNetIpFrameHeader &header, quint16 id, const QKnxByteArray &data,
    quint48 sequenceNumber, const QKnxByteArray &serialNumber, quint16 messageTag)
{
    if (key.isEmpty() || !header.isValid())
        return {};

    auto sn = (serialNumber.isEmpty() ? QKnxByteArray(6, 0x00) : serialNumber);

    QKnxByteArray B0, B;
    if (header.serviceType() == QKnxNetIp::ServiceType::SecureWrapper) {
        if (data.isEmpty())
            return {};

        const auto A = header.bytes() + QKnxUtils::QUint16::bytes(id);
        B0 = QKnxPrivate::b0(sequenceNumber, sn, messageTag, quint16(data.size()));
        B = B0 + QKnxUtils::QUint16::bytes(quint16(A.size())) + A + data;
    } else if (header.serviceType() == QKnxNetIp::ServiceType::SessionResponse
        || header.serviceType() == QKnxNetIp::ServiceType::SessionAuthenticate) {
            if (data.isEmpty())
                return {};

            const auto A = header.bytes() + QKnxUtils::QUint16::bytes(id);
            B0 = QKnxPrivate::b0(sequenceNumber, sn, messageTag, 0);
            B = B0 + QKnxUtils::QUint16::bytes(quint16(A.size() + data.size())) + A + data;
    } else if (header.serviceType() == QKnxNetIp::ServiceType::TimerNotify) {
        const auto A = header.bytes();
        B0 = QKnxPrivate::b0(sequenceNumber, sn, messageTag, 0);
        B = B0 + QKnxUtils::QUint16::bytes(quint16(A.size())) + A;
    }

    if (B.isEmpty())
        return {};
    B.resize(B.size() + (16 - (B.size() % 16))); // pad to multiple of 16

    return QKnxPrivate::encrypt(key, B);
}

/*!
    Encrypts the given KNXnet/IP frame \a frame with the given key \a key,
    sequence number \a sequenceNumber, serial number \a serialNumber, and
    message tag \a messageTag. Returns an array of bytes that represent the
    encrypted frame or an empty byte array in case of an error or invalid
    KNXnet/IP frame \a frame.
*/
QKnxByteArray QKnxCryptographicEngine::encryptSecureWrapperPayload(const QKnxByteArray &key,
    const QKnxNetIpFrame &frame, quint48 sequenceNumber, const QKnxByteArray &serialNumber,
    quint16 messageTag)
{
    if (!frame.isValid())
        return {};
    return QKnxPrivate::processPayload(key, frame.bytes(), sequenceNumber, serialNumber, messageTag);
}

/*!
    Decrypts the given KNXnet/IP frame \a frame with the given key \a key,
    sequence number \a sequenceNumber, serial number \a serialNumber, and
    message tag \a messageTag. Returns an array of bytes that represent the
    decrypted frame or an empty byte array in case of an error.
*/
QKnxByteArray QKnxCryptographicEngine::decryptSecureWrapperPayload(const QKnxByteArray &key,
    const QKnxByteArray &frame, quint48 sequenceNumber, const QKnxByteArray &serialNumber,
    quint16 messageTag)
{
    return QKnxPrivate::processPayload(key, frame, sequenceNumber, serialNumber, messageTag);
}

/*!
    Encrypts the given message authentication code (MAC) \a mac with the given
    key \a key, sequence number \a sequenceNumber, serial number \a serialNumber,
    and message tag \a messageTag. Returns an array of bytes that represent
    the encrypted MAC or an empty byte array in case of an error.

    \note The \a sequenceNumber, \a serialNumber and \a messageTag are mandatory
    to properly encrypt the MAC for KNXnet/IP secure wrapper frame, for all other
    secure frames the default value of \c 0 can be used.
*/
QKnxByteArray QKnxCryptographicEngine::encryptMessageAuthenticationCode(const QKnxByteArray &key,
    const QKnxByteArray &mac, quint48 sequenceNumber, const QKnxByteArray &serialNumber,
    quint16 messageTag)
{
    return QKnxPrivate::processMAC(key, mac, sequenceNumber, serialNumber, messageTag);
}

/*!
    Decrypts the given message authentication code (MAC) \a mac with the given
    key \a key, sequence number \a sequenceNumber, serial number \a serialNumber,
    and message tag \a messageTag. Returns an array of bytes that represent
    the decrypted MAC or an empty byte array in case of an error.

    \note The \a sequenceNumber, \a serialNumber and \a messageTag values are
    required to properly decrypt the MAC for KNXnet/IP secure wrapper frame.
    For all other secure frames, the default value of \c 0 can be used.

*/
QKnxByteArray QKnxCryptographicEngine::decryptMessageAuthenticationCode(const QKnxByteArray &key,
    const QKnxByteArray &mac, quint48 sequenceNumber, const QKnxByteArray &serialNumber,
    quint16 messageTag)
{
    return QKnxPrivate::processMAC(key, mac, sequenceNumber, serialNumber, messageTag);
}

QT_END_NAMESPACE
