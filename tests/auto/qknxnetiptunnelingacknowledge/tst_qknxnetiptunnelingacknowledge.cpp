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
#include <QtKnx/qknxnetiptunnelingacknowledge.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/QKnxNetIpConnectionHeader>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpTunnelingAcknowledge: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testValidationTunnelingAcknowledge();
};

void tst_QKnxNetIpTunnelingAcknowledge::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpTunnelingAcknowledgeProxy tunneling(frame);

    QCOMPARE(tunneling.isValid(), false);

    frame = QKnxNetIpTunnelingAcknowledgeProxy::builder().create();
    const QKnxNetIpTunnelingAcknowledgeProxy view(frame);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // TODO: make xxx::builder.create() consistent all around the module.
    // if no setters used it should create an invalid object.
    QCOMPARE(tunneling.isValid(), false);
    QCOMPARE(view.isValid(), false);
#else
    QCOMPARE(tunneling.isValid(), true);
    QCOMPARE(view.isValid(), true);
#endif
}

void tst_QKnxNetIpTunnelingAcknowledge::testConstructor()
{
    quint8 channelId = 15;
    quint8 sequenceNumber = 10;
    QKnxNetIp::Error status = QKnxNetIp::Error::VersionNotSupported;

    auto ackFrame = QKnxNetIpTunnelingAcknowledgeProxy::builder()
               .setChannelId(channelId)
               .setSequenceNumber(sequenceNumber)
               .setStatus(status)
               .create();

    QCOMPARE(ackFrame.isValid(), true);
    QCOMPARE(ackFrame.channelId(), channelId);
    QCOMPARE(ackFrame.sequenceNumber(), sequenceNumber);
    QCOMPARE(QKnxNetIp::Error(ackFrame.serviceTypeSpecificValue()), status);
    QCOMPARE(ackFrame.data(), QKnxByteArray());

    const QKnxNetIpTunnelingAcknowledgeProxy view(ackFrame);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.channelId(), channelId);
    QCOMPARE(view.sequenceNumber(), sequenceNumber);
    QCOMPARE(QKnxNetIp::Error(view.status()), status);
}

void tst_QKnxNetIpTunnelingAcknowledge::testValidationTunnelingAcknowledge()
{
    quint8 channelId = 15;
    quint8 sequenceNumber = 10;
    QKnxNetIp::Error status = QKnxNetIp::Error::VersionNotSupported;
    {
        // test tunneling Acknowledge frame with wrong service type set
        auto ackFrame = QKnxNetIpTunnelingAcknowledgeProxy::builder()
                        .setChannelId(channelId)
                        .setSequenceNumber(sequenceNumber)
                        .setStatus(status)
                        .create();
        QCOMPARE(ackFrame.isValid(), true);
        const QKnxNetIpTunnelingAcknowledgeProxy view(ackFrame);
        QCOMPARE(view.isValid(), true);
        ackFrame.setServiceType(QKnxNetIp::ServiceType::TunnelingRequest);
        QCOMPARE(view.isValid(), false);
    }
    {
        // missing channelId parameter, sequence number and status in header
        QKnxNetIpConnectionHeader header;
        QCOMPARE(header.size(), 0);
        QCOMPARE(header.isValid(), false);
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::TunnelingAcknowledge,
                                 header };
        QVERIFY(frame.size() != 10);
        QCOMPARE(frame.isValid(), false);
        const QKnxNetIpTunnelingAcknowledgeProxy view(frame);
        QCOMPARE(view.isValid(), false);
    }
    {
        QKnxNetIpConnectionHeader header {
            channelId,
            sequenceNumber,
            quint8(status)
        };
        QCOMPARE(header.size(), 4);
        QCOMPARE(header.isValid(), true);
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::TunnelingAcknowledge,
                                 header };
        QCOMPARE(frame.size(), 10);
        QCOMPARE(frame.isValid(), true);
        const QKnxNetIpTunnelingAcknowledgeProxy view(frame);
        QCOMPARE(view.isValid(), true);
    }
}

void tst_QKnxNetIpTunnelingAcknowledge::testDebugStream()
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

    qDebug() << QKnxNetIpTunnelingAcknowledgeProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100421000a04000000"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpTunnelingAcknowledge)

#include "tst_qknxnetiptunnelingacknowledge.moc"
