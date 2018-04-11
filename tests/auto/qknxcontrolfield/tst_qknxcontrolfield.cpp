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
#include <QtKnx/qknxcontrolfield.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxControlField : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testControlFieldBuilder();
};

void tst_QKnxControlField::testDefaultConstructor()
{
    QKnxControlField crf1(QKnxByteArray { 0xbc });

    QCOMPARE(crf1.frameFormat(), QKnxControlField::FrameFormat::Standard);
    QCOMPARE(crf1.repeat(), QKnxControlField::Repeat::DoNotRepeat);
    QCOMPARE(crf1.broadcast(), QKnxControlField::Broadcast::Domain);

    QKnxExtendedControlField crf2(0xe0);
    QCOMPARE(crf2.hopCount(), quint8(6));
    QCOMPARE(crf2.destinationAddressType(), QKnxAddress::Type::Group);
}

void tst_QKnxControlField::testControlFieldBuilder()
{
    QKnxControlField crf(0xbc);
    QCOMPARE(crf.frameFormat(), QKnxControlField::FrameFormat::Standard);
    QCOMPARE(crf.repeat(), QKnxControlField::Repeat::DoNotRepeat);
    QCOMPARE(crf.broadcast(), QKnxControlField::Broadcast::Domain);
    QCOMPARE(crf.priority(), QKnxControlField::Priority::Low);
    QCOMPARE(crf.acknowledge(), QKnxControlField::Acknowledge::NotRequested);
    QCOMPARE(crf.confirm(), QKnxControlField::Confirm::NoError);
    QCOMPARE(crf.byte() , 0xbc);

    auto field = QKnxControlField::builder().create();
    QCOMPARE(field.byte(), 0xbc);
    QCOMPARE(field.byte(), crf.byte());

    field = QKnxControlField::builder()
        .setPriority(QKnxControlField::Priority::Normal)
        .create();
    QCOMPARE(field.byte(), 0xb4);
    QCOMPARE(field.priority(), QKnxControlField::Priority::Normal);

    auto builder = QKnxControlField::builder();
    field = builder.setFrameFormat(QKnxControlField::FrameFormat::Extended).create();
    QCOMPARE(field.frameFormat(), QKnxControlField::FrameFormat::Extended);

    field = builder.setFrameFormat(QKnxControlField::FrameFormat::Standard).create();
    QCOMPARE(field.frameFormat(), QKnxControlField::FrameFormat::Standard);

    field = builder.setRepeat(QKnxControlField::Repeat::Repeat).create();
    QCOMPARE(field.repeat(), QKnxControlField::Repeat::Repeat);

    field = builder.setRepeat(QKnxControlField::Repeat::DoNotRepeat).create();
    QCOMPARE(field.repeat(), QKnxControlField::Repeat::DoNotRepeat);

    field = builder.setBroadcast(QKnxControlField::Broadcast::System).create();
    QCOMPARE(field.broadcast(), QKnxControlField::Broadcast::System);

    field = builder.setBroadcast(QKnxControlField::Broadcast::Domain).create();
    QCOMPARE(field.broadcast(), QKnxControlField::Broadcast::Domain);

    field = builder.setPriority(QKnxControlField::Priority::System).create();
    QCOMPARE(field.priority(), QKnxControlField::Priority::System);

    field = builder.setPriority(QKnxControlField::Priority::Low).create();
    QCOMPARE(field.priority(), QKnxControlField::Priority::Low);

    field = builder.setAcknowledge(QKnxControlField::Acknowledge::Requested).create();
    QCOMPARE(field.acknowledge(), QKnxControlField::Acknowledge::Requested);

    field = builder.setAcknowledge(QKnxControlField::Acknowledge::NotRequested).create();
    QCOMPARE(field.acknowledge(), QKnxControlField::Acknowledge::NotRequested);

    field = builder.setConfirm(QKnxControlField::Confirm::Error).create();
    QCOMPARE(field.confirm(), QKnxControlField::Confirm::Error);

    field = builder.setConfirm(QKnxControlField::Confirm::NoError).create();
    QCOMPARE(field.confirm(), QKnxControlField::Confirm::NoError);
}

QTEST_APPLESS_MAIN(tst_QKnxControlField)

#include "tst_qknxcontrolfield.moc"
