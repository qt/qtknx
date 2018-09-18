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

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxnetiprouter.h>
#include <QtKnx/qknxnetiproutingbusy.h>
#include <QtKnx/qknxnetiproutingindication.h>
#include <QtKnx/qknxnetiproutinglostmessage.h>
#include <QtKnx/qknxnetiproutingsystembroadcast.h>

#include <QtKnx/private/qknxnetiprouter_p.h>
#include <QtKnx/private/qknxnetiptestrouter_p.h>
#include <QtKnx/private/qknxtpdufactory_p.h>

#include <QtCore/qdebug.h>
#include <QtNetwork/qnetworkdatagram.h>
#include <QtNetwork/qnetworkinterface.h>
#include <QtNetwork/qudpsocket.h>
#include <QtTest>

Q_DECLARE_METATYPE(QKnxAddress)

#ifdef QT_BUILD_INTERNAL

class tst_QKnxNetIpRouter : public QObject
{
    Q_OBJECT

public:
    tst_QKnxNetIpRouter();

private slots:
    void initTestCase();
    void cleanup();
    void test_udp_sockets();
    void test_network_interface();
    void test_multicast_address();
    void test_routing();
    void test_routing_sends_indications();
    void test_routing_receives_indications();
    void test_routing_receives_busy();
    void test_routing_busy_sent_packets_same_individual_address();
    void test_routing_interface_sends_system_broadcast();
    void test_routing_interface_receives_system_broadcast();
    void test_routing_filter();
    void test_routing_filter_data();

private:
    void simulateFramesReceived(const QKnxNetIpFrame &netIpFrame, int numFrames = 1);

    bool runTests { true };
    int kMulticastPort { 3671 };

    QNetworkInterface kIface;
    QHostAddress kMulticastAddress { QString("224.0.23.12") };

    QKnxNetIpRouter m_router;
};

tst_QKnxNetIpRouter::tst_QKnxNetIpRouter()
{
    // look for one interface running and able to multicast

    const auto interfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface foundIface;
    for (const auto &iface : interfaces) {
        if (iface.flags() != QNetworkInterface::IsLoopBack)
            continue;

        if (iface.addressEntries().isEmpty())
            continue;

        if (!iface.flags().testFlag(QNetworkInterface::IsRunning)
            || !iface.flags().testFlag(QNetworkInterface::CanMulticast)) {
            continue;
        }

        kIface = iface;
    }
}

void tst_QKnxNetIpRouter::initTestCase()
{
    // don't run test case if this check fails
    runTests = kIface.flags().testFlag(QNetworkInterface::IsRunning)
        && kIface.flags().testFlag(QNetworkInterface::CanMulticast);

    if (!runTests)
        return;

    auto interfaceIndividualAddress = QKnxAddress::createIndividual(1, 1, 0);
    m_router.setIndividualAddress(interfaceIndividualAddress);
    m_router.setInterfaceAffinity(kIface);
}

void tst_QKnxNetIpRouter::cleanup()
{
    if (!runTests)
        return;

    m_router.disconnect();
    m_router.stop();
}

void tst_QKnxNetIpRouter::test_network_interface()
{
    if (!runTests)
        return;

    bool errorEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::errorOccurred,
        [&](QKnxNetIpRouter::Error error, QString errorString) {
        QCOMPARE(error, QKnxNetIpRouter::Error::Network);
        QCOMPARE(error, m_router.error());
        QCOMPARE(errorString, m_router.errorString());
        QCOMPARE(m_router.state(), QKnxNetIpRouter::State::Failure);
        errorEmitted = true;
    });
    {
        // setting invalid interface should cause error
        auto m_iface = QNetworkInterface();
        m_router.setInterfaceAffinity(m_iface);
        QVERIFY(errorEmitted);
    }
    {
        // setting invalid interface using IP address of the interface
        errorEmitted = false;
        m_router.setInterfaceAffinity(QHostAddress("2.2.2.2"));
        QVERIFY(errorEmitted);
    }
    {
        // local host address does not allow multicast
        errorEmitted = false;
        m_router.setInterfaceAffinity(QHostAddress(QHostAddress::LocalHost));
        QVERIFY(errorEmitted);
    }
    {
        // setting a valid interface no signal emitted
        errorEmitted = false;
        m_router.setInterfaceAffinity(kIface);
        QVERIFY(!errorEmitted);
        QCOMPARE(kIface.index(), m_router.interfaceAffinity().index());
    }
}

