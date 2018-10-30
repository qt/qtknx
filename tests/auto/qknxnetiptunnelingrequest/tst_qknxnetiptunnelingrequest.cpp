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
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnamespace.h>
#include <QtKnx/QKnxNetIpConnectionHeader>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxnetiptunnelingrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpTunnelingRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testValidationTunnelingRequest();
    void testDebugStream();
};

void tst_QKnxNetIpTunnelingRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpTunnelingRequestProxy tunneling(frame);

    QCOMPARE(tunneling.isValid(), false);

    frame = QKnxNetIpTunnelingRequestProxy::builder().create();
    QCOMPARE(tunneling.isValid(), false);
}

void tst_QKnxNetIpTunnelingRequest::testConstructor()
{
    quint8 channelId = 15;
    quint8 sequenceNumber = 10;

    // L_data.req
    static const auto bytes = QKnxByteArray::fromHex("1100b4e000000002010000");
    auto cemi = QKnxLinkLayerFrame::builder()
                .setData(bytes)
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();

    auto reqFrame = QKnxNetIpTunnelingRequestProxy::builder()
                    .setChannelId(channelId)
                    .setSequenceNumber(sequenceNumber)
                    .setCemi(cemi)
                    .create();

    QCOMPARE(reqFrame.isValid(), true);
    QCOMPARE(reqFrame.channelId(), channelId);
    QCOMPARE(reqFrame.sequenceNumber(), sequenceNumber);
    QCOMPARE(reqFrame.data(), bytes);

    const QKnxNetIpTunnelingRequestProxy view(reqFrame);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.channelId(), channelId);
    QCOMPARE(view.sequenceNumber(), sequenceNumber);
    QCOMPARE(view.cemi().bytes(), bytes);
}

void tst_QKnxNetIpTunnelingRequest::testValidationTunnelingRequest()
{
    quint8 channelId = 15;
    quint8 sequenceNumber = 10;
    // L_data.req
    static const auto bytes = QKnxByteArray::fromHex("1100b4e000000002010000");
    auto cemi = QKnxLinkLayerFrame::builder()
                .setData(bytes)
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();
    {
        // test tunneling request frame with wrong service type set
        auto reqFrame = QKnxNetIpTunnelingRequestProxy::builder()
                        .setChannelId(channelId)
                        .setSequenceNumber(sequenceNumber)
                        .setCemi(cemi)
                        .create();
        QCOMPARE(reqFrame.isValid(), true);
        const QKnxNetIpTunnelingRequestProxy view(reqFrame);
        QCOMPARE(view.isValid(), true);
        reqFrame.setServiceType(QKnxNetIp::ServiceType::TunnelingAcknowledge);
        QCOMPARE(view.isValid(), false);
    }
    {
        // missing channelId parameter and sequence number in header
        QKnxNetIpConnectionHeader header;
        QCOMPARE(header.size(), 0);
        QCOMPARE(header.isValid(), false);
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::TunnelingAcknowledge,
                                 header,
                                 cemi.bytes()
                               };
        QVERIFY(frame.size() != 10);
        QCOMPARE(frame.isValid(), false);
        const QKnxNetIpTunnelingRequestProxy view(frame);
        QCOMPARE(view.isValid(), false);
    }
    {
        // missing KNX frame bytes after header in QKnxNetIpFrame
        QKnxNetIpConnectionHeader header { channelId, sequenceNumber };
        QCOMPARE(header.size(), 4);
        QCOMPARE(header.isValid(), true);
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::TunnelingRequest,
                                 header
                               };
        QCOMPARE(frame.size(), 10);
        QCOMPARE(frame.isValid(), true);
        const QKnxNetIpTunnelingRequestProxy view(frame);
        QCOMPARE(view.isValid(), false);
    }
    {
        // valid Tunneling Request
        QKnxNetIpConnectionHeader header { channelId, sequenceNumber };
        QCOMPARE(header.size(), 4);
        QCOMPARE(header.isValid(), true);
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::TunnelingRequest,
                                 header,
                                 cemi.bytes()
                               };
        QCOMPARE(frame.size(), 21);
        QCOMPARE(frame.isValid(), true);
        const QKnxNetIpTunnelingRequestProxy view(frame);
        QCOMPARE(view.isValid(), true);
    }
}

void tst_QKnxNetIpTunnelingRequest::testDebugStream()
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

    qDebug() << QKnxNetIpTunnelingRequestProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100420000a04000000"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpTunnelingRequest)

#include "tst_qknxnetiptunnelingrequest.moc"
