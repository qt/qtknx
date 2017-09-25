/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#ifndef QKNXNETIPSERVERDESCRIPTIONAGENT_P_H
#define QKNXNETIPSERVERDESCRIPTIONAGENT_P_H

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

#include <QtCore/qtimer.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipdescriptionrequest.h>
#include <QtKnx/qknxnetipdescriptionresponse.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipserverdescriptionagent.h>
#include <QtKnx/qknxnetipserverinfo.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkdatagram.h>
#include <QtNetwork/qudpsocket.h>

#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpServerDescriptionAgentPrivate final : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpServerDescriptionAgent)

public:
    QKnxNetIpServerDescriptionAgentPrivate(const QHostAddress &addr, quint16 prt);
    ~QKnxNetIpServerDescriptionAgentPrivate() override = default;

    void setupSocket();
    void setupAndStartReceiveTimer();

    void setAndEmitStateChanged(QKnxNetIpServerDescriptionAgent::State newState);
    void setAndEmitServerDescriptionReceived(const QKnxNetIpServerInfo &discoveryInfo);
    void setAndEmitErrorOccurred(QKnxNetIpServerDescriptionAgent::Error e, const QString &message);

private:
    QUdpSocket *socket { nullptr };
    QTimer *receiveTimer { nullptr };

    quint16 port { 0 }, usedPort;
    QHostAddress address { QHostAddress::AnyIPv4 };

    quint8 ttl { 60 };
    bool nat { false };
    int timeout { QKnxNetIp::Timeout::DescriptionTimeout };

    QString errorString;
    QKnxNetIpHpai m_server;
    QKnxNetIpServerInfo m_description;

    QKnxNetIpServerDescriptionAgent::Error error { QKnxNetIpServerDescriptionAgent::Error::None };
    QKnxNetIpServerDescriptionAgent::State state { QKnxNetIpServerDescriptionAgent::State::NotRunning };
};

QT_END_NAMESPACE

#endif
