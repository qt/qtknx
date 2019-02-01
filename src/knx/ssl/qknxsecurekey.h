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

#ifndef QKNXSECUREKEY_H
#define QKNXSECUREKEY_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class QKnxSecureKeyData;
class Q_KNX_EXPORT QKnxSecureKey final
{
public:
    enum class Type : quint8 {
        Private,
        Public,
        Invalid
    };
    QKnxSecureKey::Type type() const;

    QKnxSecureKey();
    ~QKnxSecureKey();

    bool isNull() const;
    bool isValid() const;

    QKnxByteArray bytes() const;
    static QKnxSecureKey fromBytes(QKnxSecureKey::Type type,
                                   const QKnxByteArray &data,
                                   quint16 index = 0);

    static QKnxSecureKey generatePrivateKey();

    static QKnxSecureKey publicKeyFromPrivate(const QKnxSecureKey &privateKey);
    static QKnxSecureKey publicKeyFromPrivate(const QKnxByteArray &privateKey);

    static void generateKeys(QKnxSecureKey *privateKey, QKnxSecureKey *publicKey);

    static QKnxByteArray sharedSecret(const QKnxSecureKey &privateKey,
                                      const QKnxSecureKey &peerPublicKey);
    static QKnxByteArray sharedSecret(const QKnxByteArray &privateKey,
                                      const QKnxByteArray &peerPublicKey);

    QKnxSecureKey(const QKnxSecureKey &other);
    QKnxSecureKey &operator=(const QKnxSecureKey &other);

    bool operator==(const QKnxSecureKey &other) const;
    bool operator!=(const QKnxSecureKey &other) const;

private:
    QSharedDataPointer<QKnxSecureKeyData> d_ptr;
};

QT_END_NAMESPACE

#endif
