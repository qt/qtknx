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
#include <QtKnx/qknxnetiproutinginterface.h>
#include <QtKnx/qknxnetiproutingbusy.h>
#include <QtKnx/qknxnetiproutingindication.h>
#include <QtKnx/qknxnetiproutinglostmessage.h>
#include <QtKnx/qknxnetiproutingsystembroadcast.h>

#include <QtKnx/private/qknxnetiproutinginterface_p.h>
#include <QtKnx/private/qknxtestingrouter_p.h>
#include <QtKnx/private/qknxtpdufactory_p.h>

#include <QtCore/qdebug.h>
#include <QtNetwork/qnetworkdatagram.h>
#include <QtNetwork/qnetworkinterface.h>
#include <QtNetwork/qudpsocket.h>
#include <QtTest>

Q_DECLARE_METATYPE(QKnxAddress)

#ifdef QT_BUILD_INTERNAL

class tst_QKnxNetIpRoutingInterface : public QObject
{
    Q_OBJECT

public:
    tst_QKnxNetIpRoutingInterface();

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

    QKnxNetIpRoutingInterface m_routingInterface;
};

tst_QKnxNetIpRoutingInterface::tst_QKnxNetIpRoutingInterface()
{
    // look for one interface running and able to multicast

    const auto interfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface foundIface;
    for (const auto &iface : interfaces) {
        if (iface.addressEntries().isEmpty())
            continue;

        if (!iface.flags().testFlag(QNetworkInterface::IsRunning)
            || !iface.flags().testFlag(QNetworkInterface::CanMulticast)) {
            continue;
        }

        kIface = iface;
    }
}

void tst_QKnxNetIpRoutingInterface::initTestCase()
{
    // don't run test case if this check fails
    runTests = kIface.flags().testFlag(QNetworkInterface::IsRunning)
           && kIface.flags().testFlag(QNetworkInterface::CanMulticast);

    if (!runTests)
        return;

    auto interfaceIndividualAddress = QKnxAddress::createIndividual(1, 1, 0);
    m_routingInterface.setIndividualAddress(interfaceIndividualAddress);
    m_routingInterface.setInterfaceAffinity(kIface);
}

void tst_QKnxNetIpRoutingInterface::cleanup()
{
    if (!runTests)
        return;

    m_routingInterface.disconnect();
    m_routingInterface.stop();
}

void tst_QKnxNetIpRoutingInterface::test_network_interface()
{
    if (!runTests)
        return;

    bool errorEmitted = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::errorOccurred,
            [&](QKnxNetIpRoutingInterface::Error error, QString errorString){
        QCOMPARE(error, QKnxNetIpRoutingInterface::Error::Network);
        QCOMPARE(error, m_routingInterface.error());
        QCOMPARE(errorString, m_routingInterface.errorString());
        QCOMPARE(m_routingInterface.state(), QKnxNetIpRoutingInterface::State::Failure);
        errorEmitted = true;
    });
    {
        // setting invalid interface should cause error
        auto m_iface = QNetworkInterface();
        m_routingInterface.setInterfaceAffinity(m_iface);
        QVERIFY(errorEmitted);
    }
    {
        // setting invalid interface using IP address of the interface
        errorEmitted = false;
        m_routingInterface.setInterfaceAffinity(QHostAddress("2.2.2.2"));
        QVERIFY(errorEmitted);
    }
    {
        // local host address does not allow multicast
        errorEmitted = false;
        m_routingInterface.setInterfaceAffinity(QHostAddress(QHostAddress::LocalHost));
        QVERIFY(errorEmitted);
    }
    {
        // setting a valid interface no signal emitted
        errorEmitted = false;
        m_routingInterface.setInterfaceAffinity(kIface);
        QVERIFY(!errorEmitted);
        QCOMPARE(kIface.index(), m_routingInterface.interfaceAffinity().index());
    }
}

