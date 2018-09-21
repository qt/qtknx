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

#ifndef QKNXNETIPTESTROUTER_P_H
#define QKNXNETIPTESTROUTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt KNX API.  It exists for the convenience
// of the Qt KNX implementation.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qobject.h>

#include <QtNetwork/qudpsocket.h>
#include <QtNetwork/qhostaddress.h>

#include <QtKnx/private/qknxnetiprouter_p.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpRouterPrivate;
class Q_AUTOTEST_EXPORT QKnxNetIpTestRouter
{
public:
    static QKnxNetIpTestRouter *instance()
    {
        static QKnxNetIpTestRouter routerInterface;
        return &routerInterface;
    }

    void emitReadyRead(bool readAllPackets = true)
    {
        Q_ASSERT(m_router);
        auto socket = m_router->m_socket;
        Q_ASSERT(socket);

        // By default the QKnxNetIpRouter gets the IP of the interface. A dummy
        // address is assigned to trick the router into not discarding packets that
        // have the same address as the interface used by the QKnxNetIpRouter
        if (readAllPackets)
            m_router->m_ownAddress = QHostAddress(16843010); //  setting ip 1.1.1.2
        socket->waitForReadyRead(1);
    }

    QKnxNetIpRouterPrivate *routerInstance() { return m_router; }
    void setRouterInstance(QKnxNetIpRouterPrivate *router) { m_router = router; }

private:
    QKnxNetIpTestRouter() = default;
    QKnxNetIpRouterPrivate *m_router { nullptr };
};

QT_END_NAMESPACE

#endif
