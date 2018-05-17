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
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxnetiptunnelingfeatureget.h>
#include <QtKnx/qknxnetiptunnelingfeatureset.h>
#include <QtKnx/qknxnetiptunnelingfeatureinfo.h>
#include <QtKnx/qknxnetiptunnelingfeatureresponse.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpTunnelingFeature : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpTunnelingFeature::testDefaultConstructor()
{
    {
        QKnxNetIpFrame frame;
        QKnxNetIpTunnelingFeatureGetProxy tunneling(frame);
        QCOMPARE(tunneling.isValid(), false);
        frame = QKnxNetIpTunnelingFeatureGetProxy::builder().create();
        QCOMPARE(tunneling.isValid(), false);
    }

    {
        QKnxNetIpFrame frame;
        QKnxNetIpTunnelingFeatureSetProxy tunneling(frame);
        QCOMPARE(tunneling.isValid(), false);
        frame = QKnxNetIpTunnelingFeatureSetProxy::builder().create();
        QCOMPARE(tunneling.isValid(), false);
    }

    {
        QKnxNetIpFrame frame;
        QKnxNetIpTunnelingFeatureInfoProxy tunneling(frame);
        QCOMPARE(tunneling.isValid(), false);
        frame = QKnxNetIpTunnelingFeatureInfoProxy::builder().create();
        QCOMPARE(tunneling.isValid(), false);
    }

    {
        QKnxNetIpFrame frame;
        QKnxNetIpTunnelingFeatureResponseProxy tunneling(frame);
        QCOMPARE(tunneling.isValid(), false);
        frame = QKnxNetIpTunnelingFeatureResponseProxy::builder().create();
        QCOMPARE(tunneling.isValid(), false);
    }
}

QT_BEGIN_NAMESPACE

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

QT_END_NAMESPACE

