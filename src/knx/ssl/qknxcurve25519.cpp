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

QT_BEGIN_NAMESPACE

bool QKnxOpenSsl::s_libraryLoaded = false;
bool QKnxOpenSsl::s_libraryEnabled = false;
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, qt_knxOpenSslInitMutex, (QMutex::Recursive))

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
    \class QKnxCurve25519PublicKey

    \inmodule QtKnx

    \since 5.12
    \ingroup qtknx-general-classes

    \brief The QKnxCurve25519PublicKey class represents the elliptic-curve
    public key to be used with the elliptic curve Diffie-Hellman (ECDH) key
    agreement scheme.

    TODO: Add more documentation
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

    if (!QKnxOpenSsl::supportsSsl())
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
    return QKnxOpenSsl::supportsSsl() && !isNull();
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
    if (!QKnxOpenSsl::supportsSsl() || ba.size() < 32)
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

    TODO: Add more documentation
*/

/*!
    Constructs a valid private key if OpenSSL is available and no error occurs.
*/
QKnxCurve25519PrivateKey::QKnxCurve25519PrivateKey()
    : d_ptr(new QKnxCurve25519KeyData)
{
    if (!QKnxOpenSsl::supportsSsl())
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
    return QKnxOpenSsl::supportsSsl() && !isNull();
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
    if (!QKnxOpenSsl::supportsSsl() || ba.size() < 32)
        return {};

    QKnxCurve25519PrivateKey key;
    key.d_ptr->m_evpPKey = q_EVP_PKEY_new_raw_private_key(NID_X25519, nullptr, ba.constData(),
        ba.size()); // preferred way
    if (key.d_ptr->m_evpPKey)
        return key;

    static const auto pkcs8 = QKnxByteArray::fromHex("302e020100300506032b656e04220420");
    auto tmp = pkcs8 + ba;  // PKCS #8 is a standard syntax for storing private key information

    BIO *bio = nullptr;
    if (bio = q_BIO_new_mem_buf(reinterpret_cast<void *> (tmp.data()), tmp.size()))
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

    \inmodule QtKnx

    \since 5.12
    \ingroup qtknx-general-classes

    \brief The QKnxCryptographicEngine class provides the means to handle all
    KNXnet/IP security related tasks.

    TODO: Add more documentation
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
    if (!QKnxOpenSsl::supportsSsl())
        return {};

    if (pub.isNull() || priv.isNull())
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

QT_END_NAMESPACE

