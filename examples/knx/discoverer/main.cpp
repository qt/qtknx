/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/QDebug>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtKnx/QKnxNetIpServerDiscoveryAgent>
#include <QtNetwork/QNetworkInterface>

static QString familieToString(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId id)
{
    switch (id) {
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core:
            return "Core";
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement:
            return "Device Management";
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling:
            return "Tunnel";
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting:
            return "Routing";
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging:
            return "Remote Logging";
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis:
            return "Remote Configuration";
        case QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer:
            return "Object Server";
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
    return QString(address == QHostAddress(QKnxNetIp::MulticastAddress) ? "Multicast" : "Unknown");
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

            auto services = server.supportedServices();
            qInfo().noquote() << QString::fromLatin1("    Supported services:");
            for (auto it = services.constBegin(); it != services.constEnd(); ++it) {
                qInfo().noquote() << QString::fromLatin1("      KNXnet/IP %1, Version: %2")
                    .arg(familieToString(it.key())).arg(it.value());
            }
            qInfo() << "";
        }
    } else {
        qInfo().noquote() << "No server(s) found on the network.";
    }

    return 0;
}
