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
#include <QtKnx/qknxlinklayerframe.h>
#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/qknxtpdufactory.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxLinkLayerFrame : public QObject
{
    Q_OBJECT

private slots:
    void testConstructors()
    {
        QVector<QKnxAdditionalInfo> addInfos = {
            { QKnxAdditionalInfo::Type::BiBatInformation, QKnxByteArray::fromHex("1020") },
            { QKnxAdditionalInfo::Type::RfFastAckInformation, QKnxByteArray::fromHex("30405060") },
            { QKnxAdditionalInfo::Type::ManufacturerSpecificData, QKnxByteArray::fromHex("708090") }
        };

        QKnxLinkLayerFrame frame(QKnx::MediumType::NetIP, QKnxLinkLayerFrame::MessageCode::DataRequest);
        frame.setControlField(QKnxControlField(1));
        frame.setExtendedControlField(QKnxExtendedControlField(2));
        frame.addAdditionalInfo(addInfos.first());
        frame.setSourceAddress(QKnxAddress::Individual::Unregistered);
        frame.setDestinationAddress(QKnxAddress::Group::Broadcast);

        QCOMPARE(frame.controlField().bytes(), QKnxControlField(1).bytes());
        QCOMPARE(frame.extendedControlField().bytes(), QKnxExtendedControlField(2).bytes());
        QCOMPARE(frame.additionalInfos().first().bytes(), QKnxByteArray::fromHex("07021020"));
        QCOMPARE(frame.sourceAddress().bytes(), QKnxAddress::Individual::Unregistered.bytes());
        QCOMPARE(frame.destinationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());

        frame.addAdditionalInfo(addInfos.last());
        frame.addAdditionalInfo(addInfos.value(1));

        auto infos = frame.additionalInfos();
        for (int i = 0; i < infos.size(); ++i)
            QCOMPARE(infos[i].bytes(), addInfos[i].bytes());

        QCOMPARE(frame.destinationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());

        frame.removeAdditionalInfo(QKnxAdditionalInfo::Type::RfFastAckInformation);
        infos = frame.additionalInfos();
        QCOMPARE(infos.count(), 2);
        QCOMPARE(frame.additionalInfos().first().bytes(), QKnxByteArray::fromHex("07021020"));
        QCOMPARE(frame.additionalInfos().last().bytes(), QKnxByteArray::fromHex("fe03708090"));

        QCOMPARE(frame.destinationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());
    }

    void testTpduFetcher()
    {
        QVector<QKnxAdditionalInfo> addInfos = {
            { QKnxAdditionalInfo::Type::BiBatInformation, QKnxByteArray::fromHex("1020") },
            { QKnxAdditionalInfo::Type::RfFastAckInformation, QKnxByteArray::fromHex("30405060") },
            { QKnxAdditionalInfo::Type::ManufacturerSpecificData, QKnxByteArray::fromHex("708090") }
        };

        QKnxLinkLayerFrame frame(QKnx::MediumType::NetIP, QKnxLinkLayerFrame::MessageCode::DataRequest);
        frame.setControlField(QKnxControlField(1));
        frame.setExtendedControlField(QKnxExtendedControlField(2));
        frame.addAdditionalInfo(addInfos.first());
        frame.setSourceAddress(QKnxAddress::Individual::Unregistered);
        frame.setDestinationAddress(QKnxAddress::Group::Broadcast);

        QKnxTpdu tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu({ 0x01 });
        QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x00, 0x81 }));
        frame.setTpdu(tpdu);
        QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x00, 0x81 }));
        QKnxTpdu tpdu2 = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu({ 0x01, 0x01 });
        QCOMPARE(tpdu2.bytes(), QKnxByteArray({ 0x00, 0x80, 0x01, 0x01 }));
        frame.setTpdu(tpdu2);
        QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x00, 0x80, 0x01, 0x01 }));
        QKnxTpdu tpdu4 = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu({ 0xff });
        QCOMPARE(tpdu4.bytes(), QKnxByteArray({ 0x00, 0x80, 0xff }));
        frame.setTpdu(tpdu4);
        QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x00, 0x80, 0xff }));
    }

    void testEmptyAdditionalInformation()
    {
        QKnxLinkLayerFrame frame(QKnx::MediumType::NetIP, QKnxLinkLayerFrame::MessageCode::DataRequest);
        QVERIFY(frame.additionalInfosSize() == 0);
    }

    void testAdditionalInformationBytes()
    {
        QKnxAdditionalInfo info { QKnxAdditionalInfo::Type::BiBatInformation, QKnxByteArray::fromHex("1020") };
        QVERIFY(info.bytes() == QKnxByteArray::fromHex("07021020"));

        auto ctrl = QKnxControlField::builder()
            .setFrameFormat(QKnxControlField::FrameFormat::Extended)
            .setBroadcast(QKnxControlField::Broadcast::Domain)
            .setPriority(QKnxControlField::Priority::Normal)
            .create();

        auto extCtrl = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Group)
            .create();

        auto tpdu = QKnxTpduFactory::Multicast::createGroupValueReadTpdu();
        auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extCtrl)
            .setTpdu(tpdu)
            .setDestinationAddress({ QKnxAddress::Type::Group, QString("0/0/2") })
            .setSourceAddress({ QKnxAddress::Type::Individual, 0 })
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .setAdditionalInfos({ info })
            .createFrame();

        const quint8 numberAdditionalInfos = 1;
        QCOMPARE(frame.additionalInfos().size(), numberAdditionalInfos);
        const quint8 sizeBytes = 4;
        QCOMPARE(frame.additionalInfosSize(), sizeBytes);
        QCOMPARE(frame.additionalInfos().at(0), info);
        QCOMPARE(frame.bytes().mid(2, frame.additionalInfosSize()), info.bytes());
    }

    void testDebugStream()
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
    }

    //TODO: test the isValid function
};

QTEST_MAIN(tst_QKnxLinkLayerFrame)

#include "tst_qknxtunnelframe.moc"
