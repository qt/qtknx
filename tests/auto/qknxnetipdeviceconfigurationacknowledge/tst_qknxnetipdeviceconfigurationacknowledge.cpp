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
#include <QtKnx/qknxnetipdeviceconfigurationacknowledge.h>
#include <QtKnx/QKnxNetIpConnectionHeader>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDeviceConfigurationAcknowledge: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testProxyMethods();
};

void tst_QKnxNetIpDeviceConfigurationAcknowledge::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpDeviceConfigurationAcknowledgeProxy acknowledge(frame);

    QCOMPARE(acknowledge.isValid(), false);
}

void tst_QKnxNetIpDeviceConfigurationAcknowledge::testConstructor()
{
    auto frame = QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder()
        .setChannelId(255)
        .setSequenceNumber(250)
        .setStatus(QKnxNetIp::Error::ConnectionId)
        .create();
    QKnxNetIpDeviceConfigurationAcknowledgeProxy acknowledge(frame);

    QCOMPARE(acknowledge.isValid(), true);
    QCOMPARE(frame.size(), quint16(10));
    QCOMPARE(frame.data(), QKnxByteArray {});
    QCOMPARE(frame.data().size(), 0);
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("06100311000a04fffa21"));
    QCOMPARE(frame.connectionHeader().bytes(), QKnxByteArray::fromHex("04fffa21"));

    QCOMPARE(acknowledge.channelId(), quint8(255));
    QCOMPARE(acknowledge.sequenceNumber(), quint8(250));
    QCOMPARE(acknowledge.status(), QKnxNetIp::Error::ConnectionId);
}

void tst_QKnxNetIpDeviceConfigurationAcknowledge::testProxyMethods()
{
    quint8 channelId = 255;
    quint8 sequenceNumber = 250;
    auto status = QKnxNetIp::Error::ConnectionId;
    auto frame = QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder()
        .setChannelId(channelId)
        .setSequenceNumber(sequenceNumber)
        .setStatus(status)
        .create();
    {
        // Check channelId(), status() and isValid() with Wrong service type
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::DisconnectRequest,
                                 QKnxNetIpConnectionHeader { channelId,
                                                             sequenceNumber,
                                                             quint8(status) }
                               };
        const QKnxNetIpDeviceConfigurationAcknowledgeProxy view(frame);
        QCOMPARE(view.channelId(), channelId);
        QCOMPARE(view.status(), status);
        QCOMPARE(view.sequenceNumber(), sequenceNumber);
        QCOMPARE(view.isValid(), false);
    }
    {
        // Check channelId(), status(), sequenceNumber() and isValid() with
        // valid device configuration acknowledge frame
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge };
        QCOMPARE(frame.header().size(), 6);
        QCOMPARE(frame.dataSize(), 0);
        QCOMPARE(frame.size(), 6);
        const QKnxNetIpDeviceConfigurationAcknowledgeProxy view(frame);
        QCOMPARE(view.isValid(), false);
    }
    {
        // Check channelId(), status(), sequenceNumber() and isValid() with
        // valid device configuration acknowledge frame
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge,
                                 QKnxNetIpConnectionHeader { channelId,
                                                             sequenceNumber,
                                                             quint8(status) }
                               };
        const QKnxNetIpDeviceConfigurationAcknowledgeProxy view(frame);
        QCOMPARE(view.channelId(), channelId);
        QCOMPARE(view.status(), status);
        QCOMPARE(view.sequenceNumber(), sequenceNumber);
        QCOMPARE(view.isValid(), true);
    }
}

void tst_QKnxNetIpDeviceConfigurationAcknowledge::testDebugStream()
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

    qDebug() << QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100311000a04000000"));

    qDebug() << QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder()
        .setChannelId(255)
        .setSequenceNumber(250)
        .setStatus(QKnxNetIp::Error::None)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100311000a04fffa00"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceConfigurationAcknowledge)

#include "tst_qknxnetipdeviceconfigurationacknowledge.moc"
