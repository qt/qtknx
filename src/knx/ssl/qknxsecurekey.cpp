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

#include "qknxsecurekey.h"
#include "qknxcryptographicengine.h"

#include <QtNetwork/private/qtnetworkglobal_p.h>

#if QT_CONFIG(opensslv11)
# include "private/qsslsocket_openssl_symbols_p.h"
# include "private/qsslsocket_openssl11_symbols_p.h"
#endif

QT_BEGIN_NAMESPACE

class QKnxSecureKeyData : public QSharedData
{
public:
    QKnxSecureKeyData() = default;
    ~QKnxSecureKeyData()
    {
#if QT_CONFIG(opensslv11)
        q_EVP_PKEY_free(m_evpPKey);
#endif
    }

    bool isTypeValid() const
    {
        return m_type >= QKnxSecureKey::Type::Private && m_type < QKnxSecureKey::Type::Invalid;
    }

#if QT_CONFIG(opensslv11)
    EVP_PKEY *m_evpPKey{ nullptr };
#endif
    QKnxSecureKey::Type m_type{ QKnxSecureKey::Type::Invalid };
};

/*!
    \since 5.13
    \inmodule QtKnx

    \class QKnxSecureKey
    \ingroup qtknx-general-classes

    \brief The QKnxSecureKey class represents the elliptic-curve secure key to
    be used with the elliptic curve Diffie-Hellman (ECDH) key agreement scheme.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.
*/

/*!
    \enum QKnxSecureKey::Type

    Describes the types of keys QKnxSecureKey supports.

    \value Private      A private key.
    \value Public       A public key.
    \value Invalid      An invalid key, please do not use.
*/

/*!
    Constructs an empty invalid secure key.
*/
QKnxSecureKey::QKnxSecureKey()
    : d_ptr(new QKnxSecureKeyData)
{}

/*!
    Destroys the secure key and releases all allocated resources.
*/
QKnxSecureKey::~QKnxSecureKey() = default;

/*!
    Returns the type of the key: \l {QKnxSecureKey::Public}{Public} or
    \l {QKnxSecureKey::Private}{Private}.
*/
QKnxSecureKey::Type QKnxSecureKey::type() const
{
    return d_ptr->m_type;
}

/*!
    Returns \c true if this is a default constructed secure key; otherwise
    returns \c false. A secure key is considered \c null if it contains no
    initialized values.
*/
bool QKnxSecureKey::isNull() const
{
#if QT_CONFIG(opensslv11)
    return d_ptr->m_evpPKey == nullptr;
#else
    return true;
#endif
}

/*!
    Returns \c true if OpenSSL is available and if the key contains initialized
    values, otherwise returns \c false.
*/
bool QKnxSecureKey::isValid() const
{
#if QT_CONFIG(opensslv11)
    return !isNull() && d_ptr->isTypeValid() && QKnxCryptographicEngine::supportsCryptography();
#else
    return false;
#endif
}

/*!
    Returns an array of bytes that represent the Curve25519 raw secure key.

    \note The function will return an empty byte array for the private
    key unless OpenSLL with version 1.1.1a is used as back-end.
*/
QKnxByteArray QKnxSecureKey::bytes() const
{
#if QT_CONFIG(opensslv11)
    if (!isValid())
        return {};

    if (d_ptr->m_type == Type::Private) {
        size_t len = 32;
        QKnxByteArray ba(int(len), 0);
        if (q_EVP_PKEY_get_raw_private_key(d_ptr->m_evpPKey, ba.data(), &len) <= 0)
            return {}; // preferred, no other way possible
        return ba;
    }

    size_t len = 32;
    QKnxByteArray pub(32, Qt::Uninitialized);
    if (q_EVP_PKEY_get_raw_public_key(d_ptr->m_evpPKey, pub.data(), &len) > 0)
        return pub; // preferred way

    pub.resize(q_i2d_PUBKEY(d_ptr->m_evpPKey, nullptr));
    auto tmp = pub.data();
    q_i2d_PUBKEY(d_ptr->m_evpPKey, &tmp);
    return pub.right(32);
#else
    return {};
#endif
}

