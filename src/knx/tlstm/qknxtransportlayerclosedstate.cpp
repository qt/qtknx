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

#include "qknxtransportlayerclosedstate_p.h"
#include "qknxtransportlayerconnectingstate_p.h"
#include "qknxtransportlayeropenidlestate_p.h"
#include "qknxtransportlayerstatemachine_p.h"

QT_BEGIN_NAMESPACE

void QKnxTransportLayerClosedState::event00()
{
    sm->doActionA1();
    sm->setActiveState(new QKnxTransportLayerOpenIdleState(sm));
}

void QKnxTransportLayerClosedState::event01()
{
    sm->doActionA1();
    sm->setActiveState(new QKnxTransportLayerOpenIdleState(sm));
}

void QKnxTransportLayerClosedState::event25()
{
    sm->doActionA12();
    sm->setActiveState(new QKnxTransportLayerConnectingState(sm));
}

void QKnxTransportLayerClosedState::event26()
{
    sm->doActionA15();
}

QString QKnxTransportLayerClosedState::name() const
{
    return QStringLiteral("Closed state.");
}

QKnxTransportLayerState::State QKnxTransportLayerClosedState::state() const
{
    return QKnxTransportLayerState::State::Closed;
}

QT_END_NAMESPACE
