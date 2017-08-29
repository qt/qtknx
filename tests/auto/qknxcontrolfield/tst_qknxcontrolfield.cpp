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
};

void tst_QKnxControlField::testDefaultConstructor()
{
    QKnxControlField crf1(QByteArray::fromHex("bc"));

    QCOMPARE(crf1.frameType(), QKnxControlField::FrameType::Standard);
    QCOMPARE(crf1.repeat(), QKnxControlField::Repeat::DoNotRepeat);
    QCOMPARE(crf1.broadcast(), QKnxControlField::Broadcast::Domain);

    QKnxExtendedControlField crf2(QByteArray::fromHex("e0"));
    QCOMPARE(crf2.hopCount(), quint8(6));
    qDebug()<<"destination Type"<<quint8(crf2.destinationAddressType());
    QCOMPARE(crf2.destinationAddressType(), QKnxAddress::Type::Group);
}

QTEST_APPLESS_MAIN(tst_QKnxControlField)

#include "tst_qknxcontrolfield.moc"
