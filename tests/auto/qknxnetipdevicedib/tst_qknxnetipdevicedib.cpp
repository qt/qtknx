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
        QKnxNetIpDeviceDib deviceDib(QKnx::MediumType::NetIP,
                                     QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                                     QKnxAddress::Individual::Unregistered,
                                     0x1111,
                                     QKnxByteArray::fromHex("123456123456"),
                                     QHostAddress::AnyIPv4,
                                     QKnxByteArray::fromHex("bcaec56690f9"),
                                     QKnxByteArray("qt.io KNX device", 16));

        QCOMPARE(deviceDib.mediumType(), QKnx::MediumType::NetIP);
        QCOMPARE(deviceDib.descriptionType(), QKnxNetIp::DescriptionType::DeviceInfo);
        QCOMPARE(deviceDib.deviceStatus(), QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode);
        QCOMPARE(deviceDib.individualAddress().toString(), QKnxAddress::Individual::Unregistered.toString());
        QCOMPARE(deviceDib.projectInstallationIdentfier(), quint16(0x1111));
        QCOMPARE(deviceDib.serialNumber(), QKnxByteArray::fromHex("123456123456"));
        QCOMPARE(deviceDib.multicastAddress(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(deviceDib.macAddress(), QKnxByteArray::fromHex("bcaec56690f9"));
        QCOMPARE(deviceDib.deviceName(), QKnxByteArray("qt.io KNX device", 16));

        QCOMPARE(deviceDib.data().size(), quint16(52));
        QCOMPARE(deviceDib.data(),
            QKnxByteArray::fromHex("2001ffff111112345612345600000000bcaec56690f9")
            + QKnxByteArray("qt.io KNX device", 16)
            + QKnxByteArray::fromHex("0000000000000000000000000000"));

        QCOMPARE(deviceDib.size(), quint16(54));
        QCOMPARE(deviceDib.bytes(),
            QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
            + QKnxByteArray("qt.io KNX device", 16)
            + QKnxByteArray::fromHex("0000000000000000000000000000"));
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

    qDebug() << QKnxNetIpDeviceDib(QKnx::MediumType::NetIP,
                                   QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                                   QKnxAddress::Individual::Unregistered,
                                   0x1111,
                                   QKnxByteArray::fromHex("123456123456"),
                                   QHostAddress::AnyIPv4,
                                   QKnxByteArray::fromHex("bcaec56690f9"),
                                   QKnxByteArray("qt.io KNX device", 16));
    QCOMPARE(s_msg, QString::fromLatin1("0x36012001ffff111112345612345600000000bcaec56690f9"
            "71742e696f204b4e58206465766963650000000000000000000000000000"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceDib)

#include "tst_qknxnetipdevicedib.moc"
