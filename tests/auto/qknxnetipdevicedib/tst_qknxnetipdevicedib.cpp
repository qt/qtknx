/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDeviceDIB : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor()
    {
        QKnxNetIpDeviceDIB deviceDIB(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                     QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                     QKnxAddress::Individual::Unregistered,
                                     0x1111,
                                     QByteArray::fromHex("123456123456"),
                                     QHostAddress::AnyIPv4,
                                     QByteArray::fromHex("bcaec56690f9"),
                                     QByteArray("qt.io KNX device"));

        QCOMPARE(deviceDIB.mediumCode(), QKnxNetIpDeviceDIB::MediumCode::Ip);
        QCOMPARE(deviceDIB.descriptionTypeCode(), QKnxNetIp::DescriptionTypeCode::DeviceInfo);
        QCOMPARE(deviceDIB.deviceStatus(), QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode);
        QCOMPARE(deviceDIB.individualAddress().toString(), QKnxAddress::Individual::Unregistered.toString());
        QCOMPARE(deviceDIB.projectInstallationIdentfier(), quint16(0x1111));
        QCOMPARE(deviceDIB.serialNumber(), QByteArray::fromHex("123456123456"));
        QCOMPARE(deviceDIB.multicastAddress(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(deviceDIB.macAddress(), QByteArray::fromHex("bcaec56690f9"));
        QCOMPARE(deviceDIB.deviceName(), QByteArray("qt.io KNX device"));

        QCOMPARE(deviceDIB.payload().size(), quint16(52));
        QCOMPARE(deviceDIB.payload().bytes<QByteArray>(),
            QByteArray::fromHex("2001ffff111112345612345600000000bcaec56690f9")
            + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000"));

        QCOMPARE(deviceDIB.size(), quint16(54));
        QCOMPARE(deviceDIB.bytes<QByteArray>(),
            QByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
            + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000"));

        QCOMPARE(deviceDIB.toString(), QString::fromLatin1("Total size { 0x36 }, Code { 0x01 }, "
            "Bytes { 0x20, 0x01, 0xff, 0xff, 0x11, 0x11, 0x12, 0x34, 0x56, 0x12, 0x34, 0x56, 0x00, "
            "0x00, 0x00, 0x00, 0xbc, 0xae, 0xc5, 0x66, 0x90, 0xf9, 0x71, 0x74, 0x2e, 0x69, 0x6f, "
            "0x20, 0x4b, 0x4e, 0x58, 0x20, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x00, 0x00, 0x00, "
            "0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }"));
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

    qDebug() << QKnxNetIpDeviceDIB();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpDeviceDIB(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                   QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                   QKnxAddress::Individual::Unregistered,
                                   0x1111,
                                   QByteArray::fromHex("123456123456"),
                                   QHostAddress::AnyIPv4,
                                   QByteArray::fromHex("bcaec56690f9"),
                                   QByteArray("qt.io KNX device"));
    QCOMPARE(s_msg, QString::fromLatin1("0x36012001ffff111112345612345600000000bcaec56690f9"
            "71742e696f204b4e58206465766963650000000000000000000000000000"));
    }

    void testDataStream()
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpDeviceDIB(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                  QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                  QKnxAddress::Individual::Unregistered,
                                  0x1111,
                                  QByteArray::fromHex("123456123456"),
                                  QHostAddress::AnyIPv4,
                                  QByteArray::fromHex("bcaec56690f9"),
                                  QByteArray("qt.io KNX device"));
        QCOMPARE(byteArray, QByteArray::fromHex("36012001FFFF111112345612345600000000BCAEC56690F9")+
            "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000"));
    }
};
QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceDIB)

#include "tst_qknxnetipdevicedib.moc"
