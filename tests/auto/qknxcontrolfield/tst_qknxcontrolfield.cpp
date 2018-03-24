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

QTEST_APPLESS_MAIN(tst_QKnxControlField)

#include "tst_qknxcontrolfield.moc"
