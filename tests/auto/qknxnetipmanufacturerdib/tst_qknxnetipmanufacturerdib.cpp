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
#include <QtKnx/qknxnetipmanufacturerdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpManufacturerDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArguments();
    void testConstructorWithByteArrayDataArguments();
    void testConstructorWithVectorDataArguments();
    void testDebugStream();
};

void tst_QKnxNetIpManufacturerDib::testDefaultConstructor()
{
    auto manufacturerDib = QKnxNetIpManufacturerDibView::builder().create();
    QCOMPARE(manufacturerDib.isValid(), true);
    QCOMPARE(manufacturerDib.size(), quint16(4));
    QCOMPARE(manufacturerDib.bytes(), QKnxByteArray::fromHex("04fe0000"));
    QCOMPARE(manufacturerDib.data().size(), quint16(2));
    QCOMPARE(manufacturerDib.data(), QKnxByteArray::fromHex("0000"));

    QKnxNetIpManufacturerDibView view(manufacturerDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::ManufacturerData);
    QCOMPARE(view.manufacturerId(), quint16(0));
}

void tst_QKnxNetIpManufacturerDib::testConstructorWithOneArguments()
{
    auto manufacturerDib = QKnxNetIpManufacturerDibView::builder()
        .setManufacturerId(65535).create();
    QCOMPARE(manufacturerDib.isValid(), true);
    QCOMPARE(manufacturerDib.size(), quint16(4));
    QCOMPARE(manufacturerDib.bytes(), QKnxByteArray::fromHex("04FEFFFF"));
    QCOMPARE(manufacturerDib.data().size(), quint16(2));
    QCOMPARE(manufacturerDib.data(), QKnxByteArray::fromHex("FFFF"));

    QKnxNetIpManufacturerDibView view(manufacturerDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::ManufacturerData);
    QCOMPARE(view.manufacturerId(), quint16(65535));
    QCOMPARE(view.manufacturerData(), QKnxByteArray {});
}

void tst_QKnxNetIpManufacturerDib::testConstructorWithByteArrayDataArguments()
{
    const auto data = QKnxByteArray::fromHex("0102030405");
    const auto evenData = QKnxByteArray::fromHex("010203040500");
    auto manufacturerDib = QKnxNetIpManufacturerDibView::builder().setManufacturerId(65535)
        .setManufacturerData(data).create();
    QCOMPARE(manufacturerDib.isValid(), true);
    QCOMPARE(manufacturerDib.size(), quint16(10));
    QCOMPARE(manufacturerDib.bytes(), QKnxByteArray::fromHex("0aFEFFFF010203040500"));
    QCOMPARE(manufacturerDib.data().size(), quint16(8));
    QCOMPARE(manufacturerDib.data(), QKnxByteArray::fromHex("FFFF010203040500"));

    QKnxNetIpManufacturerDibView view(manufacturerDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::ManufacturerData);
    QCOMPARE(view.manufacturerId(), quint16(65535));
    QCOMPARE(view.manufacturerData(), evenData);
    QCOMPARE(view.manufacturerData().size(), evenData.size());
}

void tst_QKnxNetIpManufacturerDib::testConstructorWithVectorDataArguments()
{
    const QKnxByteArray data = { 1, 2, 3, 4, 5 };
    const QKnxByteArray evenData = { 1, 2, 3, 4, 5, 0 };
    auto manufacturerDib = QKnxNetIpManufacturerDibView::builder()
        .setManufacturerId(65535)
        .setManufacturerData(data)
        .create();
    QCOMPARE(manufacturerDib.isValid(), true);
    QCOMPARE(manufacturerDib.size(), quint16(10));
    QCOMPARE(manufacturerDib.bytes(), QKnxByteArray::fromHex("0aFEFFFF010203040500"));
    QCOMPARE(manufacturerDib.data().size(), quint16(8));
    QCOMPARE(manufacturerDib.data(), QKnxByteArray::fromHex("FFFF010203040500"));

    QKnxNetIpManufacturerDibView view(manufacturerDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::ManufacturerData);
    QCOMPARE(view.manufacturerId(), quint16(65535));
    QCOMPARE(view.manufacturerData(), evenData);
    QCOMPARE(view.manufacturerData().size(), evenData.size());
}

void tst_QKnxNetIpManufacturerDib::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
        {}
        ~DebugHandler()
        {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpManufacturerDibView::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x04fe0000"));

    qDebug() << QKnxNetIpManufacturerDibView::builder().setManufacturerId(65535).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x04feffff"));

    qDebug() << QKnxNetIpManufacturerDibView::builder().setManufacturerId(65535)
        .setManufacturerData(QKnxByteArray::fromHex("0102030405")).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0afeffff010203040500"));

    qDebug() << QKnxNetIpManufacturerDibView::builder().setManufacturerId(65535)
        .setManufacturerData({ 1, 2, 3, 4, 5 }).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0afeffff010203040500"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpManufacturerDib)

#include "tst_qknxnetipmanufacturerdib.moc"
