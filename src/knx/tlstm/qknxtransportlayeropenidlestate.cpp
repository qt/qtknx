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
#include "qknxtransportlayeropenidlestate_p.h"
#include "qknxtransportlayeropenwaitstate_p.h"
#include "qknxtransportlayerstatemachine_p.h"

QT_BEGIN_NAMESPACE

void QKnxTransportLayerOpenIdleState::event01()
{
    sm->doActionA10();
}

void QKnxTransportLayerOpenIdleState::event02()
{
    sm->doActionA5();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenIdleState::event04()
{
    sm->doActionA2();
}

void QKnxTransportLayerOpenIdleState::event05()
{
    sm->doActionA3();
}

void QKnxTransportLayerOpenIdleState::event06()
{
    sm->doActionA4();
}

void QKnxTransportLayerOpenIdleState::event12()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenIdleState::event13()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenIdleState::event15()
{
    sm->doActionA7();
    sm->setActiveState(new QKnxTransportLayerOpenWaitState(sm));
}

void QKnxTransportLayerOpenIdleState::event16()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenIdleState::event25()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenIdleState::event26()
{
    sm->doActionA14();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

QString QKnxTransportLayerOpenIdleState::name() const
{
    return QStringLiteral("Open idle state.");
}

QKnxTransportLayerState::State QKnxTransportLayerOpenIdleState::state() const
{
    return QKnxTransportLayerState::State::OpenIdle;
}

QT_END_NAMESPACE
