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
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::Reserved);
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.toString(), QStringLiteral(""));
        QCOMPARE(info.bytes<QVector<quint8>>(), QVector<quint8> {});

        info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation,
            QByteArray::fromHex("1020"));
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::BiBatInformation);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.toString(), QStringLiteral("Type { 0x07 }, Size { 0x02 }, Data { 0x10, 0x20 }"));
        QCOMPARE(info.bytes<QByteArray>(), QByteArray::fromHex("07021020"));

        info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation,
            QVector<quint8>({ 0x10, 0x20, 0x30 }));
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::Reserved);
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.toString(), QStringLiteral(""));
        QCOMPARE(info.bytes<QVector<quint8>>(), QVector<quint8> {});

        info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type(0xaa),
            std::vector<quint8>{ 0x10, 0x20, 0x30 });
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::Reserved);
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.toString(), QStringLiteral(""));
        QCOMPARE(info.bytes<QVector<quint8>>(), QVector<quint8> {});
    }

    void testIsValid()
    {
        QKnxAdditionalInfo info;
        QCOMPARE(info.isValid(), false);

        info = { QKnxAdditionalInfo::Type::RfFastAckInformation,
            std::deque<quint8>{ 0x10, 0x20, 0x30 } };
        QCOMPARE(info.isValid(), false);

        info = { QKnxAdditionalInfo::Type::RfFastAckInformation,
            std::vector<quint8>({ 0x10, 0x20, 0x30, 0x40 }) };
        QCOMPARE(info.isValid(), true);

        QByteArray data(2, Qt::Uninitialized); data[0] = 0x10, data[1] = 0x20;
        info = { QKnxAdditionalInfo::Type::ManufactorSpecificData, data };
        QCOMPARE(info.isValid(), false);

        info = { QKnxAdditionalInfo::Type::ManufactorSpecificData,
            QVector<quint8>({ 0x10, 0x20, 0x30 }) };
        QCOMPARE(info.isValid(), true);

        info = { QKnxAdditionalInfo::Type::RfMediumInformation,
            QByteArray::fromHex("1020303040506070") };
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.bytes<QVector<quint8>>(),
            QVector<quint8> ({ 0x02, 0x08, 0x10, 0x20, 0x30, 0x30, 0x40, 0x50, 0x60, 0x70 }));

        // the passed data container has not a valid length
        info = { QKnxAdditionalInfo::Type::RfMediumInformation, QVector<quint8>(0x100, 0xff) };
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.bytes<QVector<quint8>>(), {});
    }

    void testExpectedDataSize()
    {
        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::Reserved), -1);

        bool fixed = false;
        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::PlMediumInformation,
            &fixed), 2);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::RfMediumInformation,
            &fixed), 8);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::BusmonitorStatusInfo,
            &fixed), 1);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::TimestampRelative,
            &fixed), 2);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::TimeDelayUntilSending,
            &fixed), 4);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp,
            &fixed), 4);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::BiBatInformation,
            &fixed), 2);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::RfMultiInformation,
            &fixed), 4);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::PreambleAndPostamble,
            &fixed), 3);
        QCOMPARE(fixed, true);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::RfFastAckInformation,
            &fixed), 2);
        QCOMPARE(fixed, false);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::ManufactorSpecificData,
            &fixed), 3);
        QCOMPARE(fixed, false);

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type::EscCode), -1);

        // illegal values

        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(static_cast<QKnxAdditionalInfo::Type>(-3)), -1);
        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(static_cast<QKnxAdditionalInfo::Type>(0xa0)), -1);
        QCOMPARE(QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type(0x100)), -1);
    }

    void testData()
    {
        QKnxAdditionalInfo info = { QKnxAdditionalInfo::Type::RfMediumInformation,
            QByteArray::fromHex("1020303040506070") };
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.bytes<QVector<quint8>>(),
            QVector<quint8> ({ 0x02, 0x08, 0x10, 0x20, 0x30, 0x30, 0x40, 0x50, 0x60, 0x70 }));
        QCOMPARE(info.data<QByteArray>(), QByteArray::fromHex("1020303040506070"));
    }

    void testFromBytes()
    {
        auto info = QKnxAdditionalInfo::fromBytes(QByteArray::fromHex("07021020"), 0);
        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::BiBatInformation);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.toString(), QStringLiteral("Type { 0x07 }, Size { 0x02 }, Data { 0x10, 0x20 }"));
        QCOMPARE(info.bytes<QByteArray>(), QByteArray::fromHex("07021020"));
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

        QByteArray data(2, Qt::Uninitialized); data[0] = 0x10, data[1] = 0x20;
        qDebug() << QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation, data);
        QCOMPARE(s_msg, QString::fromLatin1("0x07021020"));
    }

    void testDataStream()
    {
        QByteArray byteArray;
        QByteArray data(2, Qt::Uninitialized); data[0] = 0x10, data[1] = 0x20;

        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxAdditionalInfo(QKnxAdditionalInfo::Type::BiBatInformation, data);

        QKnxAdditionalInfo info;
        QDataStream in(&byteArray, QIODevice::ReadOnly);
        in >> info;

        QCOMPARE(info.type(), QKnxAdditionalInfo::Type::BiBatInformation);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.toString(), QStringLiteral("Type { 0x07 }, Size { 0x02 }, Data { 0x10, 0x20 }"));
        QCOMPARE(info.bytes<QVector<quint8>>(), QVector<quint8> ({ 0x07, 0x02, 0x10, 0x20 }));
    }
};

QTEST_MAIN(tst_QKnxAdditionalinfo)

#include "tst_qknxadditionalinfo.moc"
