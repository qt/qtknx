/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Knx module.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QDebug>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtKnx/QKnxByteArray>
#include <QtKnx/QKnxNetIpTunnelConnection>
#include <QtNetwork/QNetworkInterface>

#ifdef Q_OS_WIN
# include <QtCore/QWinEventNotifier>
# include <windows.h>
#else
# include <QtCore/QSocketNotifier>
# include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions({
        { { "t", "timeout" }, "The heartbeat timeout in seconds.", "timeout", "60" },
        { { "n", "nat" }, "Use Network Address Translation to traverse across network routers." },
        { "localAddress", "The local IP address used for the control endpoint.", "localAddress",
            "127.0.0.1" },
        { "localPort", "The local UDP port used for the control endpoint.", "localPort", "0" },
        { "remoteAddress", "The remote IP address used by the server the control endpoint.",
            "remoteAddress" },
        { "remotePort", "The remote UDP port used by the server the control endpoint.",
            "remotePort" }
    });
    parser.process(app);

    QKnxNetIpTunnelConnection tunnel;
    tunnel.setNatAware(parser.isSet("nat"));
    tunnel.setLocalPort(parser.value("localPort").toUInt());
    tunnel.setHeartbeatTimeout(parser.value("timeout").toInt() * 1000);
    tunnel.setLocalAddress(QHostAddress(parser.value("localAddress")));

    QObject::connect(&tunnel, &QKnxNetIpTunnelConnection::disconnected, &app, &QCoreApplication::quit);

    tunnel.connectToHost(QHostAddress(parser.value("remoteAddress")),
        parser.value("remotePort").toUInt());

    QTextStream input(stdin, QIODevice::ReadOnly);
#ifdef Q_OS_WIN
    QWinEventNotifier notifier(GetStdHandle(STD_INPUT_HANDLE));
    QObject::connect(&notifier, &QWinEventNotifier::activated, [&](HANDLE) {
#else
    QSocketNotifier notifier(STDIN_FILENO, QSocketNotifier::Read);
    QObject::connect(&notifier, &QSocketNotifier::activated, [&](int) {
#endif
        auto tmp = input.readLine().toLatin1();
        if (tmp != "quit") {
            const auto bytes = QKnxByteArray::fromHex(tmp);
            tunnel.sendTunnelFrame(QKnxLinkLayerFrame::fromBytes(bytes, 0, bytes.size()));
        } else {
            tunnel.disconnectFromHost();
        }
    });

    if (tunnel.error() == QKnxNetIpTunnelConnection::Error::None) {
        qInfo().noquote() << "Type 'quit' to stop the application.";
        app.exec();
    }

    return 0;
}
