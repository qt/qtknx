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

void QKnxTransportLayerConnectingState::event01()
{
    sm->doActionA10();
}

void QKnxTransportLayerConnectingState::event02()
{
    sm->doActionA5();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event04()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event05()
{
    sm->doActionA3();
}

void QKnxTransportLayerConnectingState::event06()
{
    sm->doActionA6();
}

void QKnxTransportLayerConnectingState::event07()
{
    sm->doActionA10();
}

void QKnxTransportLayerConnectingState::event08()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event09()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event10()
{
    sm->doActionA10();
}

void QKnxTransportLayerConnectingState::event11()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event12()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event13()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event14()
{
    sm->doActionA10();
}

void QKnxTransportLayerConnectingState::event15()
{
    sm->doActionA11();
}

void QKnxTransportLayerConnectingState::event16()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event19()
{
    sm->doActionA13();
    sm->setActiveState(new QKnxTransportLayerOpenIdleState(sm));
}

void QKnxTransportLayerConnectingState::event20()
{
    sm->doActionA5();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event25()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerConnectingState::event26()
{
    sm->doActionA14();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

QString QKnxTransportLayerConnectingState::name() const
{
    return QStringLiteral("Connecting state.");
}

QKnxTransportLayerState::State QKnxTransportLayerConnectingState::state() const
{
    return QKnxTransportLayerState::State::Connecting;
}

QT_END_NAMESPACE
