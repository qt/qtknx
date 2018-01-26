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

#ifndef QKNXTRANSPORTLAYERSTATE_H
#define QKNXTRANSPORTLAYERSTATE_H

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

#include <QtCore/qmetatype.h>
#include <QtCore/qstring.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class QKnxTransportLayerStateMachine;
class Q_KNX_EXPORT QKnxTransportLayerState
{
public:
    enum class State : quint8
    {
        Closed,
        Connecting,
        OpenIdle,
        OpenWait
    };

    QKnxTransportLayerState(QKnxTransportLayerStateMachine *stateMachine);
    virtual ~QKnxTransportLayerState() = 0;

    virtual void event00() {}
    virtual void event01() {}
    virtual void event02() {}
    virtual void event03() {}
    virtual void event04() {}
    virtual void event05() {}
    virtual void event06() {}
    virtual void event07() {}
    virtual void event08() {}
    virtual void event09() {}
    virtual void event10() {}
    virtual void event11() {}
    virtual void event12() {}
    virtual void event13() {}
    virtual void event14() {}
    virtual void event15() {}
    virtual void event16() {}
    virtual void event17() {}
    virtual void event18() {}
    virtual void event19() {}
    virtual void event20() {}
    virtual void event21() {}
    virtual void event22() {}
    virtual void event23() {}
    virtual void event24() {}
    virtual void event25() {}
    virtual void event26() {}
    virtual void event27() {}

    virtual QString name() const = 0;
    virtual QKnxTransportLayerState::State state() const = 0;

protected:
    QKnxTransportLayerStateMachine *sm { nullptr };
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxTransportLayerState::State)

#endif
