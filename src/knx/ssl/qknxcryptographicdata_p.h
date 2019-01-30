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

#ifndef QKNXCRYPTOGRAPHICDATA_P_H
#define QKNXCRYPTOGRAPHICDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt KNX API.  It exists for the convenience
// of the Qt KNX implementation.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtKnx/qknxcurve25519.h>
#include <private/qtnetworkglobal_p.h>

#if QT_CONFIG(opensslv11)
#include <QtKnx/private/qsslsocket_openssl_symbols_p.h>
#include <QtKnx/private/qsslsocket_openssl11_symbols_p.h>
#endif

QT_BEGIN_NAMESPACE

#if QT_CONFIG(opensslv11)
class Q_KNX_EXPORT QKnxOpenSsl
{
public:
    static bool supportsSsl();
    static long sslLibraryVersionNumber();
    static QString sslLibraryVersionString();
    static long sslLibraryBuildVersionNumber();
    static QString sslLibraryBuildVersionString();

protected:
    static bool ensureLibraryLoaded();

private:
    static bool s_libraryLoaded;
    static bool s_libraryEnabled;
};
#endif

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
    EVP_PKEY *m_evpPKey { nullptr };
#endif
    QKnxSecureKey::Type m_type { QKnxSecureKey::Type::Invalid };
};

QT_END_NAMESPACE

#endif
