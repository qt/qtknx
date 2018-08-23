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

#include "qknxcurve25519.h"
#include "qknxcryptographicdata_p.h"

#include "qknxnetipsessionauthenticate.h"
#include "qknxnetipsecurewrapper.h"
#include "qknxnetipsessionrequest.h"
#include "qknxnetipsessionresponse.h"
#include "qknxnetipsessionstatus.h"
#include "qknxnetiptimernotify.h"

#include <QtCore/qcryptographichash.h>

QT_BEGIN_NAMESPACE

bool QKnxOpenSsl::s_libraryLoaded = false;
bool QKnxOpenSsl::s_libraryEnabled = false;

Q_GLOBAL_STATIC(QKnxOpenSsl, qt_QKnxOpenSsl)
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, qt_knxOpenSslInitMutex, (QMutex::Recursive))

/*!
    \internal
*/
bool QKnxOpenSsl::supportsSsl()
{
    return ensureLibraryLoaded();
}

/*!
    \internal
*/
long QKnxOpenSsl::sslLibraryVersionNumber()
{
    if (!supportsSsl())
        return 0;
    return q_OpenSSL_version_num();
}

/*!
    \internal
*/
QString QKnxOpenSsl::sslLibraryVersionString()
{
    if (!supportsSsl())
        return QString();

    const char *versionString = q_OpenSSL_version(OPENSSL_VERSION);
    if (!versionString)
        return QString();

    return QString::fromLatin1(versionString);
}

/*!
    \internal
*/
long QKnxOpenSsl::sslLibraryBuildVersionNumber()
{
    return OPENSSL_VERSION_NUMBER;
}


QString QKnxOpenSsl::sslLibraryBuildVersionString()
{
    return QStringLiteral(OPENSSL_VERSION_TEXT);
}

/*!
    \internal
*/
bool QKnxOpenSsl::ensureLibraryLoaded()
{
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
}


/*!
    \inmodule QtKnx
    \class QKnxCurve25519PublicKey

    \since 5.12
    \ingroup qtknx-general-classes

    \brief The QKnxCurve25519PublicKey class represents the elliptic-curve
    public key to be used with the elliptic curve Diffie-Hellman (ECDH) key
    agreement scheme.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the module may be subject to change at any time without prior notice.
*/

/*!
    Constructs an empty invalid public key.
*/
QKnxCurve25519PublicKey::QKnxCurve25519PublicKey()
    : d_ptr(new QKnxCurve25519KeyData)
{}

/*!
    Destroys the public key and releases all allocated resources.
*/
QKnxCurve25519PublicKey::~QKnxCurve25519PublicKey() = default;

/*!
    Creates a new public key with the given private key \a key.
*/
QKnxCurve25519PublicKey::QKnxCurve25519PublicKey(const QKnxCurve25519PrivateKey &key)
    : d_ptr(new QKnxCurve25519KeyData)
{
    if (!key.d_ptr->m_evpPKey)
        return;

    if (!qt_QKnxOpenSsl->supportsSsl())
        return;

    q_EVP_PKEY_up_ref(key.d_ptr->m_evpPKey);
    d_ptr->m_evpPKey = key.d_ptr->m_evpPKey;
}

/*!
    Returns \c true if this is a default constructed public key; otherwise
    returns \c false. A public key is considered null if it contains no
    initialized values.
*/
bool QKnxCurve25519PublicKey::isNull() const
{
    return d_ptr->m_evpPKey == nullptr;
}

/*!
    Returns \c true if OpenSSL is available and if the public key contains
    initialized values, otherwise returns \c false.
*/
bool QKnxCurve25519PublicKey::isValid() const
{
    return qt_QKnxOpenSsl->supportsSsl() && !isNull();
}

/*!
    Returns an array of bytes that represent the Curve25519 raw public key.
*/
QKnxByteArray QKnxCurve25519PublicKey::bytes() const
{
    if (!isValid())
        return {};

    size_t len = 32;
    QKnxByteArray ba(int(len), 0);
    if (q_EVP_PKEY_get_raw_public_key(d_ptr->m_evpPKey, ba.data(), &len) > 0)
        return ba; // preferred way

    ba.resize(q_i2d_PUBKEY(d_ptr->m_evpPKey, nullptr));
    auto tmp = ba.data();
    q_i2d_PUBKEY(d_ptr->m_evpPKey, &tmp);
    return ba.right(32);
}

