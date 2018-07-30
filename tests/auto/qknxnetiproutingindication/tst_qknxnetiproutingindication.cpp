/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxnetiproutingindication.h>
#include <QtKnx/private/qknxtpdufactory_p.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpRoutingIndication: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpRoutingIndication::testDefaultConstructor()
{
    QKnxNetIpFrame frame;

    QKnxNetIpRoutingIndicationProxy routing(frame);
    QCOMPARE(routing.isValid(), false);

    frame = QKnxNetIpRoutingIndicationProxy::builder().create();
    QCOMPARE(routing.isValid(), false);
}

void tst_QKnxNetIpRoutingIndication::testConstructor()
{
    //Only L_data.ind allowed in ROUTING_INDICATION
    auto bytes  = QKnxByteArray::fromHex("2900b4e011010002010081"); //L_data.ind
    auto cemi = QKnxLinkLayerFrame::builder()
                .setData(bytes)
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();
    auto netIpFrame = QKnxNetIpRoutingIndicationProxy::builder()
                      .setCemi(cemi)
                      .create();
    QKnxNetIpRoutingIndicationProxy routing(netIpFrame);
    QCOMPARE(routing.isValid(), true);

    // L_data.req not allowed inside ROUTING_INDICATION
    bytes = QKnxByteArray::fromHex("1100b4e000000002010000"); // L_data.req
    cemi = QKnxLinkLayerFrame::builder()
           .setData(bytes)
           .setMedium(QKnx::MediumType::NetIP)
           .createFrame();
    netIpFrame = QKnxNetIpRoutingIndicationProxy::builder()
                 .setCemi(cemi)
                 .create();
    QKnxNetIpRoutingIndicationProxy routing2(netIpFrame);
    QCOMPARE(routing2.isValid(), false);
}

void tst_QKnxNetIpRoutingIndication::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler)) {}
        ~DebugHandler() {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpRoutingIndicationProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061005300006"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpRoutingIndication)

#include "tst_qknxnetiproutingindication.moc"
