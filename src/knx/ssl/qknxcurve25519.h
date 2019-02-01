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

#ifndef QKNXCURVE25519_H
#define QKNXCURVE25519_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxsecurekey.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxCryptographicEngine final
{
public:
    QKnxCryptographicEngine() = delete;
    ~QKnxCryptographicEngine() = default;

    static QKnxByteArray sessionKey(const QKnxSecureKey &privateKey,
                                    const QKnxSecureKey &peerPublicKey);
    static QKnxByteArray sessionKey(const QKnxByteArray &privateKey,
                                    const QKnxByteArray &peerPublicKey);
    static QKnxByteArray sessionKey(const QKnxByteArray &sharedSecret);

    static QKnxByteArray userPasswordHash(const QByteArray &password);
    static QKnxByteArray deviceAuthenticationCodeHash(const QByteArray &password);

    static QKnxByteArray XOR(const QKnxByteArray &l, const QKnxByteArray &r, bool adjust = true);

    static QKnxByteArray computeMessageAuthenticationCode(const QKnxByteArray &key,
                                                          const QKnxNetIpFrameHeader &header,
                                                          quint16 id,
                                                          const QKnxByteArray &data,
                                                          quint48 sequenceNumber = 0,
                                                          const QKnxByteArray &serialNumber = {},
                                                          quint16 messageTag = 0);

    static QKnxByteArray encryptSecureWrapperPayload(const QKnxByteArray &key,
                                                     const QKnxNetIpFrame &frame,
                                                     quint48 sequenceNumber,
                                                     const QKnxByteArray &serialNumber,
                                                     quint16 messageTag);

    static QKnxByteArray decryptSecureWrapperPayload(const QKnxByteArray &key,
                                                     const QKnxByteArray &frame,
                                                     quint48 sequenceNumber,
                                                     const QKnxByteArray &serialNumber,
                                                     quint16 messageTag);

    static QKnxByteArray encryptMessageAuthenticationCode(const QKnxByteArray &key,
                                                          const QKnxByteArray &mac,
                                                          quint48 sequenceNumber = 0,
                                                          const QKnxByteArray &serialNumber = {},
                                                          quint16 messageTag = 0);

    static QKnxByteArray decryptMessageAuthenticationCode(const QKnxByteArray &key,
                                                          const QKnxByteArray &data,
                                                          quint48 sequenceNumber = 0,
                                                          const QKnxByteArray &serialNumber = {},
                                                          quint16 messageTag = 0);
};

QT_END_NAMESPACE

#endif
