/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qcommandlineparser.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdatetime.h>

#include <QtKnx/qknxnetiproutinginterface.h>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxnetiproutinginterface.h>
#include <QtKnx/qknxnetiproutingbusy.h>
#include <QtKnx/qknxnetiproutingindication.h>
#include <QtKnx/qknxnetiproutinglostmessage.h>

#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkinterface.h>
#include <QtNetwork/qnetworkdatagram.h>
#include <QtNetwork/qudpsocket.h>


#ifdef Q_OS_WIN
# include <QtCore/QWinEventNotifier>
# include <windows.h>
#else
# include <QtCore/QSocketNotifier>
# include <unistd.h>
#endif

void startRouter(QKnxNetIpRoutingInterface &router, const QCommandLineParser &cliParser)
{
    router.start();

    if (router.error() != QKnxNetIpRoutingInterface::Error::None)
        return;

    qInfo().noquote() << "Network interface used: "
                      << router.interfaceAffinity().name() << "("
                      << router.interfaceAffinity().addressEntries().first().ip()
                      << ")";
    qInfo().noquote() << "Multicast address: "
                      << router.multicastAddress();
    if (cliParser.isSet("receiver")) {
        qInfo().noquote() << "Working as a receiver of KNX Routing messages.";
        return;
    }

    qInfo().noquote() << "Type 'quit' to stop the application.";
    if (cliParser.isSet("indication")) {
        qInfo().noquote() << "Type the hexadecimal content of the indication "
                             "and hit enter. Setting no content by default "
                             "uses 1100b4e000000002010000.";
    } else {
        qInfo().noquote() << "Hit enter for sending the message";
    }
}

