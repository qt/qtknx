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

#ifndef QKNXNETIPTUNNELINGINFODIB_H
#define QKNXNETIPTUNNELINGINFODIB_H

#include <QtCore/qshareddata.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxnetipstruct.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpTunnelingSlotInfoPrivate;
class Q_KNX_EXPORT QKnxNetIpTunnelingSlotInfo final
{
    Q_GADGET

public:
    enum State : quint32
    {
        NotAvailable = 0x00000000,
        Free = 0x00000001,
        Authorized = 0x00000002,
        Usable = 0x00000004,
        Available = Free | Authorized | Usable,
        Invalid = 0xffffffff
    };
    Q_ENUM(State)
    Q_DECLARE_FLAGS(Status, State)

    QKnxNetIpTunnelingSlotInfo();
    ~QKnxNetIpTunnelingSlotInfo();

    explicit QKnxNetIpTunnelingSlotInfo(const QKnxAddress &ia);
    QKnxNetIpTunnelingSlotInfo(const QKnxAddress &is, QKnxNetIpTunnelingSlotInfo::Status status);

    bool isValid() const;

    QKnxAddress individualAddress() const;
    void setIndividualAddress(const QKnxAddress &ia);

    QKnxNetIpTunnelingSlotInfo::Status status() const;
    void setStatus(QKnxNetIpTunnelingSlotInfo::Status status);

    QKnxByteArray bytes() const;
    static QKnxNetIpTunnelingSlotInfo fromBytes(const QKnxByteArray &data, quint16 index);

    QKnxNetIpTunnelingSlotInfo(const QKnxNetIpTunnelingSlotInfo &other);
    QKnxNetIpTunnelingSlotInfo &operator=(const QKnxNetIpTunnelingSlotInfo &other);

    void swap(QKnxNetIpTunnelingSlotInfo &other) Q_DECL_NOTHROW;

    QKnxNetIpTunnelingSlotInfo(QKnxNetIpTunnelingSlotInfo &&other) Q_DECL_NOTHROW;
    QKnxNetIpTunnelingSlotInfo &operator=(QKnxNetIpTunnelingSlotInfo &&other) Q_DECL_NOTHROW;

    bool operator==(const QKnxNetIpTunnelingSlotInfo &other) const;
    bool operator!=(const QKnxNetIpTunnelingSlotInfo &other) const;

private:
    QSharedDataPointer<QKnxNetIpTunnelingSlotInfoPrivate> d_ptr;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpTunnelingSlotInfo::Status)

class QKnxNetIpTunnelingInfoDibPrivate;
class Q_KNX_EXPORT QKnxNetIpTunnelingInfoDibProxy final
{
public:
    QKnxNetIpTunnelingInfoDibProxy() = delete;
    ~QKnxNetIpTunnelingInfoDibProxy() = default;

    QKnxNetIpTunnelingInfoDibProxy(const QKnxNetIpDib &&) = delete;
    explicit QKnxNetIpTunnelingInfoDibProxy(const QKnxNetIpDib &dib);

    bool isValid() const;

    QKnxNetIp::DescriptionType descriptionType() const;

    quint16 maximumInterfaceApduLength() const;
    QKnxNetIpTunnelingSlotInfo tunnelingSlotInfo() const;
    QVector<QKnxNetIpTunnelingSlotInfo> optionalSlotInfos() const;

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder();
        ~Builder();

        Builder &setMaximumInterfaceApduLength(quint16 length);
        Builder &setTunnelingSlotInfo(const QKnxNetIpTunnelingSlotInfo &info);
        Builder &setOptionalSlotInfos(const QVector<QKnxNetIpTunnelingSlotInfo> &infos);

        QKnxNetIpDib create() const;

        Builder(const Builder &other);
        Builder &operator=(const Builder &other);

    private:
        QSharedDataPointer<QKnxNetIpTunnelingInfoDibPrivate> d_ptr;
    };
    static QKnxNetIpTunnelingInfoDibProxy::Builder builder();

private:
    const QKnxNetIpDib &m_dib;
};

QT_END_NAMESPACE

#endif
