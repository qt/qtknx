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

#include "qknxssl_p.h"

#include <private/qtnetworkglobal_p.h>

#if QT_CONFIG(opensslv11)
# include "private/qsslsocket_openssl_symbols_p.h"
# include "private/qsslsocket_openssl11_symbols_p.h"
#endif

#include <QtCore/qmutex.h>

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
    if (!QKnxPrivate::q_resolveOpenSslSymbols())
        return false;

    const QMutexLocker locker(qt_knxOpenSslInitMutex);
    if (!s_libraryLoaded) {
        s_libraryLoaded = true;

        // Initialize OpenSSL.
        if (QKnxPrivate::q_OPENSSL_init_ssl(0, nullptr) != 1)
            return false;
        QKnxPrivate::q_SSL_load_error_strings();
        QKnxPrivate::q_OpenSSL_add_all_algorithms();

        // Initialize OpenSSL's random seed.
        if (!QKnxPrivate::q_RAND_status()) {
            qWarning("Random number generator not seeded, disabling SSL support");
            return false;
        }

        if (QKnxPrivate::q_EVP_PKEY_type(NID_X25519) == NID_undef) {
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
        return QKnxPrivate::q_OpenSSL_version_num();
#endif
    return 0;
}

/*!
    \internal
*/
bool QKnxSsl::supportsCryptography()
{
    return qt_QKnxOpenSsl->supportsSsl();
}

/*!
    \internal
*/
long QKnxSsl::sslLibraryVersionNumber()
{
    return qt_QKnxOpenSsl->sslLibraryVersionNumber();
}

/*!
    \internal
*/
QKnxByteArray QKnxSsl::doCrypt(const QKnxByteArray &key, const QKnxByteArray &iv,
    const QKnxByteArray &data, Mode mode)
{
#if QT_CONFIG(opensslv11)
    if (!qt_QKnxOpenSsl->supportsSsl())
        return {};

    QSharedPointer<EVP_CIPHER_CTX> ctxPtr(QKnxPrivate::q_EVP_CIPHER_CTX_new(),
        QKnxPrivate::q_EVP_CIPHER_CTX_free);
    if (ctxPtr.isNull())
        return {};
    QKnxPrivate::q_EVP_CIPHER_CTX_reset(ctxPtr.data());

    const auto ctx = ctxPtr.data();
    const auto c = QKnxPrivate::q_EVP_aes_128_cbc();
    if (QKnxPrivate::q_EVP_CipherInit_ex(ctx, c, nullptr, nullptr, nullptr, mode) <= 0)
        return {};

    if (QKnxPrivate::q_EVP_CIPHER_CTX_set_padding(ctx, 0) <= 0)
        return {};

    Q_ASSERT(QKnxPrivate::q_EVP_CIPHER_CTX_iv_length(ctx) == 16);
    Q_ASSERT(QKnxPrivate::q_EVP_CIPHER_CTX_key_length(ctx) == 16);

    if (QKnxPrivate::q_EVP_CipherInit_ex(ctx, nullptr, nullptr, key.constData(), iv.constData(), mode) <= 0)
        return {};

    int outl, offset = 0;
    QKnxByteArray out(data.size() + QKnxPrivate::q_EVP_CIPHER_block_size(c), 0x00);
    if (QKnxPrivate::q_EVP_CipherUpdate(ctx, out.data(), &outl, data.constData(), data.size()) <= 0)
        return {};
    offset += outl;

    if (QKnxPrivate::q_EVP_CipherFinal_ex(ctx, out.data() + offset, &outl) <= 0)
        return {};
    offset += outl;

    return out.left(offset);
#else
    Q_UNUSED(key)
    Q_UNUSED(iv)
    Q_UNUSED(data)
    Q_UNUSED(mode)
    return {};
#endif
}

QT_END_NAMESPACE
