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
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDeviceDib : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor()
    {
        QKnxNetIpDeviceDib deviceDib(QKnxNetIpDeviceDib::Medium::Ip,
                                     QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                                     QKnxAddress::Individual::Unregistered,
                                     0x1111,
                                     QByteArray::fromHex("123456123456"),
                                     QHostAddress::AnyIPv4,
                                     QByteArray::fromHex("bcaec56690f9"),
                                     QByteArray("qt.io KNX device"));

        QCOMPARE(deviceDib.medium(), QKnxNetIpDeviceDib::Medium::Ip);
        QCOMPARE(deviceDib.descriptionType(), QKnxNetIp::DescriptionType::DeviceInfo);
        QCOMPARE(deviceDib.deviceStatus(), QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode);
        QCOMPARE(deviceDib.individualAddress().toString(), QKnxAddress::Individual::Unregistered.toString());
        QCOMPARE(deviceDib.projectInstallationIdentfier(), quint16(0x1111));
        QCOMPARE(deviceDib.serialNumber(), QByteArray::fromHex("123456123456"));
        QCOMPARE(deviceDib.multicastAddress(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(deviceDib.macAddress(), QByteArray::fromHex("bcaec56690f9"));
        QCOMPARE(deviceDib.deviceName(), QByteArray("qt.io KNX device"));

        QCOMPARE(deviceDib.payload().size(), quint16(52));
        QCOMPARE(deviceDib.payload().bytes<QByteArray>(),
            QByteArray::fromHex("2001ffff111112345612345600000000bcaec56690f9")
            + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000"));

        QCOMPARE(deviceDib.size(), quint16(54));
        QCOMPARE(deviceDib.bytes<QByteArray>(),
            QByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
            + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000"));

        QCOMPARE(deviceDib.toString(), QString::fromLatin1("Total size { 0x36 }, Code { 0x01 }, "
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

    qDebug() << QKnxNetIpDeviceDib();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpDeviceDib(QKnxNetIpDeviceDib::Medium::Ip,
                                   QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
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
        out << QKnxNetIpDeviceDib(QKnxNetIpDeviceDib::Medium::Ip,
                                  QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
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
QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceDib)

#include "tst_qknxnetipdevicedib.moc"