void setupRouterCLI(QKnxNetIpRoutingInterface &router,
                    const QCommandLineParser &cliParser,
                    QTextStream & input,
#ifdef Q_OS_WIN
                    QWinEventNotifier &notifier)
{
    QObject::connect(&notifier, &QWinEventNotifier::activated, [&](HANDLE) {
#else
                    QSocketNotifier &notifier)
{
    QObject::connect(&notifier, &QSocketNotifier::activated, [&](int) {
#endif
        auto tmp = input.readLine().toLatin1();
        if (tmp == "quit") {
            router.stop();
            return;
        }

        if (cliParser.isSet("indication")) {
            static const auto bytes = QKnxByteArray::fromHex("1100b4e000000002010000");
            auto frame = QKnxLinkLayerFrame::builder()
                .setData(tmp.isEmpty() ? bytes : QKnxByteArray::fromHex(tmp))
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();
            router.sendRoutingIndication(frame);
        } else if (cliParser.isSet("busy")) {
            auto routingBusyFrame = QKnxNetIpRoutingBusyProxy::builder()
                .setDeviceState(QKnxNetIp::DeviceState::IpFault)
                .setRoutingBusyWaitTime(cliParser.value("BusyWaitTime").toUInt())
                .setRoutingBusyControl(cliParser.value("BusyControlField").toUInt())
                .create();
            router.sendRoutingBusy(routingBusyFrame);
        } else if (cliParser.isSet("lost")) {
            auto routingLostFrame = QKnxNetIpRoutingLostMessageProxy::builder()
                .setDeviceState(QKnxNetIp::DeviceState::IpFault)
                .setLostMessageCount(0xffff)
                .create();
            router.sendRoutingLostMessage(routingLostFrame);
        }
    });
}

void setupRouterSignalHandlers(QKnxNetIpRoutingInterface &router, const QNetworkInterface &iface,
                 const QHostAddress &multicastAddress)
{
    router.setInterfaceAffinity(iface);
    Q_ASSERT(router.interfaceAffinity().index() == iface.index());
    router.setMulticastAddress(multicastAddress);

    QObject::connect(&router, &QKnxNetIpRoutingInterface::stateChanged,
                     [&](QKnxNetIpRoutingInterface::State state) {
        if (state == QKnxNetIpRoutingInterface::State::Routing)
            qInfo().noquote()  << QTime::currentTime().toString()
                               << ": Router is in Routing state.";
        else if (state == QKnxNetIpRoutingInterface::State::Failure)
            qInfo().noquote()  << QTime::currentTime().toString()
                               << ": Error: " << router.errorString();
        else if (state == QKnxNetIpRoutingInterface::State::NeighborBusy)
            qInfo().noquote() << QTime::currentTime().toString()
                              << ": Neighbor router is busy. Busy timer started";
        else if (state == QKnxNetIpRoutingInterface::State::Stop)
            QCoreApplication::quit();
    });

    QObject::connect(&router, &QKnxNetIpRoutingInterface::routingIndicationReceived,
                     [](QKnxNetIpFrame frame) {
        QKnxNetIpRoutingIndicationProxy indication(frame);
        qInfo().noquote() << "Received routing indication"
                          << indication.isValid();
    });

    QObject::connect(&router, &QKnxNetIpRoutingInterface::routingBusyReceived,
                     [](QKnxNetIpFrame frame) {
        QKnxNetIpRoutingBusyProxy busy(frame);
        qInfo().noquote() << "Received routing busy, wait time: "
                          << busy.routingBusyWaitTime() << " busy control field: "
                          << busy.routingBusyControl();
    });

    QObject::connect(&router, &QKnxNetIpRoutingInterface::routingLostCountReceived,
                     [](QKnxNetIpFrame frame) {
        QKnxNetIpRoutingLostMessageProxy lost(frame);
        qInfo().noquote() << "Received routing lost count"
                          << lost.deviceState();
    });

    QObject::connect(&router, &QKnxNetIpRoutingInterface::routingIndicationSent,
                     [](QKnxNetIpFrame frame) {
        qInfo().noquote() << "Sent routing indication";
        Q_UNUSED(frame);
    });

    QObject::connect(&router, &QKnxNetIpRoutingInterface::routingBusySent,
                     [](QKnxNetIpFrame frame) {
        qInfo().noquote() << "Sent routing busy";
        Q_UNUSED(frame);
    });

    QObject::connect(&router, &QKnxNetIpRoutingInterface::routingLostCountSent,
                     [](QKnxNetIpFrame frame) {
        qInfo().noquote() << "Sent routing lost count";
        Q_UNUSED(frame);
    });
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QCommandLineParser cliParser;
    cliParser.addHelpOption();

    cliParser.addOptions({
        { { "n", "interface" }, "Set the network interface name that "
        "the KNXnet/Ip routing interface will use.", "InterfaceName", "eth0" },
        { { "m", "multicastAddress" }, "Sets the multicast address that "
        "the KNXnet/Ip routing interface will use for sending routing messages.",
        "multicastAddress", "224.0.23.12" },
        { { "i", "indication" }, "Sends routing indication messages" },
        { { "b", "busy" }, "Sends routing busy messages" },
        { "busyControlField", "Sets the busy control field of busy messages.",
        "busyControlField", "0" },
        { "busyWaitTime", "Sets the wait time for busy messages.",
        "busyWaitTime", "10000" },
        { { "l", "lost" }, "Sends routing lost messages" },
        { { "r", "receiver" }, "Only work as a KNXnet/IP routing message receiver" },
    });
    cliParser.process(a);

    QNetworkInterface iface { QNetworkInterface::interfaceFromName(cliParser.value("interface")) };
    if (!iface.isValid()) {
        auto interfaces = iface.allInterfaces();
        QList<QNetworkInterface>::iterator found = ( std::find_if(interfaces.begin(),
                             interfaces.end(),
                             [](const QNetworkInterface &i){
                                return i.type() == QNetworkInterface::Ethernet;
        }));
        if (found ==  iface.allInterfaces().end()) {
            qInfo().noquote() << "No valid network interface given and no "
                                 "Ethernet adapter found. ";
            cliParser.showHelp();
        }
        iface = static_cast<QNetworkInterface>(*found);
    }

    QHostAddress multicastAddress = QHostAddress(cliParser.value("multicastAddress"));
    if (!multicastAddress.isMulticast()) {
        qInfo().noquote() << "Multicast address invalid.";
        cliParser.showHelp();
    }
    if (!cliParser.isSet("indication")
        && !cliParser.isSet("busy")
        && !cliParser.isSet("lost")
        && !cliParser.isSet("receiver")) {
        qInfo().noquote() << "Missing parameter that specifies the types of "
                             "messages to send.";
        cliParser.showHelp();
    }

    QKnxNetIpRoutingInterface router;
    setupRouterSignalHandlers(router, iface, multicastAddress);

    QTextStream input(stdin, QIODevice::ReadOnly);
#ifdef Q_OS_WIN
    QWinEventNotifier notifier(GetStdHandle(STD_INPUT_HANDLE));
#else
    QSocketNotifier notifier(STDIN_FILENO, QSocketNotifier::Read);
#endif
    if (!cliParser.isSet("receiver")) {
        setupRouterCLI(router, cliParser, input, notifier);
    }
    startRouter(router, cliParser);
    return a.exec();
}
