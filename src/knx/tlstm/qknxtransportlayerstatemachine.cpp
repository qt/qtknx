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
#include "qknxtransportlayerstatemachine_p.h"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(QT_KNX_TL_STM, "qt.qknxtransportlayerstatemachine")
Q_LOGGING_CATEGORY(QT_KNX_TL_STM_LOW, "qt.qknxtransportlayerstatemachine.low")

QKnxTransportLayerStateMachine::QKnxTransportLayerStateMachine(QObject *parent)
    : QObject(parent)
{
    connectionTimeoutTimer.setInterval(6000);
    connectionTimeoutTimer.setSingleShot(true);
    connect(&connectionTimeoutTimer, &QTimer::timeout, this, [&]() {
        qCDebug(QT_KNX_TL_STM) << "Connection timeout.";
        processEvent(Event::Event16);
    });

    acknowledgmentTimeoutTimer.setInterval(3000);
    acknowledgmentTimeoutTimer.setSingleShot(true);
    connect(&acknowledgmentTimeoutTimer, &QTimer::timeout, this, [&]() {
        qCDebug(QT_KNX_TL_STM) << "Acknowledgment timeout.";
        qCDebug(QT_KNX_TL_STM) << "Repetition count:" << repCount;
        processEvent(repCount < 3 ? Event::Event17 : Event::Event18);
    });

    setActiveState(new QKnxTransportLayerClosedState(this));
}

QKnxTransportLayerStateMachine::~QKnxTransportLayerStateMachine()
{
    delete state;
}

QKnxTransportLayerState *QKnxTransportLayerStateMachine::activeState() const
{
    return state;
}

void QKnxTransportLayerStateMachine::setActiveState(QKnxTransportLayerState *activeState)
{
    qSwap(state, activeState);
    delete activeState;

    qCDebug(QT_KNX_TL_STM) << "Active state changed to:" << state->name();

    switch (state->state())
    {
    case QKnxTransportLayerState::State::Closed:
        emit closed();
        break;
    case QKnxTransportLayerState::State::Connecting:
        emit connecting();
        break;
    case QKnxTransportLayerState::State::OpenIdle:
        emit openIdle();
        break;
    case QKnxTransportLayerState::State::OpenWait:
        emit openWait();
        break;
    }
    emit stateChanged(state->state());
}

void QKnxTransportLayerStateMachine::processEvent(Event event)
{
    qCDebug(QT_KNX_TL_STM) << "Process event:" << event;

    if (!state)
        return;

    switch (event) {
    case Event::Event00:
        state->event00();
        break;
    case Event::Event01:
        state->event01();
        break;
    case Event::Event02:
        state->event02();
        break;
    case Event::Event03:
        state->event03();
        break;
    case Event::Event04:
        state->event04();
        break;
    case Event::Event05:
        state->event05();
        break;
    case Event::Event06:
        state->event06();
        break;
    case Event::Event07:
        state->event07();
        break;
    case Event::Event08:
        state->event08();
        break;
    case Event::Event09:
        state->event09();
        break;
    case Event::Event10:
        state->event10();
        break;
    case Event::Event11:
        state->event11();
        break;
    case Event::Event12:
        state->event12();
        break;
    case Event::Event13:
        state->event13();
        break;
    case Event::Event14:
        state->event14();
        break;
    case Event::Event15:
        state->event15();
        break;
    case Event::Event16:
        state->event16();
        break;
    case Event::Event17:
        state->event17();
        break;
    case Event::Event18:
        state->event18();
        break;
    case Event::Event19:
        state->event19();
        break;
    case Event::Event20:
        state->event20();
        break;
    case Event::Event21:
        state->event21();
        break;
    case Event::Event22:
        state->event22();
        break;
    case Event::Event23:
        state->event23();
        break;
    case Event::Event24:
        state->event24();
        break;
    case Event::Event25:
        state->event25();
        break;
    case Event::Event26:
        state->event26();
        break;
    case Event::Event27:
        state->event27();
        break;
    }
}

