/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxtunnelframe.h>
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
        frame.setDestionationAddress(QKnxAddress::Group::Broadcast);

        QCOMPARE(frame.controlField().bytes(), QKnxControlField(1).bytes());
        QCOMPARE(frame.extendedControlField().bytes(), QKnxExtendedControlField(2).bytes());
        QCOMPARE(frame.additionalInfos().first().bytes(), QByteArray::fromHex("07021020"));
        QCOMPARE(frame.sourceAddress().bytes(), QKnxAddress::Individual::Unregistered.bytes());
        QCOMPARE(frame.destionationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());

        frame.addAdditionalInfo(addInfos.last());
        frame.addAdditionalInfo(addInfos.value(1));

        auto infos = frame.additionalInfos();
        for (int i = 0; i < infos.size(); ++i)
            QCOMPARE(infos[i].bytes(), addInfos[i].bytes());

        QCOMPARE(frame.destionationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());

        frame.removeAdditionalInfo(QKnxAdditionalInfo::Type::RfFastAckInformation);
        infos = frame.additionalInfos();
        QCOMPARE(infos.count(), 2);
        QCOMPARE(frame.additionalInfos().first().bytes(), QByteArray::fromHex("07021020"));
        QCOMPARE(frame.additionalInfos().last().bytes(), QByteArray::fromHex("fe03708090"));

        QCOMPARE(frame.destionationAddress().bytes(), QKnxAddress::Group::Broadcast.bytes());
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
};

QTEST_MAIN(tst_QKnxTunnelFrame)

#include "tst_qknxtunnelframe.moc"