void tst_QKnxNetIpRouter::test_multicast_address()
{
    if (!runTests)
        return;

    {
        // default multicast address
        QKnxNetIpRouter m_router;
        QCOMPARE(m_router.multicastAddress(), kMulticastAddress);
    }
    {
        // setting an invalid multicast address shouldn't work
        QKnxNetIpRouter m_router;
        auto kMulticastAddress = QHostAddress(QHostAddress::LocalHost);
        m_router.setMulticastAddress(kMulticastAddress);
        QVERIFY(m_router.multicastAddress() != kMulticastAddress);
    }
    {
        // setting a valid multicast address
        QKnxNetIpRouter m_router;
        auto address = QHostAddress("224.0.55.55");
        m_router.setMulticastAddress(address);
        QCOMPARE(m_router.multicastAddress(), address);
    }
}

void tst_QKnxNetIpRouter::test_udp_sockets()
{
    if (!runTests)
        return;

    QUdpSocket receiver;
    QSignalSpy recvSpy(&receiver, SIGNAL(readyRead()));
    receiver.bind(QHostAddress(QHostAddress::AnyIPv4),
        kMulticastPort, (QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint));
    receiver.joinMulticastGroup(kMulticastAddress);
    QList<QByteArray> datagrams = QList<QByteArray>()
        << QByteArray("0123")
        << QByteArray("4567")
        << QByteArray("89ab")
        << QByteArray("cdef");

    QUdpSocket sender;
    QSignalSpy bytesspy(&sender, SIGNAL(bytesWritten(qint64)));
    for (const QByteArray &datagram : datagrams) {
        QNetworkDatagram dgram(datagram, kMulticastAddress, kMulticastPort);
        sender.writeDatagram(dgram);
    }

    QCOMPARE(bytesspy.count(), 4);

    receiver.waitForReadyRead(1);
    for (int i = 0; i < 4; ++i) {
        QCOMPARE(receiver.pendingDatagramSize(), 4);
        QCOMPARE(receiver.readDatagram(0, 0), qint64(0));
        QCOMPARE(recvSpy.count(), 1);
    }
}

void tst_QKnxNetIpRouter::simulateFramesReceived(const QKnxNetIpFrame &netIpFrame, int numFrames)
{
    if (!runTests)
        return;

    QUdpSocket * s = new QUdpSocket();

    s->bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    s->setMulticastInterface(kIface);

    for (; numFrames > 0; --numFrames) {
        QNetworkDatagram datagram(netIpFrame.bytes().toByteArray(),
            kMulticastAddress, kMulticastPort);
        s->writeDatagram(datagram);
    }

    // notify router socket to wait for any received packet
    QKnxNetIpTestRouter::instance()->emitReadyRead();

    if (s) {
        s->disconnect();
        s->deleteLater();
    }
}

void tst_QKnxNetIpRouter::test_routing()
{
    if (!runTests)
        return;

    bool stateChangedEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::stateChanged, [&](QKnxNetIpRouter::State state) {
        stateChangedEmitted = true;
        QCOMPARE(state, QKnxNetIpRouter::State::Routing);
    });

    m_router.start();
    QVERIFY(stateChangedEmitted);
}


void tst_QKnxNetIpRouter::test_routing_sends_indications()
{
    if (!runTests)
        return;

    m_router.start();

    auto bytes = QKnxByteArray::fromHex("2900b4e000000002010000");
    auto frameSent = QKnxLinkLayerFrame::builder()
        .setData(bytes)
        .setMedium(QKnx::MediumType::NetIP)
        .createFrame();

    bool indicationSentEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingIndicationSent, [&](QKnxNetIpFrame frame) {
        indicationSentEmitted = true;
        QKnxNetIpRoutingIndicationProxy indicationSent(frame);
        QVERIFY(indicationSent.isValid());
        QCOMPARE(indicationSent.linkLayerFrame().bytes(), frameSent.bytes());
    });
    m_router.sendRoutingIndication(frameSent);
    QVERIFY(indicationSentEmitted);

    bool stateChangedEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::stateChanged, [&](QKnxNetIpRouter::State state) {
        stateChangedEmitted = true;
        QCOMPARE(state, QKnxNetIpRouter::State::Stop);
    });
    m_router.stop();
    QVERIFY(stateChangedEmitted);
}

QKnxNetIpFrame dummyRoutingIndication(QKnxAddress dst, quint8 hopCount = 6)
{
    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueReadTpdu();
    auto ctrl = QKnxControlField::builder()
        .setFrameFormat(QKnxControlField::FrameFormat::Standard)
        .setBroadcast(QKnxControlField::Broadcast::Domain)
        .setPriority(QKnxControlField::Priority::Normal)
        .create();

    auto extCtrl = QKnxExtendedControlField::builder()
        .setDestinationAddressType(dst.type())
        .setHopCount(hopCount)
        .create();

    auto frame = QKnxLinkLayerFrame::builder()
        .setControlField(ctrl)
        .setExtendedControlField(extCtrl)
        .setTpdu(tpdu)
        .setDestinationAddress(dst)
        .setSourceAddress({ QKnxAddress::Type::Individual, 0 })
        .setMessageCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
        .setMedium(QKnx::MediumType::NetIP)
        .createFrame();
    return QKnxNetIpRoutingIndicationProxy::builder()
        .setLinkLayerFrame(frame)
        .create();
}

