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
#include <QtKnx/QKnxNetIpServerDiscoveryAgent>
#include <QtNetwork/QNetworkInterface>

static QString familieToString(QKnxNetIp::ServiceFamily id)
{
    switch (id) {
        case QKnxNetIp::ServiceFamily::Core:
            return "Core";
        case QKnxNetIp::ServiceFamily::DeviceManagement:
            return "Device Management";
        case QKnxNetIp::ServiceFamily::IpTunneling:
            return "Tunnel";
        case QKnxNetIp::ServiceFamily::IpRouting:
            return "Routing";
        case QKnxNetIp::ServiceFamily::RemoteLogging:
            return "Remote Logging";
        case QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis:
            return "Remote Configuration";
        case QKnxNetIp::ServiceFamily::ObjectServer:
            return "Object Server";
        case QKnxNetIp::ServiceFamily::Security:
            return "Security";
        default:
            break;
    }
    return "Unknown";
}

QString interfaceFromAddress(const QHostAddress &address)
{
    auto interfaces = QNetworkInterface::allInterfaces();
    for (const auto &interface : qAsConst(interfaces)) {
        if (interface.allAddresses().contains(address))
            return interface.humanReadableName();
    }
    return QString(address == QHostAddress(QKnxNetIp::Constants::MulticastAddress)
        ? "Multicast" : "Unknown");
}

int main(int argc, char *argv[])
{
    QCoreApplication discoverer(argc, argv);
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions({
        { { "t", "timeout" }, "Discovery response timeout in seconds.", "timeout", "3" },
        { { "n", "nat" }, "Use Network Address Translation to traverse across network routers." },
        { { "u", "unicast" }, "Force unicast response. (defaults to multicast response)" },
        { { "a", "localAddress" }, "The local IP address a response shall be sent to. "
             "Implies <unicast>", "localAddress", "127.0.0.1" },
        { { "p", "localPort" }, "The local UDP port a response shall be sent to (defaults to "
            "system assigned). Implies <unicast>", "localPort", "0"}
    });
    parser.process(discoverer);

    QKnxNetIpServerDiscoveryAgent agent;
    agent.setNatAware(parser.isSet("nat"));
    agent.setLocalPort(parser.value("localPort").toUInt());
    agent.setTimeout(parser.value("timeout").toInt() * 1000);
    agent.setLocalAddress(QHostAddress(parser.value("localAddress")));

    if (parser.isSet("unicast"))
        agent.setResponseType(QKnxNetIpServerDiscoveryAgent::ResponseType::Unicast);

    QObject::connect(&agent, &QKnxNetIpServerDiscoveryAgent::finished, &discoverer,
        &QCoreApplication::quit);

    agent.start();

    if (agent.error() == QKnxNetIpServerDiscoveryAgent::Error::None)
        discoverer.exec();

    qInfo().noquote() << endl << "Device used to send the search request:";
    qInfo().noquote() << QString::fromLatin1("  Network interface: %1, address: %2, port: %3")
        .arg(interfaceFromAddress(agent.localAddress()), agent.localAddress().toString())
        .arg(agent.localPort());

    const auto servers = agent.discoveredServers();
    if (servers.size() > 0) {
        qInfo().noquote() << endl << QString::fromLatin1("%1 server(s) found on the network.")
            .arg(servers.size());
        for (auto server : servers) {
            qInfo().noquote() << QString::fromLatin1("  Server: %1").arg(server.deviceName());
            qInfo().noquote() << QString::fromLatin1("      Individual address: %1").arg(server
                .individualAddress().toString());
            qInfo().noquote() << QString::fromLatin1("      Server control endpoint: %1:%2")
                .arg(server.controlEndpointAddress().toString()).arg(server.controlEndpointPort());

            const auto services = server.supportedServices();
            qInfo().noquote() << QString::fromLatin1("    Supported services:");
            for (const auto service : services) {
                qInfo().noquote() << QString::fromLatin1("      KNXnet/IP %1, Version: %2")
                    .arg(familieToString(service.ServiceFamily)).arg(service.ServiceFamilyVersion);
            }
            qInfo() << "";
        }
    } else {
        qInfo().noquote() << "No server(s) found on the network.";
    }

    return 0;
}