/*!
    Constructs the Curve25519 secure key from the byte array \a data starting
    at position \a index inside the array and sets the key type to \a type if
    OpenSSL is available and no error occurs;
    otherwise returns a \e {default-constructed key} which can be invalid.
*/
QKnxSecureKey QKnxSecureKey::fromBytes(QKnxSecureKey::Type type, const QKnxByteArray &data,
    quint16 index)
{
#if QT_CONFIG(opensslv11)
    auto ba = data.mid(index, 32);
    if (ba.size() < 32)
        return {};

    if (!QKnxCryptographicEngine::supportsCryptography())
        return {};

    QKnxSecureKey key;
    key.d_ptr->m_type = type;

    if (type == Type::Private) {
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

    if (type == Type::Public) {
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
#else
    Q_UNUSED(type)
    Q_UNUSED(data)
    Q_UNUSED(index)
#endif
    return {};
}

/*!
    Returns a new valid private key if OpenSSL is available and no error occurs.
*/
QKnxSecureKey QKnxSecureKey::generatePrivateKey()
{
    QKnxSecureKey key;
#if QT_CONFIG(opensslv11)
    if (!QKnxCryptographicEngine::supportsCryptography())
        return key;

    if (auto *pctx = q_EVP_PKEY_CTX_new_id(NID_X25519, nullptr)) {
        q_EVP_PKEY_keygen_init(pctx);
        key.d_ptr->m_type = Type::Private;
        q_EVP_PKEY_keygen(pctx, &key.d_ptr->m_evpPKey);
        q_EVP_PKEY_CTX_free(pctx);
    }
#endif
    return key;
}

/*!
    Returns a new valid public key with the given private key \a privateKey if
    OpenSSL is available and no error occurs.
*/
QKnxSecureKey QKnxSecureKey::publicKeyFromPrivate(const QKnxSecureKey &privateKey)
{
    QKnxSecureKey key;
#if QT_CONFIG(opensslv11)
    if (privateKey.type() == QKnxSecureKey::Type::Private && privateKey.isValid()) {
        q_EVP_PKEY_up_ref(privateKey.d_ptr->m_evpPKey);
        key.d_ptr->m_type = Type::Public;
        key.d_ptr->m_evpPKey = privateKey.d_ptr->m_evpPKey;
    }
#else
    Q_UNUSED(privateKey)
#endif
    return key;
}

/*!
    \overload publicKeyFromPrivate()
*/
QKnxSecureKey QKnxSecureKey::publicKeyFromPrivate(const QKnxByteArray &privateKey)
{
    return QKnxSecureKey::publicKeyFromPrivate(QKnxSecureKey::fromBytes(QKnxSecureKey::Type::Private,
        privateKey));
}

/*!
    Sets \a privateKey to a new valid private key and \a publicKey to a new
    valid public key derived from the freshly generated private key if OpenSSL
    is available and no error occurs.
*/
void QKnxSecureKey::generateKeys(QKnxSecureKey *privateKey, QKnxSecureKey *publicKey)
{
    if (!privateKey || !publicKey)
        return;

    *privateKey = generatePrivateKey();
    *publicKey = publicKeyFromPrivate(*privateKey);
}

/*!
    Derives and returns the shared secret from the given private key
    \a privateKey and the peer's public key \a peerPublicKey if OpenSSL
    is available and no error occurs;
    otherwise returns a \l {default-constructed value} which can be empty.
*/
QKnxByteArray QKnxSecureKey::sharedSecret(const QKnxSecureKey &privateKey,
    const QKnxSecureKey &peerPublicKey)
{
#if QT_CONFIG(opensslv11)
    if (privateKey.type() != QKnxSecureKey::Type::Private || !privateKey.isValid())
        return {};

    if (peerPublicKey.type() != QKnxSecureKey::Type::Public || !peerPublicKey.isValid())
        return {};

    auto evpPKeyCtx = q_EVP_PKEY_CTX_new(privateKey.d_ptr->m_evpPKey, nullptr);
    if (!evpPKeyCtx)
        return {};

    struct ScopedFree final
    {
        ScopedFree(EVP_PKEY_CTX *key) : m_evpPKeyCtx(key) {}
        ~ScopedFree() { q_EVP_PKEY_CTX_free(m_evpPKeyCtx); }
        EVP_PKEY_CTX *m_evpPKeyCtx = nullptr;
    } _{ evpPKeyCtx };

    if (q_EVP_PKEY_derive_init(evpPKeyCtx) <= 0)
        return {};

    if (q_EVP_PKEY_derive_set_peer(evpPKeyCtx, peerPublicKey.d_ptr->m_evpPKey) <= 0)
        return {};

    size_t keylen = 0;
    if (q_EVP_PKEY_derive(evpPKeyCtx, nullptr, &keylen) <= 0)
        return {};

    QKnxByteArray ba(int(keylen), 0);
    if (q_EVP_PKEY_derive(evpPKeyCtx, ba.data(), &keylen) <= 0)
        return {};
    return ba;
#else
    Q_UNUSED(privateKey)
    Q_UNUSED(peerPublicKey)
    return {};
#endif
}

/*!
    \overload sharedSecret()
*/
QKnxByteArray QKnxSecureKey::sharedSecret(const QKnxByteArray &privateKey,
    const QKnxByteArray &peerPublicKey)
{
    return sharedSecret(QKnxSecureKey::fromBytes(QKnxSecureKey::Type::Private, privateKey),
        QKnxSecureKey::fromBytes(QKnxSecureKey::Type::Public, peerPublicKey));
}


/*!
    Constructs a copy of \a other.
*/
QKnxSecureKey::QKnxSecureKey(const QKnxSecureKey &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxSecureKey &QKnxSecureKey::operator=(const QKnxSecureKey &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Returns \c true if this key and the given \a other key are equal; otherwise
    returns \c false.
*/
bool QKnxSecureKey::operator==(const QKnxSecureKey &other) const
{
    return d_ptr == other.d_ptr
#if QT_CONFIG(opensslv11)
        || (d_ptr->m_evpPKey == other.d_ptr->m_evpPKey && d_ptr->m_type == other.d_ptr->m_type)
#endif
        || (bytes() == other.bytes());
}

/*!
    Returns \c true if this key and the given \a other key are not equal;
    otherwise returns \c false.
*/
bool QKnxSecureKey::operator!=(const QKnxSecureKey &other) const
{
    return !operator==(other);
}

QT_END_NAMESPACE