void tst_QKnxNetIpRouter::test_routing_receives_indications()
{
    if (!runTests)
        return;

    m_router.start();

    bool indicationRcvEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingIndicationReceived,
        [&](QKnxNetIpFrame frame, QKnxNetIpRouter::FilterAction routingAction) {
            indicationRcvEmitted = true;
            QKnxNetIpRoutingIndicationProxy indicationRcv(frame);
            QVERIFY(indicationRcv.isValid());
            QCOMPARE(routingAction, QKnxNetIpRouter::FilterAction::RouteDecremented);
    });
    simulateFramesReceived(dummyRoutingIndication(QKnxAddress::createIndividual(1, 1, 1)));
    QVERIFY(indicationRcvEmitted);
}

void tst_QKnxNetIpRouter::test_routing_receives_busy()
{
    if (!runTests)
        return;

    // test receiving routing busy!
    m_router.start();

    bool indicationRcvEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingBusyReceived, [&](QKnxNetIpFrame frame) {
        indicationRcvEmitted = true;
        QKnxNetIpRoutingBusyProxy busy(frame);
        QVERIFY(busy.isValid());
    });

    auto routingBusyFrame = QKnxNetIpRoutingBusyProxy::builder()
        .setDeviceState(QKnxNetIp::DeviceState::IpFault)
        .setRoutingBusyWaitTime(50)
        .setRoutingBusyControl(0)
        .create();
    simulateFramesReceived(routingBusyFrame);
    QVERIFY(indicationRcvEmitted);
}

void tst_QKnxNetIpRouter::test_routing_busy_sent_packets_same_individual_address()
{
    if (!runTests)
        return;

    // test receiving routing indications!
    m_router.start();

    int indRecvCount = 0;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingIndicationReceived,
        [&](QKnxNetIpFrame frame, QKnxNetIpRouter::FilterAction routingAction) {
            QKnxNetIpRoutingIndicationProxy indicationRcv(frame);
            QVERIFY(indicationRcv.isValid());
            QCOMPARE(routingAction, QKnxNetIpRouter::FilterAction::RouteDecremented);
            indRecvCount++;
    });
    simulateFramesReceived(dummyRoutingIndication(QKnxAddress::createIndividual(1, 1, 1)), 7);

    // The 7th packet is ignored because there were 5 consecutive packets with
    // the same individual address. Router shall send automatically a busy message
    QCOMPARE(indRecvCount, 6);

    QCOMPARE(QKnxNetIpTestRouter::instance()->routerInstance()->m_framesReadCount, 6);
    QCOMPARE(QKnxNetIpTestRouter::instance()->routerInstance()->m_sameKnxDstAddressIndicationCount, 5);

    QCOMPARE(m_router.state(), QKnxNetIpRouter::State::NeighborBusy);
}

QKnxLinkLayerFrame generateDummySbcFrame()
{
    auto dst = QKnxAddress::createGroup(1, 1, 1);
    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueReadTpdu();
    auto ctrl = QKnxControlField::builder()
        .setFrameFormat(QKnxControlField::FrameFormat::Standard)
        .setBroadcast(QKnxControlField::Broadcast::System)
        .setPriority(QKnxControlField::Priority::Normal)
        .create();

    auto extCtrl = QKnxExtendedControlField::builder()
        .setDestinationAddressType(dst.type())
        .setHopCount(6)
        .create();

    return QKnxLinkLayerFrame::builder()
        .setControlField(ctrl)
        .setExtendedControlField(extCtrl)
        .setTpdu(tpdu)
        .setDestinationAddress(dst)
        .setSourceAddress({ QKnxAddress::Type::Individual, 0 })
        .setMessageCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
        .setMedium(QKnx::MediumType::NetIP)
        .createFrame();
}

void tst_QKnxNetIpRouter::test_routing_interface_sends_system_broadcast()
{
    if (!runTests)
        return;

    m_router.start();

    bool sbcSent = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingSystemBroadcastSent,
        [&](QKnxNetIpFrame frame) {
            sbcSent = true;
            QKnxNetIpRoutingSystemBroadcastProxy sbc(frame);
            QVERIFY(sbc.isValid());
    });
    m_router.sendRoutingSystemBroadcast(generateDummySbcFrame());
    QVERIFY(sbcSent);
}