/*!
    Constructs the public key from the byte array \a data starting at position
    \a index inside the array if OpenSSL is available and no error occurs;
    otherwise returns a \e {default-constructed key} which can be invalid.
*/
QKnxCurve25519PublicKey QKnxCurve25519PublicKey::fromBytes(const QKnxByteArray &data, quint16 index)
{
    auto ba = data.mid(index, 32);
    if (ba.size() < 32)
        return {};

    if (!qt_QKnxOpenSsl->supportsSsl())
        return {};

    QKnxCurve25519PublicKey key;
    key.d_ptr->m_evpPKey = q_EVP_PKEY_new_raw_public_key(NID_X25519, nullptr, ba.constData(),
        ba.size()); // preferred way
    if (key.d_ptr->m_evpPKey)
        return key;

    key.d_ptr->m_evpPKey = q_EVP_PKEY_new();
    if (q_EVP_PKEY_set_type(key.d_ptr->m_evpPKey, NID_X25519) <= 0)
        return {};

    if (q_EVP_PKEY_set1_tls_encodedpoint(key.d_ptr->m_evpPKey, ba.constData(), ba.size()) <= 0)
        return {};

    return key;
}

/*!
    Constructs a copy of \a other.
*/
QKnxCurve25519PublicKey::QKnxCurve25519PublicKey(const QKnxCurve25519PublicKey &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxCurve25519PublicKey &QKnxCurve25519PublicKey::operator=(const QKnxCurve25519PublicKey &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


/*!
    \class QKnxCurve25519PrivateKey

    \inmodule QtKnx

    \since 5.12
    \ingroup qtknx-general-classes

    \brief The QKnxCurve25519PrivateKey class represents the elliptic-curve
    private key to be used with the elliptic curve Diffie-Hellman (ECDH) key
    agreement scheme.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the module may be subject to change at any time without prior notice.
*/

/*!
    Constructs a valid private key if OpenSSL is available and no error occurs.
*/
QKnxCurve25519PrivateKey::QKnxCurve25519PrivateKey()
    : d_ptr(new QKnxCurve25519KeyData)
{
    if (!qt_QKnxOpenSsl->supportsSsl())
        return;

    if (auto *pctx = q_EVP_PKEY_CTX_new_id(NID_X25519, nullptr)) {
        q_EVP_PKEY_keygen_init(pctx);
        q_EVP_PKEY_keygen(pctx, &d_ptr->m_evpPKey);
        q_EVP_PKEY_CTX_free(pctx);
    }
}

/*!
    Destroys the private key and releases all allocated resources.
*/
QKnxCurve25519PrivateKey::~QKnxCurve25519PrivateKey() = default;

/*!
    Returns \c true if this is a default constructed private key; otherwise
    returns \c false. A private key is considered null if it contains no
    initialized values.
*/
bool QKnxCurve25519PrivateKey::isNull() const
{
    return d_ptr->m_evpPKey == nullptr;
}

/*!
    Returns \c true if OpenSSL is available and if the private key contains
    initialized values, otherwise returns \c false.
*/
bool QKnxCurve25519PrivateKey::isValid() const
{
    return qt_QKnxOpenSsl->supportsSsl() && !isNull();
}

/*!
    Returns an array of bytes that represent the Curve25519 raw private key.
*/
QKnxByteArray QKnxCurve25519PrivateKey::bytes() const
{
    if (!isValid())
        return {};

    size_t len = 32;
    QKnxByteArray ba(int(len), 0);
    if (q_EVP_PKEY_get_raw_private_key(d_ptr->m_evpPKey, ba.data(), &len) <= 0)
        return {}; // preferred, no other way possible
    return ba;
}

/*!
    Constructs the private key from the byte array \a data starting at position
    \a index inside the array if OpenSSL is available and no error occurs;
    otherwise returns a \e {default-constructed key} which can be invalid.
*/
QKnxCurve25519PrivateKey QKnxCurve25519PrivateKey::fromBytes(const QKnxByteArray &data, quint16 index)
{
    auto ba = data.mid(index, 32);
    if (!qt_QKnxOpenSsl->supportsSsl() || ba.size() < 32)
        return {};

    QKnxCurve25519PrivateKey key;
    key.d_ptr->m_evpPKey = q_EVP_PKEY_new_raw_private_key(NID_X25519, nullptr, ba.constData(),
        ba.size()); // preferred way
    if (key.d_ptr->m_evpPKey)
        return key;

    static const auto pkcs8 = QKnxByteArray::fromHex("302e020100300506032b656e04220420");
    auto tmp = pkcs8 + ba;  // PKCS #8 is a standard syntax for storing private key information

    BIO *bio = nullptr;
    if ((bio = q_BIO_new_mem_buf(reinterpret_cast<void *> (tmp.data()), tmp.size())))
        key.d_ptr->m_evpPKey = q_d2i_PrivateKey_bio(bio, nullptr);
    q_BIO_free(bio);

    return key;
}

/*!
    Constructs a copy of \a other.
*/
QKnxCurve25519PrivateKey::QKnxCurve25519PrivateKey(const QKnxCurve25519PrivateKey &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxCurve25519PrivateKey &QKnxCurve25519PrivateKey::operator=(const QKnxCurve25519PrivateKey &other)
{
    d_ptr = other.d_ptr;
    return *this;
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
    by the module may be subject to change at any time without prior notice.
*/

/*!
    \internal
    \since 5.12
    \enum QKnxCryptographicEngine::Mode
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
    Derives and returns the shared secret from the given public peer key \a pub
    and the private key \a priv if OpenSSL is available and no error occurs;
    otherwise returns a \l {default-constructed value} which can be empty.
*/
QKnxByteArray QKnxCryptographicEngine::sharedSecret(const QKnxCurve25519PublicKey &pub,
                                                    const QKnxCurve25519PrivateKey &priv)
{
    if (pub.isNull() || priv.isNull())
        return {};

    if (!qt_QKnxOpenSsl->supportsSsl())
        return {};

    auto evpPKeyCtx = q_EVP_PKEY_CTX_new(priv.d_ptr->m_evpPKey, nullptr);
    if (!evpPKeyCtx)
        return {};

    struct ScopedFree final
    {
        ScopedFree(EVP_PKEY_CTX *key) : m_evpPKeyCtx(key) {}
        ~ScopedFree() { q_EVP_PKEY_CTX_free(m_evpPKeyCtx); }
        EVP_PKEY_CTX *m_evpPKeyCtx = nullptr;
    } _ { evpPKeyCtx };

    if (q_EVP_PKEY_derive_init(evpPKeyCtx) <= 0)
        return {};

    if (q_EVP_PKEY_derive_set_peer(evpPKeyCtx, pub.d_ptr->m_evpPKey) <= 0)
        return {};

    size_t keylen = 0;
    if (q_EVP_PKEY_derive(evpPKeyCtx, nullptr, &keylen) <= 0)
        return {};

    QKnxByteArray ba(int(keylen), 0);
    if (q_EVP_PKEY_derive(evpPKeyCtx, ba.data(), &keylen) <= 0)
        return {};

    return ba;
}

/*!
    Returns the session key calculated from the given secret \a sharedSecret.
*/
QKnxByteArray QKnxCryptographicEngine::sessionKey(const QKnxByteArray &sharedSecret)
{
    if (sharedSecret.isEmpty())
        return {};

    return QKnxByteArray::fromByteArray(QCryptographicHash::hash(sharedSecret.toByteArray(),
        QCryptographicHash::Sha256)).mid(0, 16);
}

/*!
    Returns the session key calculated from the given peer public key \a pub
    and the private key \a priv.
*/
QKnxByteArray QKnxCryptographicEngine::sessionKey(const QKnxCurve25519PublicKey &pub,
                                                  const QKnxCurve25519PrivateKey &priv)
{
    return sessionKey(sharedSecret(pub, priv));
}

/*!
    Returns the password hash derived from the user chosen password \a password.

    \note The salt used in the Password-Based Key Derivation Function (PBKDF2)
    function is set to \e {user-password.1.secure.ip.knx.org}.

    \sa pkcs5Pbkdf2HmacSha256()
*/
QKnxByteArray QKnxCryptographicEngine::userPasswordHash(const QByteArray &password)
{
    return pkcs5Pbkdf2HmacSha256(password,
        QKnxByteArray("user-password.1.secure.ip.knx.org", 33), 0x10000, 16);
}

/*!
    Returns the device authentication code hash derived from the user chosen
    password \a password.

    \note The salt used in the Password-Based Key Derivation Function (PBKDF2)
    function is set to \e {device-authentication-code.1.secure.ip.knx.org}.

    \sa pkcs5Pbkdf2HmacSha256()
*/
QKnxByteArray QKnxCryptographicEngine::deviceAuthenticationCodeHash(const QByteArray &password)
{
    return pkcs5Pbkdf2HmacSha256(password,
        QKnxByteArray("device-authentication-code.1.secure.ip.knx.org", 46), 0x10000, 16);
}

namespace QKnxPrivate
{
    static QKnxByteArray xor(const QKnxByteArray &left, const QKnxByteArray &right)
    {
        QKnxByteArray result(qMax(left.size(), right.size()), Qt::Uninitialized);
        for (int i = result.size() - 1; i >= 0; --i)
            result.set(i, left.value(i, 0x00) ^ right.value(i, 0x00));
        return result;
    }

    static QKnxByteArray doCrypt(int mode, const QKnxByteArray &key, const QKnxByteArray &iv,
        const QKnxByteArray &data)
    {
        QSharedPointer<EVP_CIPHER_CTX> ctxPtr(q_EVP_CIPHER_CTX_new(), q_EVP_CIPHER_CTX_free);
        if (ctxPtr.isNull())
            return {};

        const auto ctx = ctxPtr.data();
        if (q_EVP_CipherInit_ex(ctx, q_EVP_aes_128_cbc(), nullptr, nullptr, nullptr, mode) <= 0)
            return {};

        if (q_EVP_CIPHER_CTX_set_padding(ctx, 0) <= 0)
            return {};

        Q_ASSERT(q_EVP_CIPHER_CTX_iv_length(ctx) == 16);
        Q_ASSERT(q_EVP_CIPHER_CTX_key_length(ctx) == 16);

        if (q_EVP_CipherInit_ex(ctx, nullptr, nullptr, key.constData(), iv.constData(), mode) <= 0)
            return {};

        int outl;
        QKnxByteArray out(EVP_MAX_BLOCK_LENGTH, 0x00);
        if (q_EVP_CipherUpdate(ctx, out.data(), &outl,  data.constData(), data.size()) <= 0)
            return {};

        int outlen;
        if (q_EVP_CipherFinal_ex(ctx, out.data() + outl, &outlen) <= 0)
            return {};

        return out.mid(0, outl + outlen);
    }
}

/*!
    \internal
*/
QKnxByteArray QKnxCryptographicEngine::messageAuthenticationCode(QKnxCryptographicEngine::Mode mode,
    quint16 secureId, const QKnxNetIpFrameHeader &frameHdr, const QKnxCurve25519PublicKey &publicKey,
    const QKnxCurve25519PublicKey &peerKey, const QKnxByteArray &deviceAuthenticationCode)
{
    if (mode > QKnxCryptographicEngine::Mode::Encrypt
        || !frameHdr.isValid()
        || publicKey.isNull() || peerKey.isNull()
        || deviceAuthenticationCode.isEmpty()) {
            return {};
    }

    QKnxByteArray iv, ctr0; // initialization vector, block counter
    if (frameHdr.serviceType() == QKnxNetIp::ServiceType::SecureWrapper) {
        // |--------------------------------------------------------------------|
        // |  0  |  ...  |  5  |  6  |  ...  |  11  |  12  |  13  |  14  |  15  |
        // |--------------------------------------------------------------------|
        // |   Sequence info   |    Serial number   |     Tag     |      Q      |
        // |--------------------------------------------------------------------|

        // Q Shall be the length of the payload in octets, which is the length of
        // the original, encapsulated KNXnet/IP frame.
        iv = QKnxByteArray { /* TODO: Implement the scheme above */ };

        // |--------------------------------------------------------------------|
        // |  0  |  ...  |  5  |  6  |  ...  |  11  |  12  |  13  |  14  |  15  |
        // |--------------------------------------------------------------------|
        // |   Sequence info   |    Serial number   |     Tag     |  ff  |  i   |
        // |--------------------------------------------------------------------|

        // For Ctr0 the counter [i] shall be 00h initially. Each counter value [i]
        // shall be calculated by incrementing the preceding counter value by 1.
        ctr0 = QKnxByteArray { /* TODO: Implement the scheme above */ };
    } else if (frameHdr.serviceType() == QKnxNetIp::ServiceType::SessionResponse
        || frameHdr.serviceType() == QKnxNetIp::ServiceType::SessionAuthenticate) {
        iv = QKnxByteArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00 };
        ctr0 = QKnxByteArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0xff, 0x00 };
    } else if (frameHdr.serviceType() == QKnxNetIp::ServiceType::TimerNotify) {
        // |--------------------------------------------------------------------|
        // |  0  |  ...  |  5  |  6  |  ...  |  11  |  12  |  13  |  14  |  15  |
        // |--------------------------------------------------------------------|
        // |    Timer value    |    Serial number   |     Tag     |  Q = 0000h  |
        // |--------------------------------------------------------------------|
        iv = QKnxByteArray { /* TODO: Implement the scheme above */ };

        // |--------------------------------------------------------------------|
        // |  0  |  ...  |  5  |  6  |  ...  |  11  |  12  |  13  |  14  |  15  |
        // |--------------------------------------------------------------------|
        // |    Timer value    |    Serial number   |     Tag     |  ff  | 00h  |
        // |--------------------------------------------------------------------|
        ctr0 = QKnxByteArray { /* TODO: Implement the scheme above */ };
    }

    if (iv.isEmpty() || ctr0.isEmpty())
        return {};

    auto header = frameHdr.bytes();
    auto xor = QKnxPrivate::xor (publicKey.bytes(), peerKey.bytes());
    auto sessionId = QKnxUtils::QUint16::bytes(quint16(secureId));

    auto A = QKnxByteArray({ 0x00, 0x28 }) + header + sessionId + xor;
    A.resize(48); // pad

    if (!qt_QKnxOpenSsl->supportsSsl())
        return {};

    auto B0 = iv;
    auto B1 = A.mid(0, 16);
    auto B2 = A.mid(16, 16);
    auto B3 = A.mid(32, 16);

    int m = int(mode);
    auto Y0 = QKnxPrivate::doCrypt(m, deviceAuthenticationCode, iv, QKnxPrivate::xor(B0, { 0x00 }));
    auto Y1 = QKnxPrivate::doCrypt(m, deviceAuthenticationCode, iv, QKnxPrivate::xor(B1, Y0));
    auto Y2 = QKnxPrivate::doCrypt(m, deviceAuthenticationCode, iv, QKnxPrivate::xor(B2, Y1));
    auto Y3 = QKnxPrivate::doCrypt(m, deviceAuthenticationCode, iv, QKnxPrivate::xor(B3, Y2));
    auto S0 = QKnxPrivate::doCrypt(m, deviceAuthenticationCode, iv, ctr0);

    return QKnxPrivate::xor(S0, Y3);
}

/*!
    Returns the hash code derived from the user chosen password \a password,
    with the given \a salt and \a iterations.
    The value of \a derivedKeyLength should be in the range \c 0 to \c 32.
*/
QKnxByteArray QKnxCryptographicEngine::pkcs5Pbkdf2HmacSha256(const QByteArray &password,
    const QKnxByteArray &salt, qint32 iterations, quint8 derivedKeyLength)
{
    if (derivedKeyLength > 32)
        return {};

    if (!qt_QKnxOpenSsl->supportsSsl())
        return {};

    QKnxByteArray out(derivedKeyLength, 0x00);
    if (q_PKCS5_PBKDF2_HMAC(password.constData(), password.size(), salt.constData(), salt.size(),
        iterations, q_EVP_sha256(), out.size(), out.data()) <= 0) {
            return {};
    }
    return out;
}

QT_END_NAMESPACE
