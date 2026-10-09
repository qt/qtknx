// Copyright (C) 2017 The Qt Company Ltd.
// Copyright (C) 2014 BlackBerry Limited. All rights reserved.
// Copyright (C) 2016 Richard J. Moore <rich@kde.org>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:critical reason:execute-external-code

/****************************************************************************
**
** In addition, as a special exception, the copyright holders listed above give
** permission to link the code of its release of Qt with the OpenSSL project's
** "OpenSSL" library (or modified versions of the "OpenSSL" library that use the
** same license as the original version), and distribute the linked executables.
**
** You must comply with the GNU General Public License version 2 in all
** respects for all of the code used other than the "OpenSSL" code.  If you
** modify this file, you may extend this exception to your version of the file,
** but you are not obligated to do so.  If you do not wish to do so, delete
** this exception statement from your version of this file.
**
****************************************************************************/

#include "qsslsocket_openssl_symbols_p.h"
#include <QtNetwork/private/qssl_p.h>

#ifdef Q_OS_WIN
# include <QtCore/private/qsystemlibrary_p.h>
#elif QT_CONFIG(library)
# include <QtCore/qlibrary.h>
#endif
#include <QtCore/qdatetime.h>
#if defined(Q_OS_UNIX)
#include <QtCore/qdir.h>
#include <QtCore/qdirlisting.h>
#endif
#include <QtCore/private/qduplicatetracker_p.h>
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include <link.h>
#endif
#ifdef Q_OS_DARWIN
#include <QtCore/private/qcore_mac_p.h>
#endif

#include <algorithm>

QT_BEGIN_NAMESPACE

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
constexpr auto DefaultWarningLevel = QtCriticalMsg;
#else
constexpr auto DefaultWarningLevel = QtDebugMsg;
#endif

Q_LOGGING_CATEGORY(lcTlsBackend, "qt.tlsbackend.ossl", DefaultWarningLevel);

