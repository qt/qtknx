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

#ifndef QKNXNETIPFRAME_H
#define QKNXNETIPFRAME_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtKnx/qknxnetipframeheader.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpFramePrivate;
class Q_KNX_EXPORT QKnxNetIpFrame
{
public:
    QKnxNetIpFrame();
    ~QKnxNetIpFrame();

    QKnxNetIpFrame(QKnxNetIp::ServiceType type, const QKnxByteArray &data = {});
    QKnxNetIpFrame(QKnxNetIp::ServiceType type,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data = {});
    QKnxNetIpFrame(const QKnxNetIpFrameHeader &header,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data = {});

    bool isNull()const;
    bool isValid() const;

    quint16 size() const;
    quint16 dataSize() const;

    QKnxNetIpFrameHeader header() const;
    void setHeader(const QKnxNetIpFrameHeader &header);

    quint8 protocolVersion() const;

    QKnxNetIp::ServiceType serviceType() const;
    void setServiceType(QKnxNetIp::ServiceType type);

    QKnxNetIpConnectionHeader connectionHeader() const;
    void setConnectionHeader(const QKnxNetIpConnectionHeader &header);

    quint8 channelId() const;
    quint8 sequenceNumber() const;
    quint8 serviceTypeSpecificValue() const;
    QKnxByteArray connectionTypeSpecificHeaderItems() const;

    QKnxByteArray data() const;
    const QKnxByteArray &constData() const;
    void setData(const QKnxByteArray &data);

    QKnxByteArray bytes() const;
    static QKnxNetIpFrame fromBytes(const QKnxByteArray &bytes, quint16 index = 0);

    QKnxNetIpFrame(const QKnxNetIpFrame &other);
    QKnxNetIpFrame &operator=(const QKnxNetIpFrame &other);

    QKnxNetIpFrame(QKnxNetIpFrame &&other) Q_DECL_NOTHROW;
    QKnxNetIpFrame &operator=(QKnxNetIpFrame &&other) Q_DECL_NOTHROW;

    void swap(QKnxNetIpFrame &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxNetIpFrame &other) const;
    bool operator!=(const QKnxNetIpFrame &other) const;

private:
    explicit QKnxNetIpFrame(QKnxNetIpFramePrivate &dd);

private:
    QSharedDataPointer<QKnxNetIpFramePrivate> d_ptr;
};

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpFrame &frame);

QT_END_NAMESPACE

#endif
