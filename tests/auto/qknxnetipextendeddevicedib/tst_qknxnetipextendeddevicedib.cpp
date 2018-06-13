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
#include <QtKnx/qknxnetipextendeddevicedib.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}
QT_BEGIN_NAMESPACE

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

QT_END_NAMESPACE
class tst_QKnxNetIpExtendedDeviceDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultDib()
    {
        QKnxNetIpDib dib;

        QKnxNetIpExtendedDeviceDibProxy proxy(dib);
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);

        dib = QKnxNetIpExtendedDeviceDibProxy::builder().create();
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);
    }

    void testValidDib()
    {
        auto infoDib = QKnxNetIpExtendedDeviceDibProxy::builder()
            .setMediumStatus(0x01)
            .setMaximumLocalApduLength(4096)
            .setDeviceDescriptorType0(0x5705)
            .create();
        auto bytes = QKnxByteArray::fromHex("0808010010005705");

        QKnxNetIpExtendedDeviceDibProxy proxy(infoDib);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::ExtendedDeviceInfo);
        QCOMPARE(proxy.mediumStatus(), 0x01);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x1000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x5705);
        QCOMPARE(infoDib.bytes(), bytes);

        bytes = QKnxByteArray::fromHex("080800000400091a");
        infoDib = QKnxNetIpDib::fromBytes(bytes, 0);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::ExtendedDeviceInfo);
        QCOMPARE(proxy.mediumStatus(), 0x00);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0400);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x091a);
    }

    void testInvalidDib()
    {
        QKnxNetIpExtendedDeviceDibProxy::Builder builder;
        auto infoDib = builder
            .create();

        QKnxNetIpExtendedDeviceDibProxy proxy(infoDib);
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);

        infoDib = builder.setMediumStatus(0x01).create();
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);

        infoDib = builder.setMaximumLocalApduLength(4096).create();
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);

        infoDib = builder.setDeviceDescriptorType0(0x5705).create();
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::ExtendedDeviceInfo);
        QCOMPARE(proxy.mediumStatus(), 0x01);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x1000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x5705);
        QCOMPARE(infoDib.bytes(), QKnxByteArray::fromHex("0808010010005705"));

        infoDib = QKnxNetIpDib::fromBytes(QKnxByteArray::fromHex("06080100100057"));
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);

        infoDib = QKnxNetIpDib::fromBytes(QKnxByteArray::fromHex("08080100100057"));
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.mediumStatus(), 0x00u);
        QCOMPARE(proxy.maximumLocalApduLength(), 0x0000);
        QCOMPARE(proxy.deviceDescriptorType0(), 0x0000u);
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

        qDebug() << QKnxNetIpExtendedDeviceDibProxy::builder().create();
        QCOMPARE(s_msg, QString::fromLatin1("0x0208"));

        qDebug() << QKnxNetIpExtendedDeviceDibProxy::builder()
            .setMediumStatus(0x01)
            .setMaximumLocalApduLength(4096)
            .setDeviceDescriptorType0(0x5705)
            .create();
        QCOMPARE(s_msg, QString::fromLatin1("0x0808010010005705"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpExtendedDeviceDib)

#include "tst_qknxnetipextendeddevicedib.moc"
