/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/QDebug>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtKnx/QKnxNetIpTunnelClient>
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

    QKnxNetIpTunnelClient client;
    client.setNatAware(parser.isSet("nat"));
    client.setLocalPort(parser.value("localPort").toUInt());
    client.setHeartbeatTimeout(parser.value("timeout").toInt() * 1000);
    client.setLocalAddress(QHostAddress(parser.value("localAddress")));

    QObject::connect(&client, &QKnxNetIpClient::disconnected, &app, &QCoreApplication::quit);

    client.connectToHost(QHostAddress(parser.value("remoteAddress")),
        parser.value("remotePort").toUInt());

    QTextStream input(stdin, QIODevice::ReadOnly);
#ifdef Q_OS_WIN
    QWinEventNotifier notifier(GetStdHandle(STD_INPUT_HANDLE));
    QObject::connect(&notifier, &QWinEventNotifier::activated, [&](HANDLE) {
#else
    QSocketNotifier notifier(STDIN_FILENO, QSocketNotifier::Read);
    QObject::connect(&notifier, &QSocketNotifier::activated, [&](int) {
#endif
        auto bytes = input.readLine().toLatin1();
        if (bytes != "quit") {
            bytes = QByteArray::fromHex(bytes);
            client.sendTunnelingFrame(QKnxCemiFrame::fromBytes(bytes, 0, bytes.size()));
        } else {
            client.disconnectFromHost();
        }
    });

    if (client.error() == QKnxNetIpTunnelClient::Error::None) {
        qInfo().noquote() << "Type 'quit' to stop the application.";
        app.exec();
    }

    return 0;
}