void tst_QKnxNetIpTunnelingFeature::testConstructor()
{
    {
        auto frame = QKnxNetIpTunnelingFeatureGetProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .create();
        QCOMPARE(frame.isNull(), false);
        QCOMPARE(frame.isValid(), true);
        QCOMPARE(frame.size(), 12);
        QCOMPARE(frame.dataSize(), 2);
        QCOMPARE(frame.serviceType(), QKnxNetIp::ServiceType::TunnelingFeatureGet);
        QCOMPARE(frame.channelId(), 15);
        QCOMPARE(frame.sequenceNumber(), 10);
        QCOMPARE(frame.serviceTypeSpecificValue(), 0x00);
        QCOMPARE(frame.connectionTypeSpecificHeaderItems(), QKnxByteArray());
        QCOMPARE(frame.data(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00 }));
        QCOMPARE(frame.constData(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00 }));
        QCOMPARE(frame.bytes(), QKnxByteArray({ 0x06, 0x10, 0x04, 0x22, 0x00, 0x0c, 0x04, 0x0f,
            0x0a, 0x00, quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00 }));

        QKnxNetIpTunnelingFeatureGetProxy feature(frame);
        QCOMPARE(feature.isValid(), true);
        QCOMPARE(feature.channelId(), 15);
        QCOMPARE(feature.sequenceNumber(), 10);
        QCOMPARE(feature.featureIdentifier(), QKnx::InterfaceFeature::IndividualAddress);
    }

    {
        auto frame = QKnxNetIpTunnelingFeatureSetProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .setFeatureValue(QKnxAddress::Individual::Unregistered.bytes())
            .create();
        QCOMPARE(frame.isNull(), false);
        QCOMPARE(frame.isValid(), true);
        QCOMPARE(frame.size(), 14);
        QCOMPARE(frame.dataSize(), 4);
        QCOMPARE(frame.serviceType(), QKnxNetIp::ServiceType::TunnelingFeatureSet);
        QCOMPARE(frame.channelId(), 15);
        QCOMPARE(frame.sequenceNumber(), 10);
        QCOMPARE(frame.serviceTypeSpecificValue(), 0x00);
        QCOMPARE(frame.connectionTypeSpecificHeaderItems(), QKnxByteArray());
        QCOMPARE(frame.data(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00, 0xff, 0xff }));
        QCOMPARE(frame.constData(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00, 0xff, 0xff }));
        QCOMPARE(frame.bytes(), QKnxByteArray({ 0x06, 0x10, 0x04, 0x24, 0x00, 0x0e, 0x04, 0x0f,
            0x0a, 0x00, quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00, 0xff, 0xff }));

        QKnxNetIpTunnelingFeatureSetProxy feature(frame);
        QCOMPARE(feature.isValid(), true);
        QCOMPARE(feature.channelId(), 15);
        QCOMPARE(feature.sequenceNumber(), 10);
        QCOMPARE(feature.featureIdentifier(), QKnx::InterfaceFeature::IndividualAddress);
        QCOMPARE(feature.featureValue(), QKnxByteArray({ 0xff, 0xff }));
    }

    {
        auto frame = QKnxNetIpTunnelingFeatureInfoProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .setFeatureValue(QKnxAddress::Individual::Unregistered.bytes())
            .create();
        QCOMPARE(frame.isNull(), false);
        QCOMPARE(frame.isValid(), true);
        QCOMPARE(frame.size(), 14);
        QCOMPARE(frame.dataSize(), 4);
        QCOMPARE(frame.serviceType(), QKnxNetIp::ServiceType::TunnelingFeatureInfo);
        QCOMPARE(frame.channelId(), 15);
        QCOMPARE(frame.sequenceNumber(), 10);
        QCOMPARE(frame.serviceTypeSpecificValue(), 0x00);
        QCOMPARE(frame.connectionTypeSpecificHeaderItems(), QKnxByteArray());
        QCOMPARE(frame.data(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00, 0xff, 0xff }));
        QCOMPARE(frame.constData(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00, 0xff, 0xff }));
        QCOMPARE(frame.bytes(), QKnxByteArray({ 0x06, 0x10, 0x04, 0x25, 0x00, 0x0e, 0x04, 0x0f,
            0x0a, 0x00, quint8(QKnx::InterfaceFeature::IndividualAddress), 0x00, 0xff, 0xff }));

        QKnxNetIpTunnelingFeatureInfoProxy feature(frame);
        QCOMPARE(feature.isValid(), true);
        QCOMPARE(feature.channelId(), 15);
        QCOMPARE(feature.sequenceNumber(), 10);
        QCOMPARE(feature.featureIdentifier(), QKnx::InterfaceFeature::IndividualAddress);
        QCOMPARE(feature.featureValue(), QKnxByteArray({ 0xff, 0xff }));
    }

    {
        auto frame = QKnxNetIpTunnelingFeatureResponseProxy::builder()
            .setChannelId(15)
            .setSequenceNumber(10)
            .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress)
            .setReturnCode(QKnx::ReturnCode::SuccessWithCrc)
            .setFeatureValue(QKnxAddress::Individual::Unregistered.bytes())
            .create();
        QCOMPARE(frame.isNull(), false);
        QCOMPARE(frame.isValid(), true);
        QCOMPARE(frame.size(), 14);
        QCOMPARE(frame.dataSize(), 4);
        QCOMPARE(frame.serviceType(), QKnxNetIp::ServiceType::TunnelingFeatureResponse);
        QCOMPARE(frame.channelId(), 15);
        QCOMPARE(frame.sequenceNumber(), 10);
        QCOMPARE(frame.serviceTypeSpecificValue(), 0x00);
        QCOMPARE(frame.connectionTypeSpecificHeaderItems(), QKnxByteArray());
        QCOMPARE(frame.data(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x01, 0xff, 0xff }));
        QCOMPARE(frame.constData(),
            QKnxByteArray({ quint8(QKnx::InterfaceFeature::IndividualAddress), 0x01, 0xff, 0xff }));
        QCOMPARE(frame.bytes(), QKnxByteArray({ 0x06, 0x10, 0x04, 0x23, 0x00, 0x0e, 0x04, 0x0f,
            0x0a, 0x00, quint8(QKnx::InterfaceFeature::IndividualAddress), 0x01, 0xff, 0xff }));

        QKnxNetIpTunnelingFeatureResponseProxy feature(frame);
        QCOMPARE(feature.isValid(), true);
        QCOMPARE(feature.channelId(), 15);
        QCOMPARE(feature.sequenceNumber(), 10);
        QCOMPARE(feature.featureIdentifier(), QKnx::InterfaceFeature::IndividualAddress);
        QCOMPARE(feature.returnCode(), QKnx::ReturnCode::SuccessWithCrc);
        QCOMPARE(feature.featureValue(), QKnxByteArray({ 0xff, 0xff }));
    }
}

void tst_QKnxNetIpTunnelingFeature::testDebugStream()
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

    qDebug() << QKnxNetIpTunnelingFeatureGetProxy::builder()
        .setFeatureIdentifier(QKnx::InterfaceFeature::IndividualAddress).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100422000c040000000600"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpTunnelingFeature)

#include "tst_qknxnetiptunnelingfeature.moc"