void tst_QKnxNetIpRoutingInterface::test_multicast_address()
{
    if (!runTests)
        return;

    {
        // default multicast address
        QKnxNetIpRoutingInterface m_router;
        QCOMPARE(m_router.multicastAddress(), kMulticastAddress);
    }
    {
        // setting an invalid multicast address shouldn't work
        QKnxNetIpRoutingInterface m_router;
        auto kMulticastAddress = QHostAddress(QHostAddress::LocalHost);
        m_router.setMulticastAddress(kMulticastAddress);
        QVERIFY(m_router.multicastAddress() != kMulticastAddress);
    }
    {
        // setting a valid multicast address
        QKnxNetIpRoutingInterface m_router;
        auto address = QHostAddress("224.0.55.55");
        m_router.setMulticastAddress(address);
        QCOMPARE(m_router.multicastAddress(), address);
    }
}

void tst_QKnxNetIpRoutingInterface::test_udp_sockets()
{
    if (!runTests)
        return;

    QUdpSocket receiver;
    QSignalSpy recvSpy(&receiver, SIGNAL(readyRead()));
    receiver.bind(QHostAddress(QHostAddress::AnyIPv4),
                  kMulticastPort,
                  (QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint));
    receiver.joinMulticastGroup(kMulticastAddress);
    QList<QByteArray> datagrams = QList<QByteArray>()
                                  << QByteArray("0123")
                                  << QByteArray("4567")
                                  << QByteArray("89ab")
                                  << QByteArray("cdef");

    QUdpSocket sender;
    QSignalSpy bytesspy(&sender, SIGNAL(bytesWritten(qint64)));
    for (const QByteArray &datagram : datagrams) {
        QNetworkDatagram dgram(datagram, kMulticastAddress , kMulticastPort);
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

QKnxNetIpFrame dummyRoutingIndication(QKnxAddress dst = QKnxAddress::createIndividual(1,1,1),
                                      quint8 hopCount = 6)
{
    //auto dst = QKnxAddress::createIndividual(1,1,1);
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

void tst_QKnxNetIpRoutingInterface::simulateFramesReceived(const QKnxNetIpFrame &netIpFrame, int numFrames)
{
    if (!runTests)
        return;

    QUdpSocket * s = new QUdpSocket();

    s->bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    s->setMulticastInterface(kIface);

    for (; numFrames > 0 ; --numFrames) {
        QNetworkDatagram datagram(netIpFrame.bytes().toByteArray(),
                                  kMulticastAddress, kMulticastPort);
        s->writeDatagram(datagram);
    }

    // notify router socket to wait for any received packet
    TestingRouter::instance()->emitReadyRead();

    if (s) {
        s->disconnect();
        s->deleteLater();
    }
}

void tst_QKnxNetIpRoutingInterface::test_routing()
{
    if (!runTests)
        return;

    bool stateChangedEmitted = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::stateChanged
                     , [&](QKnxNetIpRoutingInterface::State state) {
        stateChangedEmitted = true;
        QCOMPARE(state, QKnxNetIpRoutingInterface::State::Routing);
    });

    m_routingInterface.start();
    QVERIFY(stateChangedEmitted);
}


void tst_QKnxNetIpRoutingInterface::test_routing_sends_indications()
{
    if (!runTests)
        return;

    m_routingInterface.start();

    auto bytes  = QKnxByteArray::fromHex("2900b4e000000002010000");
    auto frameSent = QKnxLinkLayerFrame::builder()
                     .setData(bytes)
                     .setMedium(QKnx::MediumType::NetIP)
                     .createFrame();

    bool indicationSentEmitted = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::routingIndicationSent
                     , [&](QKnxNetIpFrame frame) {
        indicationSentEmitted = true;
        QKnxNetIpRoutingIndicationProxy indicationSent(frame);
        QVERIFY(indicationSent.isValid());
        QCOMPARE(indicationSent.linkLayerFrame().bytes(), frameSent.bytes());
    });
    m_routingInterface.sendRoutingIndication(frameSent);
    QVERIFY(indicationSentEmitted);

    bool stateChangedEmitted = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::stateChanged
                     , [&](QKnxNetIpRoutingInterface::State state) {
        stateChangedEmitted = true;
        QCOMPARE(state, QKnxNetIpRoutingInterface::State::Stop);
    });
    m_routingInterface.stop();
    QVERIFY(stateChangedEmitted);
}

