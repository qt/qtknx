/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtTest/qsignalspy.h>
#include <QtTest/qtest.h>

#include <QtKnx/private/qknxtransportlayerstatemachine_p.h>

class tst_QKnxTransportLayerStateMachine : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QLoggingCategory::setFilterRules(QStringLiteral("qt.qknxtransportlayerstatemachine = true"));
    }

    // 08_03_04, paragraph 4.2

    void testAcknowledgmentTimeoutTimer()
    {
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
        QKnxTransportLayerStateMachine machine;
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::Closed);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event01)");
        QTest::ignoreMessage(QtDebugMsg, "Action A1!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open idle state.\"");
        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event01);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event15)");
        QTest::ignoreMessage(QtDebugMsg, "Action A7!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open wait state.\"");
        QTest::ignoreMessage(QtDebugMsg, "Acknowledgment timeout.");
        QTest::ignoreMessage(QtDebugMsg, "Repetition count: 0");
        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event17)");
        QTest::ignoreMessage(QtDebugMsg, "Action A9!");
        QTest::ignoreMessage(QtDebugMsg, "Acknowledgment timeout.");
        QTest::ignoreMessage(QtDebugMsg, "Repetition count: 1");
        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event17)");
        QTest::ignoreMessage(QtDebugMsg, "Action A9!");
        QTest::ignoreMessage(QtDebugMsg, "Acknowledgment timeout.");
        QTest::ignoreMessage(QtDebugMsg, "Repetition count: 2");
        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event17)");
        QTest::ignoreMessage(QtDebugMsg, "Action A9!");
        QTest::ignoreMessage(QtDebugMsg, "Acknowledgment timeout.");
        QTest::ignoreMessage(QtDebugMsg, "Repetition count: 3");
        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event18)");
        QTest::ignoreMessage(QtDebugMsg, "Action A6!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event15);

        QSignalSpy(&machine, SIGNAL(closed())).wait(20000);
    }

    // 08_03_04, paragraph 5.2.1

    void testConnectFromRemoteDeviceSequence1()
    {
        {
            QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
            QKnxTransportLayerStateMachine machine;
            QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::Closed);

            QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event00)");
            QTest::ignoreMessage(QtDebugMsg, "Action A1!");
            QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open idle state.\"");

            QSignalSpy idleSpy(&machine, &QKnxTransportLayerStateMachine::openIdle);
            QSignalSpy stateSpy(&machine, &QKnxTransportLayerStateMachine::stateChanged);

            machine.processEvent(QKnxTransportLayerStateMachine::Event::Event00);
            QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

            QCOMPARE(idleSpy.count(), 1);
            QCOMPARE(stateSpy[0][0].value<QKnxTransportLayerState::State>(),
                QKnxTransportLayerState::State::OpenIdle);
        }

        {
            QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
            QKnxTransportLayerStateMachine machine;
            QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::Closed);

            QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event01)");
            QTest::ignoreMessage(QtDebugMsg, "Action A1!");
            QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open idle state.\"");

            QSignalSpy idleSpy(&machine, &QKnxTransportLayerStateMachine::openIdle);
            QSignalSpy stateSpy(&machine, &QKnxTransportLayerStateMachine::stateChanged);

            machine.processEvent(QKnxTransportLayerStateMachine::Event::Event01);
            QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

            QCOMPARE(idleSpy.count(), 1);
            QCOMPARE(stateSpy.count(), 1);
            QCOMPARE(stateSpy[0][0].value<QKnxTransportLayerState::State>(),
                QKnxTransportLayerState::State::OpenIdle);
        }
    }

    void testConnectFromRemoteDeviceSequence2()
    {
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
        QKnxTransportLayerStateMachine machine;
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::Closed);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event00)");
        QTest::ignoreMessage(QtDebugMsg, "Action A1!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open idle state.\"");

        QSignalSpy idleSpy(&machine, &QKnxTransportLayerStateMachine::openIdle);
        QSignalSpy stateSpy(&machine, &QKnxTransportLayerStateMachine::stateChanged);

        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event00);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(stateSpy.count(), 1);
        QCOMPARE(stateSpy[0][0].value<QKnxTransportLayerState::State>(),
            QKnxTransportLayerState::State::OpenIdle);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event00)");
        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event00);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(stateSpy.count(), 1);
    }

    void testConnectFromRemoteDeviceSequence3()
    {
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
        QKnxTransportLayerStateMachine machine;
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::Closed);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event00)");
        QTest::ignoreMessage(QtDebugMsg, "Action A1!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open idle state.\"");

        QSignalSpy idleSpy(&machine, &QKnxTransportLayerStateMachine::openIdle);
        QSignalSpy stateSpy(&machine, &QKnxTransportLayerStateMachine::stateChanged);

        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event00);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(stateSpy.count(), 1);
        QCOMPARE(stateSpy[0][0].value<QKnxTransportLayerState::State>(),
            QKnxTransportLayerState::State::OpenIdle);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event15)");
        QTest::ignoreMessage(QtDebugMsg, "Action A7!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open wait state.\"");

        QSignalSpy waitSpay(&machine, &QKnxTransportLayerStateMachine::openWait);

        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event15);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenWait);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(waitSpay.count(), 1);
        QCOMPARE(stateSpy.count(), 2);
        QCOMPARE(stateSpy[1][0].value<QKnxTransportLayerState::State>(),
            QKnxTransportLayerState::State::OpenWait);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event00)");
        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event00);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenWait);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(waitSpay.count(), 1);
        QCOMPARE(stateSpy.count(), 2);
    }

    // 08_03_04, paragraph 5.2.2

    void testConnectFromRemoteDeviceExistingConnectionSequence4()
    {
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Closed state.\"");
        QKnxTransportLayerStateMachine machine;
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::Closed);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event00)");
        QTest::ignoreMessage(QtDebugMsg, "Action A1!");
        QTest::ignoreMessage(QtDebugMsg, "Active state changed to: \"Open idle state.\"");

        QSignalSpy idleSpy(&machine, &QKnxTransportLayerStateMachine::openIdle);
        QSignalSpy stateSpy(&machine, &QKnxTransportLayerStateMachine::stateChanged);

        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event00);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(stateSpy.count(), 1);
        QCOMPARE(stateSpy[0][0].value<QKnxTransportLayerState::State>(),
            QKnxTransportLayerState::State::OpenIdle);

        QTest::ignoreMessage(QtDebugMsg, "Process event: QKnxTransportLayerStateMachine::Event(Event01)");
        QTest::ignoreMessage(QtDebugMsg, "Action A10!");

        machine.processEvent(QKnxTransportLayerStateMachine::Event::Event01);
        QCOMPARE(machine.activeState()->state(), QKnxTransportLayerState::State::OpenIdle);

        QCOMPARE(idleSpy.count(), 1);
        QCOMPARE(stateSpy.count(), 1);
    }

    // TODO: add more test based on 08_03_04 Transport Layer Tests v01 05 00 AS
};

QTEST_MAIN(tst_QKnxTransportLayerStateMachine)

#include "tst_qknxtransportlayerstatemachine.moc"
