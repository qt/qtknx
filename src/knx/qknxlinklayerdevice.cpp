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

#include "qknxlinklayerdevice.h"
#include "qknxlinklayerdevice_p.h"

QT_BEGIN_NAMESPACE

QKnxLinkLayerDevice::State QKnxLinkLayerDevice::state() const
{
    return State();
}

QKnxLinkLayerDevice::Error QKnxLinkLayerDevice::error() const
{
    return Error();
}

QString QKnxLinkLayerDevice::errorMessage() const
{
    return QString();
}

QKnxLinkLayerDevice::QKnxLinkLayerDevice(QObject *parent)
    : QKnxLinkLayerDevice({}, {}, parent)
{}

QKnxLinkLayerDevice::QKnxLinkLayerDevice(const QKnxDeviceConfiguration &dc, QObject *parent)
    : QKnxLinkLayerDevice(dc , {}, parent)
{}

QKnxLinkLayerDevice::QKnxLinkLayerDevice(const QKnxDeviceConfiguration &dc,
        const QKnxConnectionInfo &ci, QObject *parent)
    : QIODevice(parent)
{
    Q_D(QKnxLinkLayerDevice);
    d->m_ci = ci;
    d->m_dc = dc;
}

bool QKnxLinkLayerDevice::connect()
{
    return false;
}

void QKnxLinkLayerDevice::disconnect()
{
}

QKnxDeviceConfiguration QKnxLinkLayerDevice::deviceConfig() const
{
    return QKnxDeviceConfiguration();
}

void QKnxLinkLayerDevice::setDeviceConfig(const QKnxDeviceConfiguration &dc)
{
    Q_D(QKnxLinkLayerDevice);
    d->m_dc = dc;
}

QKnxConnectionInfo QKnxLinkLayerDevice::connectionInfo() const
{
    return QKnxConnectionInfo();
}

void QKnxLinkLayerDevice::setConnectionInfo(const QKnxConnectionInfo &ci)
{
    Q_D(QKnxLinkLayerDevice);
    d->m_ci = ci;
}

bool QKnxLinkLayerDevice::send(const QKnxLinkLayerFrame &frame, bool waitForConfirmation)
{
    Q_UNUSED(frame)
    Q_UNUSED(waitForConfirmation)
    return false;
}

void QKnxLinkLayerDevice::send(const QKnxAddress &dest, const QKnxTpdu &tpdu, bool waitForConfirmation)
{
    Q_UNUSED(dest)
    Q_UNUSED(tpdu)
    Q_UNUSED(waitForConfirmation)
}

/*!
    \reimp

    Returns \c true if this device is sequential; otherwise returns false.

    Sequential devices, as opposed to a random-access devices, have no concept
    of a start, an end, a size, or a current position, and they do not support
    seeking.
*/
bool QKnxLinkLayerDevice::isSequential() const
{
    Q_D(const QKnxLinkLayerDevice);
    if (d->m_device)
        return d->m_device->isSequential();
    return true;
}

qint64 QKnxLinkLayerDevice::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    return maxlen;
}

qint64 QKnxLinkLayerDevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    return qint64(len);
}

void QKnxLinkLayerDevice::setState(State state)
{
    Q_D(QKnxLinkLayerDevice);
    d->m_state = state;
}

void QKnxLinkLayerDevice::setError(Error error, const QString &message)
{
    Q_D(QKnxLinkLayerDevice);

    d->m_error = error;
    d->m_errorString = message;
}

QT_END_NAMESPACE