void QKnxTransportLayerStateMachine::doActionA1()
{
    qCDebug(QT_KNX_TL_STM) << "Action A1!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Connection_address = source address of received message.";
    qCDebug(QT_KNX_TL_STM_LOW) << "Send a T_CONNECT_ind to the user.";
    seqNoSend = 0; seqNoRcv = 0; qCDebug(QT_KNX_TL_STM_LOW) << "seqNoSend = 0; seqNoRcv = 0.";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Start connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA2()
{
    qCDebug(QT_KNX_TL_STM) << "Action A2!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, "
        "destination = connection_address, sequence = SeqNoRcv to the Network Layer (remote device).";
    ++seqNoRcv &= 0x0f; qCDebug(QT_KNX_TL_STM_LOW) << "Increment the seqNoRcv.";
    qCDebug(QT_KNX_TL_STM_LOW) << "Send the received buffer as a T_Data_Connected.ind to the user.";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Restart the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA3()
{
    qCDebug(QT_KNX_TL_STM) << "Action A3!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send an N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, "
        "destination = connection_address, sequence = sequence of received message to the Network "
        "Layer (remote device).";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Restart the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA4()
{
    qCDebug(QT_KNX_TL_STM) << "Action A4!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send an N_Data_Individual.req with T_NAK_PDU, priority = SYSTEM, "
        "destination = connection_address, sequence = sequence of received message to the Network "
        "Layer (remote device).";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Restart the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA5()
{
    qCDebug(QT_KNX_TL_STM) << "Action A5!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a T_Disconnect.ind to the user.";
    acknowledgmentTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop acknowledgment timeout timer.";
    connectionTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA6()
{
    qCDebug(QT_KNX_TL_STM) << "Action A6!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = "
        "SYSTEM, destination = connection_address, sequence = 0 to the Network Layer (remote device).";
    qCDebug(QT_KNX_TL_STM_LOW) << "Send a T_Disconnect.ind to the user.";
    acknowledgmentTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop acknowledgment timeout timer.";
    connectionTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA7()
{
    qCDebug(QT_KNX_TL_STM) << "Action A7!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Store the received T_Data_Connected.req and send as a "
        "N_Data_Individual.req with T_DATA_CONNECTED_REQ_PDU, destination = connection_address, "
        "sequence = SeqNoSend to the Network Layer (remote device).";
    repCount = 0; qCDebug(QT_KNX_TL_STM_LOW) << "Clear the rep_count.";
    acknowledgmentTimeoutTimer.start();  qCDebug(QT_KNX_TL_STM_LOW) << "Start the acknowledge timeout timer.";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Restart the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA8()
{
    qCDebug(QT_KNX_TL_STM) << "Action A8!";

    acknowledgmentTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop the acknowledge timeout timer.";
    ++seqNoSend &= 0x0f; qCDebug(QT_KNX_TL_STM_LOW) << "Increment the SeqNoSend.";
    qCDebug(QT_KNX_TL_STM_LOW) << "Send the stored buffer as a T_Data_Connected.con with cleared errorbits, "
        "connection number = 0 to the user.";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Restart the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA9()
{
    qCDebug(QT_KNX_TL_STM) << "Action A9!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send the stored message as a N_Data_Individual.req to the Network Layer "
        "(remote device).";
    repCount++; qCDebug(QT_KNX_TL_STM_LOW) << "Increment the rep_count.";
    acknowledgmentTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Start the acknowledge timeout timer.";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Restart the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA10()
{
    qCDebug(QT_KNX_TL_STM) << "Action A10!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU Priority = SYSTEM, "
        "Destination = source (rbuffer), Sequence = 0 back to sender.";
}

void QKnxTransportLayerStateMachine::doActionA11()
{
    qCDebug(QT_KNX_TL_STM) << "Action A11!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Store event back and handle after next event. Don't change order of "
        "T_Data_Connected.req events.";
}

void QKnxTransportLayerStateMachine::doActionA12()
{
    qCDebug(QT_KNX_TL_STM) << "Action A12!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Connection_address=address from T_CONNECT_requ.";
    qCDebug(QT_KNX_TL_STM_LOW) << "Send N_Data_Individual.req with T_CONNECT_REQ_PDU.";
    seqNoSend = 0; seqNoRcv = 0; qCDebug(QT_KNX_TL_STM_LOW) << "seqNoSend = 0; seqNoRcv = 0.";
    connectionTimeoutTimer.start(); qCDebug(QT_KNX_TL_STM_LOW) << "Start connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA13()
{
    qCDebug(QT_KNX_TL_STM) << "Action A13!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a T_Connect.con to the user.";
}

void QKnxTransportLayerStateMachine::doActionA14()
{
    qCDebug(QT_KNX_TL_STM) << "Action A14!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, "
        "destination = connection_address, sequence = 0 to the Network Layer (remote device).";
    qCDebug(QT_KNX_TL_STM_LOW) << "Send a T_Disconnect.con to the user.";
    acknowledgmentTimeoutTimer.stop();  qCDebug(QT_KNX_TL_STM_LOW) << "Stop the acknowledge timeout timer.";
    connectionTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop the connection timeout timer.";
}

void QKnxTransportLayerStateMachine::doActionA15()
{
    qCDebug(QT_KNX_TL_STM) << "Action A15!";

    qCDebug(QT_KNX_TL_STM_LOW) << "Send a T_Disconnect.con to the management user.";
    acknowledgmentTimeoutTimer.stop();  qCDebug(QT_KNX_TL_STM_LOW) << "Stop the acknowledge timeout timer.";
    connectionTimeoutTimer.stop(); qCDebug(QT_KNX_TL_STM_LOW) << "Stop the connection timeout timer.";
}

QT_END_NAMESPACE
