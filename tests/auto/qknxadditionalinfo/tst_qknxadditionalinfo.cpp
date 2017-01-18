/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxadditionalinfo.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxAdditionalinfo : public QObject
{
    Q_OBJECT

private slots:
    void testConstructors()
    {
        QKnxAdditionalInfo info;
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::EscCode);
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.toString(), QStringLiteral(""));
        QCOMPARE(info.rawData(), (QVector<quint8> { }));

        info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation, { 0x10, 0x20 });
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::BiBatInformation);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.toString(), QStringLiteral("Type { 0x07 }, Size { 0x02 }, Data { 0x10, 0x20 }"));
        QCOMPARE(info.rawData(), (QVector<quint8> { 0x07, 0x02, 0x10, 0x20 }));

        info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation, { 0x10, 0x20, 0x30 });
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::EscCode);
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.toString(), QStringLiteral(""));
        QCOMPARE(info.rawData(), (QVector<quint8> { }));

        info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type(0xaa), { 0x10, 0x20, 0x30 });
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::EscCode);
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.toString(), QStringLiteral(""));
        QCOMPARE(info.rawData(), (QVector<quint8> { }));
    }

    void testIsValid()
    {
        QKnxAdditionalInfo info;
        QCOMPARE(info.isValid(), false);

        info = { QKnxAdditionalInfo::Type::RfFastAckInformation, { 0x10, 0x20, 0x30 } };
        QCOMPARE(info.isValid(), false);

        info = { QKnxAdditionalInfo::Type::RfFastAckInformation, { 0x10, 0x20, 0x30, 0x40 } };
        QCOMPARE(info.isValid(), true);

        info = { QKnxAdditionalInfo::Type::ManufactorSpecificData, { 0x10, 0x20 } };
        QCOMPARE(info.isValid(), false);

        info = { QKnxAdditionalInfo::Type::ManufactorSpecificData, { 0x10, 0x20, 0x30 } };
        QCOMPARE(info.isValid(), true);

        info = { QKnxAdditionalInfo::Type::RfMediumInformation, { 0x10, 0x20, 0x30, 0x30, 0x40, 0x50, 0x60, 0x70 } };
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.rawData(), (QVector<quint8> { 0x02, 0x08, 0x10, 0x20, 0x30, 0x30, 0x40, 0x50, 0x60, 0x70 }));
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

        qDebug() << QKnxAdditionalInfo();
        QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

        qDebug() << QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation, { 0x10, 0x20 });
        QCOMPARE(s_msg, QString::fromLatin1("0x07021020"));
    }

    void testDataStream()
    {
        QByteArray byteArray;

        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation, { 0x10, 0x20 });

        QKnxAdditionalInfo info;
        QDataStream in(&byteArray, QIODevice::ReadOnly);
        in >> info;

        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::BiBatInformation);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.toString(), QStringLiteral("Type { 0x07 }, Size { 0x02 }, Data { 0x10, 0x20 }"));
        QCOMPARE(info.rawData(), (QVector<quint8> { 0x07, 0x02, 0x10, 0x20 }));
    }
};

QTEST_MAIN(tst_QKnxAdditionalinfo)

#include "tst_qknxadditionalinfo.moc"
