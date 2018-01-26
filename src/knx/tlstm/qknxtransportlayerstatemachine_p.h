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

#ifndef QKNXTRANSPORTLAYERSTATEMACHINE_P_H
#define QKNXTRANSPORTLAYERSTATEMACHINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qloggingcategory.h>
#include <QtCore/qobject.h>
#include <QtCore/qtimer.h>

#include <QtKnx/private/qknxtransportlayerstate_p.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_KNX_TL_STM)
Q_DECLARE_LOGGING_CATEGORY(QT_KNX_TL_STM_LOW)

class Q_KNX_EXPORT QKnxTransportLayerStateMachine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxTransportLayerStateMachine)

    friend class QKnxTransportLayerConnectingState;
    friend class QKnxTransportLayerClosedState;
    friend class QKnxTransportLayerOpenIdleState;
    friend class QKnxTransportLayerOpenWaitState;

public:
    enum class Event : quint8
    {
        Event00, Event01, Event02, Event03, Event04, Event05, Event06, Event07,
        Event08, Event09, Event10, Event11, Event12, Event13, Event14, Event15,
        Event16, Event17, Event18, Event19, Event20, Event21, Event22, Event23,
        Event24, Event25, Event26, Event27
    };
    Q_ENUM(Event)

    QKnxTransportLayerStateMachine(QObject *parent = nullptr);
    ~QKnxTransportLayerStateMachine();

    QKnxTransportLayerState *activeState() const;
    void setActiveState(QKnxTransportLayerState *activeState);

public Q_SLOTS:
    void processEvent(QKnxTransportLayerStateMachine::Event event);

Q_SIGNALS:
    void closed();
    void connecting();
    void openIdle();
    void openWait();

    void stateChanged(QKnxTransportLayerState::State state);

private:
    void doActionA1();
    void doActionA2();
    void doActionA3();
    void doActionA4();
    void doActionA5();
    void doActionA6();
    void doActionA7();
    void doActionA8();
    void doActionA9();
    void doActionA10();
    void doActionA11();
    void doActionA12();
    void doActionA13();
    void doActionA14();
    void doActionA15();

private:
    QTimer connectionTimeoutTimer;
    QTimer acknowledgmentTimeoutTimer;

    quint64 repCount { 0 };
    quint8 seqNoSend { 0 }, seqNoRcv { 0 };

    QKnxTransportLayerState *state { nullptr };
};

QT_END_NAMESPACE

#endif