// Test receiving routing indications!
void tst_QKnxNetIpRoutingInterface::test_routing_receives_indications()
{
    if (!runTests)
        return;

    m_routingInterface.start();

    bool indicationRcvEmitted = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::routingIndicationReceived
                     , [&](QKnxNetIpFrame frame
                     , QKnxNetIpRoutingInterface::FilterAction routingAction) {
        indicationRcvEmitted = true;
        QKnxNetIpRoutingIndicationProxy indicationRcv(frame);
        QVERIFY(indicationRcv.isValid());
        QCOMPARE(routingAction, QKnxNetIpRoutingInterface::FilterAction::RouteDecremented);
    });
    simulateFramesReceived(dummyRoutingIndication());
    QVERIFY(indicationRcvEmitted);
}

void tst_QKnxNetIpRoutingInterface::test_routing_receives_busy()
{
    if (!runTests)
        return;

    // test receiving routing busy!
    m_routingInterface.start();

    bool indicationRcvEmitted = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::routingBusyReceived
                     , [&](QKnxNetIpFrame frame) {
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

void tst_QKnxNetIpRoutingInterface::test_routing_busy_sent_packets_same_individual_address()
{
    if (!runTests)
        return;

    // test receiving routing indications!
    m_routingInterface.start();

    int indRecvCount = 0;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::routingIndicationReceived
                     , [&](QKnxNetIpFrame frame
                     , QKnxNetIpRoutingInterface::FilterAction routingAction) {
        QKnxNetIpRoutingIndicationProxy indicationRcv(frame);
        QVERIFY(indicationRcv.isValid());
        QCOMPARE(routingAction, QKnxNetIpRoutingInterface::FilterAction::RouteDecremented);
        indRecvCount++;
    });
    simulateFramesReceived(dummyRoutingIndication(), 7);

    // The 7th packet is ignored because there were 5 consecutive packets with
    // the same individual address. Router shall send automatically a busy message
    QCOMPARE(indRecvCount, 6);

    QCOMPARE(TestingRouter::instance()->routerInstance()->m_framesReadCount, 6);
    QCOMPARE(TestingRouter::instance()->routerInstance()->m_sameKnxDstAddressIndicationCount, 5);

    QCOMPARE(m_routingInterface.state(), QKnxNetIpRoutingInterface::State::NeighborBusy);
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

void tst_QKnxNetIpRoutingInterface::test_routing_interface_sends_system_broadcast()
{
    if (!runTests)
        return;

    m_routingInterface.start();

    bool sbcSent = false;
    QObject::connect(&m_routingInterface,
        &QKnxNetIpRoutingInterface::routingSystemBroadcastSent, [&](QKnxNetIpFrame frame) {
            sbcSent = true;
            QKnxNetIpRoutingSystemBroadcastProxy sbc(frame);
            QVERIFY(sbc.isValid());
    });
    m_routingInterface.sendRoutingSystemBroadcast(generateDummySbcFrame());
    QVERIFY(sbcSent);
}

void tst_QKnxNetIpRoutingInterface::test_routing_interface_receives_system_broadcast()
{
    if (!runTests)
        return;

    m_routingInterface.start();

    bool sbcRcvEmitted = false;
    QObject::connect(&m_routingInterface,
                     &QKnxNetIpRoutingInterface::routingSystemBroadcastReceived
                     , [&](QKnxNetIpFrame frame) {
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

void tst_QKnxNetIpRoutingInterface::test_routing_filter()
{
    if (!runTests)
        return;

    QFETCH(QKnxAddress, interfaceIndividualAddress);
    QFETCH(QKnxNetIpRoutingInterface::FilterAction, expectedRoutingAction);
    QFETCH(QKnxAddress, dst);
    QFETCH(int, hopCount);
    QFETCH(QKnxNetIpRoutingInterface::FilterTable, filterTable);

    m_routingInterface.setIndividualAddress(interfaceIndividualAddress);
    m_routingInterface.setFilterTable(filterTable);
    m_routingInterface.start();

    bool receivedIndication = false;
    QObject::connect(&m_routingInterface, &QKnxNetIpRoutingInterface::routingIndicationReceived
                     , [&](QKnxNetIpFrame frame
                     , QKnxNetIpRoutingInterface::FilterAction routingAction) {
        QKnxNetIpRoutingIndicationProxy indicationRcv(frame);
        QVERIFY(indicationRcv.isValid());
        QCOMPARE(routingAction, expectedRoutingAction);
        receivedIndication = true;
    });

    auto frame = dummyRoutingIndication(dst, hopCount);
    simulateFramesReceived(frame);
    QVERIFY(receivedIndication);
}

void tst_QKnxNetIpRoutingInterface::test_routing_filter_data()
{
    QTest::addColumn<QKnxAddress>("interfaceIndividualAddress");
    QTest::addColumn<QKnxAddress>("dst");
    QTest::addColumn<int>("hopCount");
    QTest::addColumn<QKnxNetIpRoutingInterface::FilterAction>("expectedRoutingAction");
    QTest::addColumn<QKnxNetIpRoutingInterface::FilterTable>("filterTable");

    QKnxNetIpRoutingInterface::FilterTable filterTable;
    filterTable << QKnxAddress::createGroup(1,1,0);
    QTest::newRow("RouterIndividual(1.1.0)_groupDestination(1.1.1)_hop6")
        << QKnxAddress::createIndividual(1,1,0)
        << QKnxAddress::createGroup(1,1,1)
        << 6
        << QKnxNetIpRoutingInterface::FilterAction::RouteDecremented
        << filterTable;

    QTest::newRow("destinationAddressFiltered")
        << QKnxAddress::createIndividual(1,1,0)
        << QKnxAddress::createGroup(1,2,1)
        << 6
        << QKnxNetIpRoutingInterface::FilterAction::IgnoreTotally
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_groupDestination(1.1.1)_hop0")
        << QKnxAddress::createIndividual(1,1,0)
        << QKnxAddress::createGroup(1,1,1)
        << 0
        << QKnxNetIpRoutingInterface::FilterAction::IgnoreAcked
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_individualDestination(1.1.1)_hop0")
        << QKnxAddress::createIndividual(1,1,0)
        << QKnxAddress::createIndividual(1,1,1)
        << 0
        << QKnxNetIpRoutingInterface::FilterAction::IgnoreAcked
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_individualDestination(1.1.1)_hop5")
        << QKnxAddress::createIndividual(1,1,0)
        << QKnxAddress::createIndividual(1,1,1)
        << 5
        << QKnxNetIpRoutingInterface::FilterAction::RouteDecremented
        << filterTable;

    QTest::newRow("RouterIndividual(1.1.0)_individualDestination(1.1.1)_hop5")
        << QKnxAddress::createIndividual(1,1,0)
        << QKnxAddress::createIndividual(1,1,0)
        << 5
        << QKnxNetIpRoutingInterface::FilterAction::ForwardLocally
        << filterTable;
}

//TODO: test threshold for sending busy after incoming queue is
//      filled with 10 packets (queue should be able to hold until 30 messages)

#else

class tst_QKnxNetIpRoutingInterface : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSKIP("QKnxNetIpRoutingInterface methods aren't available to test");
    }
};

#endif

QTEST_MAIN(tst_QKnxNetIpRoutingInterface)

#include "tst_qknxnetiproutinginterface.moc"
