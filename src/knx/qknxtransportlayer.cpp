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

#include "qknxtransportlayer.h"
#include "qknxtransportlayer_p.h"

QT_BEGIN_NAMESPACE

QKnxTransportLayer::State QKnxTransportLayer::state() const
{
    return State();
}

QKnxTransportLayer::Error QKnxTransportLayer::error() const
{
    return Error();
}

QString QKnxTransportLayer::errorMessage() const
{
    return QString();
}

QKnxTransportLayer::QKnxTransportLayer(QObject *parent)
    : QKnxTransportLayer(nullptr, parent)
{}

QKnxTransportLayer::QKnxTransportLayer(QKnxLinkLayerDevice *lld, QObject *parent)
    : QObject(*new QKnxTransportLayerPrivate, parent)
{
    Q_D(QKnxTransportLayer);
    d->m_lld = lld;
}

QKnxTransportLayer::QKnxTransportLayer(const QKnxDeviceConfiguration &dc, const QKnxConnectionInfo
        &ci, QObject *parent)
    : QKnxTransportLayer(new QKnxLinkLayerDevice(dc, ci, this), parent)
{}

bool QKnxTransportLayer::connect(const QKnxAddress &dest)
{
    Q_UNUSED(dest)
    return false;
}

void QKnxTransportLayer::disconnect(const QKnxAddress &dest)
{
    Q_UNUSED(dest)
}

bool QKnxTransportLayer::isConnected(const QKnxAddress &dest)
{
    Q_UNUSED(dest)
    return false;
}

QKnxLinkLayerDevice *QKnxTransportLayer::linkLayerDevice() const
{
    return nullptr;
}

void QKnxTransportLayer::setLinkLayerDevice(QKnxLinkLayerDevice *lld)
{
    Q_UNUSED(lld)
}

bool QKnxTransportLayer::send(const QKnxAddress &dest, const QKnxTpdu &tpdu)
{
    Q_UNUSED(dest)
    Q_UNUSED(tpdu)
    return false;
}

bool QKnxTransportLayer::sendConnected(const QKnxAddress &dest, const QKnxTpdu &tpdu)
{
    Q_UNUSED(dest)
    Q_UNUSED(tpdu)
    return false;
}

bool QKnxTransportLayer::send(Broadcast broadcast, Priority priority, const QKnxTpdu &tpdu)
{
    Q_UNUSED(broadcast)
    Q_UNUSED(priority)
    Q_UNUSED(tpdu)
    return false;
}

void QKnxTransportLayer::setState(State state)
{
    Q_UNUSED(state)
}

void QKnxTransportLayer::setError(Error error, const QString &message)
{
    Q_UNUSED(error)
    Q_UNUSED(message)
}

QT_END_NAMESPACE
