/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include "qknxnetipdescriptionresponse.h"
#include "qknxnetipservicefamiliesdib.h"
#include "qknxnetipdevicedib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDescriptionResponseProxy::QKnxNetIpDescriptionResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

bool QKnxNetIpDescriptionResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 64
        && m_frame.serviceType() == QKnxNetIp::ServiceType::DescriptionResponse;
}

QKnxNetIpDib QKnxNetIpDescriptionResponseProxy::deviceHardware() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 0);
}

QKnxNetIpDib QKnxNetIpDescriptionResponseProxy::supportedFamilies() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 54);
}

QVector<QKnxNetIpDib> QKnxNetIpDescriptionResponseProxy::optionalDibs() const
{
    const auto &data = m_frame.constData();

    auto header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, 0);
    quint16 index = header.totalSize(); // total size of device DIB

    header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, index);
    index += header.totalSize(); // advance of total size of families DIB

    QVector<QKnxNetIpDib> dibs;
    while (index < data.size()) {
        header = QKnxNetIpStructHeader<QKnxNetIp::DescriptionType>::fromBytes(data, index);
        if (!header.isValid())
            return {};
        dibs.append(QKnxNetIpDib::fromBytes(data, index));
        index += header.totalSize(); // advance of total size of last read DIB
    }
    return dibs;
}

QKnxNetIpDescriptionResponseProxy::Builder QKnxNetIpDescriptionResponseProxy::builder()
{
    return QKnxNetIpDescriptionResponseProxy::Builder();
}


// -- QKnxNetIpSearchResponseProxy::Builder

QKnxNetIpDescriptionResponseProxy::Builder &
QKnxNetIpDescriptionResponseProxy::Builder::setDeviceHardware(const QKnxNetIpDib &ddib)
{
    if (QKnxNetIpDeviceDibProxy(ddib).isValid())
        m_ddib = ddib;
    return *this;
}

QKnxNetIpDescriptionResponseProxy::Builder &
    QKnxNetIpDescriptionResponseProxy::Builder::setSupportedFamilies(const QKnxNetIpDib &sdib)
{
    if (QKnxNetIpServiceFamiliesDibProxy(sdib).isValid())
        m_sdib = sdib;
    return *this;
}

QKnxNetIpDescriptionResponseProxy::Builder &
    QKnxNetIpDescriptionResponseProxy::Builder::setOptionalDibs(const QVector<QKnxNetIpDib> &dibs)
{
    m_optionalDibs = dibs;
    return *this;
}

QKnxNetIpFrame QKnxNetIpDescriptionResponseProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::DescriptionResponse, m_ddib.bytes() + m_sdib.bytes()
        + [&]() -> QKnxByteArray {
            QKnxByteArray bytes;
            for (const auto &dib : m_optionalDibs) {
                if (dib.isValid())
                    bytes += dib.bytes();
            }
            return bytes;
        }()
    };
}

QT_END_NAMESPACE
