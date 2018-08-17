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

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxnetiproutingsystembroadcast.h>
#include <QtKnx/private/qknxtpdufactory_p.h>

class tst_QKnxNetIpRoutingSystemBroadcast : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void setInvalidCemi();
    void setValidCemi();
};

void tst_QKnxNetIpRoutingSystemBroadcast::testDefaultConstructor()
{
    QKnxNetIpFrame frame;

    QKnxNetIpRoutingSystemBroadcastProxy routing(frame);
    QCOMPARE(routing.isValid(), false);

    frame = QKnxNetIpRoutingSystemBroadcastProxy::builder().create();
    QCOMPARE(routing.isValid(), false);
}

void tst_QKnxNetIpRoutingSystemBroadcast::setInvalidCemi()
{
    QKnxLinkLayerFrame cemi;
    QCOMPARE(cemi.isValid(), false);

    auto frame = QKnxNetIpRoutingSystemBroadcastProxy::builder()
                 .setCemi(cemi)
                 .create();
    QCOMPARE(frame.isValid(), false);
}

void tst_QKnxNetIpRoutingSystemBroadcast::setValidCemi()
{
    QKnxAddress dst = QKnxAddress::createGroup(1,1,1);
    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueReadTpdu();
    auto ctrl = QKnxControlField::builder()
                .setFrameFormat(QKnxControlField::FrameFormat::Standard)
                .setBroadcast(QKnxControlField::Broadcast::System)
                .setPriority(QKnxControlField::Priority::Normal)
                .create();

    auto extCtrl = QKnxExtendedControlField::builder()
                   .setDestinationAddressType(dst.type())
                   .setHopCount(5)
                   .create();
    QKnxLinkLayerFrame cemi = QKnxLinkLayerFrame::builder()
                 .setControlField(ctrl)
                 .setExtendedControlField(extCtrl)
                 .setTpdu(tpdu)
                 .setDestinationAddress(dst)
                 .setSourceAddress({ QKnxAddress::Type::Individual, 0 })
                 .setMessageCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
                 .setMedium(QKnx::MediumType::NetIP)
                 .createFrame();
    QCOMPARE(cemi.isValid(), true);

    auto frame = QKnxNetIpRoutingSystemBroadcastProxy::builder()
                 .setCemi(cemi)
                 .create();
    QCOMPARE(frame.isValid(), true);
    QCOMPARE(frame.serviceType(), QKnxNetIp::ServiceType::RoutingSystemBroadcast);
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpRoutingSystemBroadcast)

#include "tst_qknxnetiproutingsystembroadcast.moc"