namespace QKnxPrivate {

using namespace Qt::StringLiterals;

#ifndef QT_LINKED_OPENSSL

namespace {
void qsslSocketUnresolvedSymbolWarning(const char *functionName)
{
    qCWarning(lcTlsBackend, "QSslSocket: cannot call unresolved function %s", functionName);
}

#if QT_CONFIG(library)
void qsslSocketCannotResolveSymbolWarning(const char *functionName)
{
    qCWarning(lcTlsBackend, "QSslSocket: cannot resolve %s", functionName);
}
#endif

}

#endif // QT_LINKED_OPENSSL

// Library init and version
DEFINEFUNC2(int, OPENSSL_init_ssl, uint64_t opts, opts, const OPENSSL_INIT_SETTINGS *settings, settings, return 0, return)
DEFINEFUNC2(int, OPENSSL_init_crypto, uint64_t opts, opts, const OPENSSL_INIT_SETTINGS *settings, settings, return 0, return)
DEFINEFUNC(long, OpenSSL_version_num, void, DUMMYARG, return 0, return)
DEFINEFUNC(const char *, OpenSSL_version, int a, a, return nullptr, return)

// EVP cipher context — AES-128-CBC for KNXnet/IP Secure
DEFINEFUNC(EVP_CIPHER_CTX *, EVP_CIPHER_CTX_new, void, DUMMYARG, return nullptr, return)
DEFINEFUNC(void, EVP_CIPHER_CTX_free, EVP_CIPHER_CTX *a, a, return, DUMMYARG)
DEFINEFUNC(int, EVP_CIPHER_CTX_reset, EVP_CIPHER_CTX *c, c, return 0, return)
DEFINEFUNC(int, EVP_CIPHER_CTX_get_key_length, const EVP_CIPHER_CTX *ctx, ctx, return 0, return)
DEFINEFUNC(int, EVP_CIPHER_CTX_get_iv_length, const EVP_CIPHER_CTX *ctx, ctx, return 0, return)
DEFINEFUNC(int, EVP_CIPHER_get_block_size, const EVP_CIPHER *c, c, return 0, return)
DEFINEFUNC2(int, EVP_CIPHER_CTX_set_padding, EVP_CIPHER_CTX *x, x, int padding, padding, return 0, return)
DEFINEFUNC(int, EVP_PKEY_type, int a, a, return NID_undef, return)
DEFINEFUNC6(int, EVP_CipherInit_ex, EVP_CIPHER_CTX *ctx, ctx, const EVP_CIPHER *cipher, cipher, ENGINE *impl, impl, const unsigned char *key, key, const unsigned char *iv, iv, int enc, enc, return 0, return)
DEFINEFUNC5(int, EVP_CipherUpdate, EVP_CIPHER_CTX *ctx, ctx, unsigned char *out, out, int *outl, outl, const unsigned char *in, in, int inl, inl, return 0, return)
DEFINEFUNC3(int, EVP_CipherFinal_ex, EVP_CIPHER_CTX *ctx, ctx, unsigned char *outm, outm, int *outl, outl, return 0, return)
DEFINEFUNC(void, EVP_PKEY_free, EVP_PKEY *a, a, return, DUMMYARG)
DEFINEFUNC3(int, EVP_PKEY_get_raw_private_key, const EVP_PKEY *pkey, pkey, unsigned char *priv, priv, size_t *len, len, return 0, return)
DEFINEFUNC3(int, EVP_PKEY_get_raw_public_key, const EVP_PKEY *pkey, pkey, unsigned char *pub, pub, size_t *len, len, return 0, return)
DEFINEFUNC4(EVP_PKEY *, EVP_PKEY_new_raw_private_key, int type, type, ENGINE *e, e, const unsigned char *key, key, size_t keylen, keylen, return nullptr, return)
DEFINEFUNC4(EVP_PKEY *, EVP_PKEY_new_raw_public_key, int type, type, ENGINE *e, e, const unsigned char *key, key, size_t keylen, keylen, return nullptr, return)
DEFINEFUNC(int, EVP_PKEY_up_ref, EVP_PKEY *pkey, pkey, return 0, return)
DEFINEFUNC3(EVP_PKEY_CTX *, EVP_PKEY_CTX_new_from_name, OSSL_LIB_CTX *libctx, libctx, const char *name, name, const char *propquery, propquery, return nullptr, return)
DEFINEFUNC3(EVP_PKEY_CTX *, EVP_PKEY_CTX_new_from_pkey, OSSL_LIB_CTX *libctx, libctx, EVP_PKEY *pkey, pkey, const char *propquery, propquery, return nullptr, return)
DEFINEFUNC(void, EVP_PKEY_CTX_free, EVP_PKEY_CTX *ctx, ctx, return, DUMMYARG)
DEFINEFUNC(int, EVP_PKEY_keygen_init, EVP_PKEY_CTX *ctx, ctx, return 0, return)
DEFINEFUNC2(int, EVP_PKEY_generate, EVP_PKEY_CTX *ctx, ctx, EVP_PKEY **ppkey, ppkey, return 0, return)
DEFINEFUNC(int, EVP_PKEY_derive_init, EVP_PKEY_CTX *ctx, ctx, return 0, return)
DEFINEFUNC2(int, EVP_PKEY_derive_set_peer, EVP_PKEY_CTX *ctx, ctx, EVP_PKEY *peer, peer, return 0, return)
DEFINEFUNC3(int, EVP_PKEY_derive, EVP_PKEY_CTX *ctx, ctx, unsigned char *key, key, size_t *keylen, keylen, return 0, return)

#ifndef OPENSSL_NO_AES
DEFINEFUNC(const EVP_CIPHER *, EVP_aes_128_cbc, DUMMYARG, DUMMYARG, return nullptr, return)
#endif // OPENSSL_NO_AES

#define RESOLVEFUNC(func) \
    if (!(_q_##func = _q_PTR_##func(libs.ssl->resolve(#func)))     \
        && !(_q_##func = _q_PTR_##func(libs.crypto->resolve(#func)))) \
        qsslSocketCannotResolveSymbolWarning(#func);

#if !defined QT_LINKED_OPENSSL

#if !QT_CONFIG(library)
bool q_resolveOpenSslSymbols()
{
    qCWarning(lcTlsBackend, "QSslSocket: unable to resolve symbols. Qt is configured without the "
                     "'library' feature, which means runtime resolving of libraries won't work.");
    qCWarning(lcTlsBackend, "Either compile Qt statically or with support for runtime resolving "
                     "of libraries.");
    return false;
}
#else

# ifdef Q_OS_UNIX
struct NumericallyLess
{
    typedef bool result_type;
    result_type operator()(QStringView lhs, QStringView rhs) const
    {
        bool ok = false;
        int b = 0;
        int a = lhs.toInt(&ok);
        if (ok)
            b = rhs.toInt(&ok);
        if (ok) {
            // both toInt succeeded
            return a < b;
        } else {
            // compare as strings;
            return lhs < rhs;
        }
    }
};

struct LibGreaterThan
{
    typedef bool result_type;
    result_type operator()(QStringView lhs, QStringView rhs) const
    {
        const auto lhsparts = lhs.split(u'.');
        const auto rhsparts = rhs.split(u'.');
        Q_ASSERT(lhsparts.size() > 1 && rhsparts.size() > 1);

        // note: checking rhs < lhs, the same as lhs > rhs
        return std::lexicographical_compare(rhsparts.begin() + 1, rhsparts.end(),
                                            lhsparts.begin() + 1, lhsparts.end(),
                                            NumericallyLess());
    }
};

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID) && !defined(Q_OS_HARMONY)
static int dlIterateCallback(struct dl_phdr_info *info, size_t size, void *data)
{
    if (size < sizeof (info->dlpi_addr) + sizeof (info->dlpi_name))
        return 1;
    QDuplicateTracker<QString> *paths = (QDuplicateTracker<QString> *)data;
    QString path = QString::fromLocal8Bit(info->dlpi_name);
    if (!path.isEmpty()) {
        QFileInfo fi(path);
        path = fi.absolutePath();
        if (!path.isEmpty())
            (void)paths->hasSeen(std::move(path));
    }
    return 0;
}
#endif

static QStringList libraryPathList()
{
    QStringList paths;
#  ifdef Q_OS_DARWIN
    paths = QString::fromLatin1(qgetenv("DYLD_LIBRARY_PATH")).split(u':', Qt::SkipEmptyParts);

    // search in .app/Contents/Frameworks
    UInt32 packageType;
    CFBundleGetPackageInfo(CFBundleGetMainBundle(), &packageType, nullptr);
    if (packageType == FOUR_CHAR_CODE('APPL')) {
        QUrl bundleUrl = QUrl::fromCFURL(QCFType<CFURLRef>(CFBundleCopyBundleURL(CFBundleGetMainBundle())));
        QUrl frameworksUrl = QUrl::fromCFURL(QCFType<CFURLRef>(CFBundleCopyPrivateFrameworksURL(CFBundleGetMainBundle())));
        paths << bundleUrl.resolved(frameworksUrl).path();
    }
#  else
    paths = QString::fromLatin1(qgetenv("LD_LIBRARY_PATH")).split(u':', Qt::SkipEmptyParts);
#  endif
    paths << "/lib"_L1 << "/usr/lib"_L1 << "/usr/local/lib"_L1;
    paths << "/lib64"_L1 << "/usr/lib64"_L1 << "/usr/local/lib64"_L1;
    paths << "/lib32"_L1 << "/usr/lib32"_L1 << "/usr/local/lib32"_L1;

#if defined(Q_OS_ANDROID)
    paths << "/system/lib"_L1;
#elif defined(Q_OS_HARMONY)
    paths << "/system/lib64/platformsdk"_L1;
    paths << "/system/lib64/chipset-sdk"_L1;
    paths << "/system/lib64/chipset-sdk-sp"_L1;
#elif defined(Q_OS_LINUX)
    // discover paths of already loaded libraries
    QDuplicateTracker<QString> loadedPaths;
    dl_iterate_phdr(dlIterateCallback, &loadedPaths);
    std::move(loadedPaths).appendTo(paths);
#endif

    return paths;
}

Q_NEVER_INLINE
static QStringList findAllLibs(QLatin1StringView filter)
{
    const QStringList paths = libraryPathList();
    QStringList found;
    const QStringList filters((QString(filter)));

    using F = QDirListing::IteratorFlag;
    for (const QString &path : paths) {
        QStringList entryList;
        for (const auto &dirEntry : QDirListing(path, filters, F::FilesOnly))
            entryList.emplace_back(dirEntry.fileName());

        std::sort(entryList.begin(), entryList.end(), LibGreaterThan());
        for (const QString &entry : std::as_const(entryList))
            found << path + u'/' + entry;
    }

    return found;
}

static QStringList findAllLibSsl()
{
#if defined(Q_OS_HARMONY)
    return findAllLibs("libssl_openssl*"_L1);
#else
    return findAllLibs("libssl.*"_L1);
#endif
}

static QStringList findAllLibCrypto()
{
#if defined(Q_OS_HARMONY)
    return findAllLibs("libcrypto_openssl*"_L1);
#else
    return findAllLibs("libcrypto.*"_L1);
#endif
}
# endif

#if (OPENSSL_VERSION_NUMBER >> 28) < 3
#define QT_OPENSSL_VERSION "1_1"
#elif OPENSSL_VERSION_MAJOR >= 3
#define QT_OPENSSL_VERSION QT_STRINGIFY(OPENSSL_VERSION_MAJOR)
#endif

#ifdef Q_OS_WIN

struct LoadedOpenSsl {
    std::unique_ptr<QSystemLibrary> ssl, crypto;
};

static bool tryToLoadOpenSslWin32Library(QLatin1StringView ssleay32LibName, QLatin1StringView libeay32LibName, LoadedOpenSsl &result)
{
    auto ssleay32 = std::make_unique<QSystemLibrary>(ssleay32LibName);
    if (!ssleay32->load(false)) {
        return FALSE;
    }

    auto libeay32 = std::make_unique<QSystemLibrary>(libeay32LibName);
    if (!libeay32->load(false)) {
        return FALSE;
    }

    result.ssl = std::move(ssleay32);
    result.crypto = std::move(libeay32);
    return TRUE;
}

static LoadedOpenSsl loadOpenSsl()
{
    LoadedOpenSsl result;

    // With OpenSSL 1.1 the names have changed to libssl-1_1 and libcrypto-1_1 for builds using
    // MSVC and GCC. For 3.0 the version suffix changed again, to just '3'.
    // For non-x86 builds, an architecture suffix is also appended.

#if defined(Q_PROCESSOR_X86_64)
#define QT_SSL_SUFFIX "-x64"
#elif defined(Q_PROCESSOR_ARM_64)
#define QT_SSL_SUFFIX "-arm64"
#elif defined(Q_PROCESSOR_ARM_32)
#define QT_SSL_SUFFIX "-arm"
#else
#define QT_SSL_SUFFIX
#endif

    tryToLoadOpenSslWin32Library("libssl-" QT_OPENSSL_VERSION QT_SSL_SUFFIX ""_L1,
                                 "libcrypto-" QT_OPENSSL_VERSION QT_SSL_SUFFIX ""_L1, result);

#undef QT_SSL_SUFFIX
    return result;
}
#else // !Q_OS_WIN:

struct LoadedOpenSsl {
    std::unique_ptr<QLibrary> ssl, crypto;
};

static LoadedOpenSsl loadOpenSsl()
{
    LoadedOpenSsl result = { std::make_unique<QLibrary>(), std::make_unique<QLibrary>() };

# if defined(Q_OS_UNIX)
    QLibrary * const libssl = result.ssl.get();
    QLibrary * const libcrypto = result.crypto.get();

    // Try to find the libssl library on the system.
    //
    // Up until Qt 4.3, this only searched for the "ssl" library at version -1, that
    // is, libssl.so on most Unix systems.  However, the .so file isn't present in
    // user installations because it's considered a development file.
    //
    // The right thing to do is to load the library at the major version we know how
    // to work with: the SHLIB_VERSION_NUMBER version (macro defined in opensslv.h)
    //
    // However, OpenSSL is a well-known case of binary-compatibility breakage. To
    // avoid such problems, many system integrators and Linux distributions change
    // the soname of the binary, letting the full version number be the soname. So
    // we'll find libssl.so.0.9.7, libssl.so.0.9.8, etc. in the system. For that
    // reason, we will search a few common paths (see findAllLibSsl() above) in hopes
    // we find one that works.
    //
    // If that fails, for OpenSSL 1.0 we also try some fallbacks -- look up
    // libssl.so with a hardcoded soname. The reason is QTBUG-68156: the binary
    // builds of Qt happen (at the time of this writing) on RHEL machines,
    // which change SHLIB_VERSION_NUMBER to a non-portable string. When running
    // those binaries on the target systems, this code won't pick up
    // libssl.so.MODIFIED_SHLIB_VERSION_NUMBER because it doesn't exist there.
    // Given that the only 1.0 supported release (at the time of this writing)
    // is 1.0.2, with soname "1.0.0", give that a try too. Note that we mandate
    // OpenSSL >= 1.0.0 with a configure-time check, and OpenSSL has kept binary
    // compatibility between 1.0.0 and 1.0.2.
    //
    // It is important, however, to try the canonical name and the unversioned name
    // without going through the loop. By not specifying a path, we let the system
    // dlopen(3) function determine it for us. This will include any DT_RUNPATH or
    // DT_RPATH tags on our library header as well as other system-specific search
    // paths. See the man page for dlopen(3) on your system for more information.

#ifdef Q_OS_OPENBSD
    libcrypto->setLoadHints(QLibrary::ExportExternalSymbolsHint);
#endif

#if !defined(Q_OS_QNX) // on QNX, the libs are always libssl.so and libcrypto.so

#if defined(OPENSSL_SHLIB_VERSION)
    // OpenSSL v.3 does not have SLIB_VERSION_NUMBER but has OPENSSL_SHLIB_VERSION.
    // The comment about OPENSSL_SHLIB_VERSION in opensslv.h is a bit troublesome:
    // "This is defined in free form."
    auto shlibVersion = QString("%1"_L1).arg(OPENSSL_SHLIB_VERSION);
    libssl->setFileNameAndVersion("ssl"_L1, shlibVersion);
    libcrypto->setFileNameAndVersion("crypto"_L1, shlibVersion);
#elif defined(SHLIB_VERSION_NUMBER)
    // first attempt: the canonical name is libssl.so.<SHLIB_VERSION_NUMBER>
    libssl->setFileNameAndVersion("ssl"_L1, SHLIB_VERSION_NUMBER ""_L1);
    libcrypto->setFileNameAndVersion("crypto"_L1, SHLIB_VERSION_NUMBER ""_L1);
#endif // OPENSSL_SHLIB_VERSION

    if (libcrypto->load() && libssl->load()) {
        // libssl.so.<SHLIB_VERSION_NUMBER> and libcrypto.so.<SHLIB_VERSION_NUMBER> found
        return result;
    } else {
        libssl->unload();
        libcrypto->unload();
    }
#endif // !defined(Q_OS_QNX)

#ifndef Q_OS_DARWIN
    // second attempt: find the development files libssl.so and libcrypto.so
    //
    // disabled on macOS/iOS:
    //  macOS's /usr/lib/libssl.dylib, /usr/lib/libcrypto.dylib will be picked up in the third
    //    attempt, _after_ <bundle>/Contents/Frameworks has been searched.
    //  iOS does not ship a system libssl.dylib, libcrypto.dylib in the first place.
# if defined(Q_OS_ANDROID)
    // OpenSSL 1.1.x must be suffixed otherwise it will use the system libcrypto.so libssl.so which on API-21 are OpenSSL 1.0 not 1.1
    auto openSSLSuffix = [](const QByteArray &defaultSuffix = {}) {
        auto suffix = qgetenv("ANDROID_OPENSSL_SUFFIX");
        if (suffix.isEmpty())
            return defaultSuffix;
        return suffix;
    };

    static QString suffix = QString::fromLatin1(openSSLSuffix("_" QT_OPENSSL_VERSION));

    libssl->setFileNameAndVersion("ssl"_L1 + suffix, -1);
    libcrypto->setFileNameAndVersion("crypto"_L1 + suffix, -1);
# else
    libssl->setFileNameAndVersion("ssl"_L1, -1);
    libcrypto->setFileNameAndVersion("crypto"_L1, -1);
# endif
    if (libcrypto->load() && libssl->load()) {
        // libssl.so.0 and libcrypto.so.0 found
        return result;
    } else {
        libssl->unload();
        libcrypto->unload();
    }
#endif

    // third attempt: loop on the most common library paths and find libssl
    const QStringList sslList = findAllLibSsl();
    const QStringList cryptoList = findAllLibCrypto();

    for (const QString &crypto : cryptoList) {
#ifdef Q_OS_DARWIN
        // Clients should not load the unversioned libcrypto dylib as it does not have a stable ABI
        if (crypto.endsWith("libcrypto.dylib"_L1))
            continue;
#endif
        libcrypto->setFileNameAndVersion(crypto, -1);
        if (libcrypto->load()) {
            QFileInfo fi(crypto);
            QString version = fi.completeSuffix();

            for (const QString &ssl : sslList) {
                if (!ssl.endsWith(version))
                    continue;

                libssl->setFileNameAndVersion(ssl, -1);

                if (libssl->load()) {
                    // libssl.so.x and libcrypto.so.x found
                    return result;
                } else {
                    libssl->unload();
                }
            }
        }
        libcrypto->unload();
    }

    // failed to load anything
    result = {};
    return result;

# else
    // not implemented for this platform yet
    return result;
# endif
}
#endif

bool q_resolveOpenSslSymbols()
{
    static bool symbolsResolved = []() {
        LoadedOpenSsl libs = loadOpenSsl();
        if (!libs.ssl || !libs.crypto) {
            qCWarning(lcTlsBackend, "Failed to load libssl/libcrypto.");
            return false;
        }

        RESOLVEFUNC(OPENSSL_init_ssl)
        RESOLVEFUNC(OPENSSL_init_crypto)
        RESOLVEFUNC(OpenSSL_version_num)
        RESOLVEFUNC(OpenSSL_version)

        if (!_q_OpenSSL_version || !_q_OpenSSL_version_num) {
            qCWarning(lcTlsBackend, "Incompatible version of OpenSSL");
            return false;
        }

#if OPENSSL_VERSION_NUMBER >= 0x30000000
        if (q_OpenSSL_version_num() < 0x30000000) {
            qCWarning(lcTlsBackend, "Incompatible version of OpenSSL (built with OpenSSL >= 3.x, runtime version is < 3.x)");
            return false;
        }
#else
        if (q_OpenSSL_version_num() >= 0x30000000) {
            qCWarning(lcTlsBackend, "Incompatible version of OpenSSL (built with OpenSSL 1.x, runtime version is >= 3.x)");
            return false;
        }
#endif // OPENSSL_VERSION_NUMBER

        RESOLVEFUNC(EVP_CIPHER_CTX_new)
        RESOLVEFUNC(EVP_CIPHER_CTX_free)
        RESOLVEFUNC(EVP_CIPHER_CTX_reset)
        RESOLVEFUNC(EVP_CIPHER_CTX_get_key_length)
        RESOLVEFUNC(EVP_CIPHER_CTX_get_iv_length)
        RESOLVEFUNC(EVP_CIPHER_CTX_set_padding)
        RESOLVEFUNC(EVP_CIPHER_get_block_size)
        RESOLVEFUNC(EVP_PKEY_type)
        RESOLVEFUNC(EVP_CipherInit_ex)
        RESOLVEFUNC(EVP_CipherUpdate)
        RESOLVEFUNC(EVP_CipherFinal_ex)
        RESOLVEFUNC(EVP_PKEY_free)
        RESOLVEFUNC(EVP_PKEY_get_raw_private_key)
        RESOLVEFUNC(EVP_PKEY_get_raw_public_key)
        RESOLVEFUNC(EVP_PKEY_new_raw_private_key)
        RESOLVEFUNC(EVP_PKEY_new_raw_public_key)
        RESOLVEFUNC(EVP_PKEY_up_ref)
        RESOLVEFUNC(EVP_PKEY_CTX_new_from_name)
        RESOLVEFUNC(EVP_PKEY_CTX_new_from_pkey)
        RESOLVEFUNC(EVP_PKEY_CTX_free)
        RESOLVEFUNC(EVP_PKEY_keygen_init)
        RESOLVEFUNC(EVP_PKEY_generate)
        RESOLVEFUNC(EVP_PKEY_derive_init)
        RESOLVEFUNC(EVP_PKEY_derive_set_peer)
        RESOLVEFUNC(EVP_PKEY_derive)
#ifndef OPENSSL_NO_AES
        RESOLVEFUNC(EVP_aes_128_cbc)
#endif
        return true;
    }();

    return symbolsResolved;
}
#endif // QT_CONFIG(library)

#else // !defined QT_LINKED_OPENSSL

bool q_resolveOpenSslSymbols()
{
#ifdef QT_NO_OPENSSL
    return false;
#endif
    return true;
}
#endif // !defined QT_LINKED_OPENSSL

} // namespace QKnxPrivate

QT_END_NAMESPACE
