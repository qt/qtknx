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

#ifndef QKNXTRANSPORTLAYERCONNECTINGSTATE_P_H
#define QKNXTRANSPORTLAYERCONNECTINGSTATE_P_H

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

#include <QtKnx/private/qknxtransportlayerstate_p.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxTransportLayerConnectingState : public QKnxTransportLayerState
{
public:
    using QKnxTransportLayerState::QKnxTransportLayerState;
    ~QKnxTransportLayerConnectingState()  override = default;

    void event01() override final;
    void event02() override final;
    void event04() override final;
    void event05() override final;
    void event06() override final;
    void event07() override final;
    void event08() override final;
    void event09() override final;
    void event10() override final;
    void event11() override final;
    void event12() override final;
    void event13() override final;
    void event14() override final;
    void event15() override final;
    void event16() override final;
    void event19() override final;
    void event20() override final;
    void event25() override final;
    void event26() override final;

    QString name() const override final;
    QKnxTransportLayerState::State state() const override final;
};

QT_END_NAMESPACE

#endif
