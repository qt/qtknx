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

void QKnxTransportLayerOpenWaitState::event01()
{
    sm->doActionA10();
}

void QKnxTransportLayerOpenWaitState::event02()
{
    sm->doActionA5();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenWaitState::event04()
{
    sm->doActionA2();
}

void QKnxTransportLayerOpenWaitState::event05()
{
    sm->doActionA3();
}

void QKnxTransportLayerOpenWaitState::event06()
{
    sm->doActionA4();
}

void QKnxTransportLayerOpenWaitState::event08()
{
    sm->doActionA8();
    sm->setActiveState(new QKnxTransportLayerOpenIdleState(sm));
}

void QKnxTransportLayerOpenWaitState::event09()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenWaitState::event12()
{
    sm->doActionA9();
}

void QKnxTransportLayerOpenWaitState::event13()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenWaitState::event15()
{
    sm->doActionA11();
}

void QKnxTransportLayerOpenWaitState::event16()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenWaitState::event17()
{
    sm->doActionA9();
}

void QKnxTransportLayerOpenWaitState::event18()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenWaitState::event25()
{
    sm->doActionA6();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

void QKnxTransportLayerOpenWaitState::event26()
{
    sm->doActionA14();
    sm->setActiveState(new QKnxTransportLayerClosedState(sm));
}

QString QKnxTransportLayerOpenWaitState::name() const
{
    return QStringLiteral("Open wait state.");
}

QKnxTransportLayerState::State QKnxTransportLayerOpenWaitState::state() const
{
    return QKnxTransportLayerState::State::OpenWait;
}

QT_END_NAMESPACE
