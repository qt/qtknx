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
        auto dib = QKnxNetIpDeviceDibProxy::builder()
            .setMediumType(QKnx::MediumType::NetIP)
            .setDeviceStatus(QKnxNetIp::ProgrammingMode::Active)
            .setIndividualAddress(QKnxAddress::Individual::Unregistered)
            .setProjectInstallationId(0x1111)
            .setSerialNumber(QKnxByteArray::fromHex("123456123456"))
            .setMulticastAddress(QHostAddress::AnyIPv4)
            .setMacAddress(QKnxByteArray::fromHex("bcaec56690f9"))
            .setDeviceName(QByteArray("qt.io KNX device"))
            .create();

        const QKnxNetIpDeviceDibProxy deviceDib(dib);
        QCOMPARE(deviceDib.mediumType(), QKnx::MediumType::NetIP);
        QCOMPARE(deviceDib.descriptionType(), QKnxNetIp::DescriptionType::DeviceInfo);
        QCOMPARE(deviceDib.deviceStatus(), QKnxNetIp::ProgrammingMode::Active);
        QCOMPARE(deviceDib.individualAddress().toString(), QKnxAddress::Individual::Unregistered.toString());
        QCOMPARE(deviceDib.projectInstallationId(), quint16(0x1111));
        QCOMPARE(deviceDib.serialNumber(), QKnxByteArray::fromHex("123456123456"));
        QCOMPARE(deviceDib.multicastAddress(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(deviceDib.macAddress(), QKnxByteArray::fromHex("bcaec56690f9"));
        QCOMPARE(deviceDib.deviceName(), QByteArray("qt.io KNX device"));

        QCOMPARE(dib.data().size(), quint16(52));
        QCOMPARE(dib.data(),
            QKnxByteArray::fromHex("2001ffff111112345612345600000000bcaec56690f9")
            + QKnxByteArray("qt.io KNX device", 16)
            + QKnxByteArray::fromHex("0000000000000000000000000000"));

        QCOMPARE(dib.size(), quint16(54));
        QCOMPARE(dib.bytes(),
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

    qDebug() << QKnxNetIpDeviceDibProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0201"));

    qDebug() << QKnxNetIpDeviceDibProxy::builder()
        .setMediumType(QKnx::MediumType::NetIP)
        .setDeviceStatus(QKnxNetIp::ProgrammingMode::Active)
        .setIndividualAddress(QKnxAddress::Individual::Unregistered)
        .setProjectInstallationId(0x1111)
        .setSerialNumber(QKnxByteArray::fromHex("123456123456"))
        .setMulticastAddress(QHostAddress::AnyIPv4)
        .setMacAddress(QKnxByteArray::fromHex("bcaec56690f9"))
        .setDeviceName(QByteArray("qt.io KNX device"))
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x36012001ffff111112345612345600000000bcaec56690f9"
            "71742e696f204b4e58206465766963650000000000000000000000000000"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceDib)

#include "tst_qknxnetipdevicedib.moc"
