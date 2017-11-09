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
#include <QtKnx/qknxtunnelframe.h>
#include <QtKnx/qknxnpdufactory.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxTunnelFrame : public QObject
{
    Q_OBJECT

private slots:
    void testConstructors()
    {
        QVector<QKnxAdditionalInfo> addInfos = {
            { QKnxAdditionalInfo::Type::BiBatInformation, QByteArray::fromHex("1020") },
            { QKnxAdditionalInfo::Type::RfFastAckInformation, QByteArray::fromHex("30405060") },
            { QKnxAdditionalInfo::Type::ManufactorSpecificData, QByteArray::fromHex("708090") }
        };

        QKnxTunnelFrame frame(QKnxTunnelFrame::MessageCode::DataRequest);
        frame.setControlField(QKnxControlField(1));
        frame.setExtendedControlField(QKnxExtendedControlField(2));
        frame.addAdditionalInfo(addInfos.first());
        frame.setSourceAddress(QKnxAddress::Individual::Unregistered);
        frame.setDestinationAddress(QKnxAddress::Group::Broadcast);

        QCOMPARE(frame.controlField().bytes(), QKnxControlField(1).bytes());
        QCOMPARE(frame.extendedControlField().bytes(), QKnxExtendedControlField(2).bytes());
        QCOMPARE(frame.additionalInfos().first().bytes(), QByteArray::fromHex("07021020"));
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
        QCOMPARE(frame.additionalInfos().first().bytes(), QByteArray::fromHex("07021020"));
        QCOMPARE(frame.additionalInfos().last().bytes(), QByteArray::fromHex("fe03708090"));

        QCOMPARE(frame.destinationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());
    }

    void testNpduFetcher()
    {
        QVector<QKnxAdditionalInfo> addInfos = {
            { QKnxAdditionalInfo::Type::BiBatInformation, QByteArray::fromHex("1020") },
            { QKnxAdditionalInfo::Type::RfFastAckInformation, QByteArray::fromHex("30405060") },
            { QKnxAdditionalInfo::Type::ManufactorSpecificData, QByteArray::fromHex("708090") }
        };

        QKnxTunnelFrame frame(QKnxTunnelFrame::MessageCode::DataRequest);
        frame.setControlField(QKnxControlField(1));
        frame.setExtendedControlField(QKnxExtendedControlField(2));
        frame.addAdditionalInfo(addInfos.first());
        frame.setSourceAddress(QKnxAddress::Individual::Unregistered);
        frame.setDestinationAddress(QKnxAddress::Group::Broadcast);

        QKnxNpdu npdu = QKnxNpduFactory::Multicast::createGroupValueWriteNpdu(
            QByteArray::fromHex("01"));
        QCOMPARE(npdu.bytes(), QByteArray::fromHex("010081"));
        frame.setNpdu(npdu);
        QCOMPARE(frame.npdu().bytes(), QByteArray::fromHex("010081"));
        QKnxNpdu npdu2 = QKnxNpduFactory::Multicast::createGroupValueWriteNpdu(
            QByteArray::fromHex("0101"));
        QCOMPARE(npdu2.bytes(), QByteArray::fromHex("0300800101"));
        frame.setNpdu(npdu2);
        QCOMPARE(frame.npdu().bytes(), QByteArray::fromHex("0300800101"));
        QKnxNpdu npdu4 = QKnxNpduFactory::Multicast::createGroupValueWriteNpdu(
            QByteArray::fromHex("ff"));
        QCOMPARE(npdu4.bytes(), QByteArray::fromHex("020080ff"));
        frame.setNpdu(npdu4);
        QCOMPARE(frame.npdu().bytes(), QByteArray::fromHex("020080ff"));
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

    void testDataStream()
    {
    }

    //TO DO: test the isValid function
};

QTEST_MAIN(tst_QKnxTunnelFrame)

#include "tst_qknxtunnelframe.moc"