void tst_QKnxNetIpRouter::test_routing_interface_receives_system_broadcast()
{
    if (!runTests)
        return;

    m_router.start();

    bool sbcRcvEmitted = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingSystemBroadcastReceived,
        [&](QKnxNetIpFrame frame) {
            sbcRcvEmitted = true;
            QKnxNetIpRoutingSystemBroadcastProxy sbc(frame);
            QVERIFY(sbc.isValid());
    });

    auto netIpFrame = QKnxNetIpRoutingSystemBroadcastProxy::builder()
        .setCemi(generateDummySbcFrame())
        .create();

    simulateFramesReceived(netIpFrame);
    QVERIFY(sbcRcvEmitted);
}

void tst_QKnxNetIpRouter::test_routing_filter()
{
    if (!runTests)
        return;

    QFETCH(QKnxAddress, interfaceIndividualAddress);
    QFETCH(QKnxNetIpRouter::FilterAction, expectedRoutingAction);
    QFETCH(QKnxAddress, dst);
    QFETCH(int, hopCount);
    QFETCH(QKnxNetIpRouter::FilterTable, filterTable);

    m_router.setIndividualAddress(interfaceIndividualAddress);
    m_router.setFilterTable(filterTable);
    m_router.start();

    bool receivedIndication = false;
    QObject::connect(&m_router, &QKnxNetIpRouter::routingIndicationReceived,
        [&](QKnxNetIpFrame frame, QKnxNetIpRouter::FilterAction routingAction) {
            QKnxNetIpRoutingIndicationProxy indicationRcv(frame);
            QVERIFY(indicationRcv.isValid());
            QCOMPARE(routingAction, expectedRoutingAction);
            receivedIndication = true;
    });

    auto frame = dummyRoutingIndication(dst, hopCount);
    simulateFramesReceived(frame);
    QVERIFY(receivedIndication);
}

void tst_QKnxNetIpRouter::test_routing_filter_data()
{
    QTest::addColumn<QKnxAddress>("interfaceIndividualAddress");
    QTest::addColumn<QKnxAddress>("dst");
    QTest::addColumn<int>("hopCount");
    QTest::addColumn<QKnxNetIpRouter::FilterAction>("expectedRoutingAction");
    QTest::addColumn<QKnxNetIpRouter::FilterTable>("filterTable");

    QKnxNetIpRouter::FilterTable filterTable;
    filterTable << QKnxAddress::createGroup(1, 1, 1);
    QTest::newRow("RouterIndividual(1.1.0)_groupDestination(1.1.1)_hop6")
        << QKnxAddress::createIndividual(1, 1, 0)
        << QKnxAddress::createGroup(1, 1, 1)
        << 6
        << QKnxNetIpRouter::FilterAction::RouteDecremented
        << filterTable;

    QTest::newRow("destinationAddressFiltered")
        << QKnxAddress::createIndividual(1, 1, 0)
        << QKnxAddress::createGroup(1, 2, 1)
        << 6
        << QKnxNetIpRouter::FilterAction::IgnoreTotally
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_groupDestination(1.1.1)_hop0")
        << QKnxAddress::createIndividual(1, 1, 0)
        << QKnxAddress::createGroup(1, 1, 1)
        << 0
        << QKnxNetIpRouter::FilterAction::IgnoreAcked
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_individualDestination(1.1.1)_hop0")
        << QKnxAddress::createIndividual(1, 1, 0)
        << QKnxAddress::createIndividual(1, 1, 1)
        << 0
        << QKnxNetIpRouter::FilterAction::IgnoreAcked
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_individualDestination(1.1.1)_hop5")
        << QKnxAddress::createIndividual(1, 1, 0)
        << QKnxAddress::createIndividual(1, 1, 1)
        << 5
        << QKnxNetIpRouter::FilterAction::RouteDecremented
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_individualDestination(1.1.1)_hop5")
        << QKnxAddress::createIndividual(1, 1, 0)
        << QKnxAddress::createIndividual(1, 1, 0)
        << 5
        << QKnxNetIpRouter::FilterAction::ForwardLocally
        << filterTable;
}

//TODO: test threshold for sending busy after incoming queue is
//      filled with 10 packets (queue should be able to hold until 30 messages)

#else

class tst_QKnxNetIpRouter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSKIP("QKnxNetIpRouter methods aren't available to test");
    }
};

#endif

QTEST_MAIN(tst_QKnxNetIpRouter)

#include "tst_qknxnetiprouter.moc"
