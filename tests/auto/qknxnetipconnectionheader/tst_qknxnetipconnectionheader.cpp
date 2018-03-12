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

#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpConnectionHeader : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor()
    {
        QKnxNetIpConnectionHeader header;
        QCOMPARE(header.isNull(), true);
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.channelId(), quint8(0));
        QCOMPARE(header.sequenceNumber(), quint8(0));
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(0));
        QCOMPARE(header.connectionTypeSpecificHeaderItems(), QKnxByteArray {});
        QCOMPARE(header.toString(), QStringLiteral("Size { 0x00 }"
            ", Communication channel ID { 0x00 }, Sequence number { 0x00 }"
            ", Service type specific value { 0x00 }, Connection type specific header items {  }"));
        QCOMPARE(header.size(), quint8(0));
        QCOMPARE(header.bytes(), QKnxByteArray {});
    }

    void testConstructorWithArguments()
    {
        QKnxNetIpConnectionHeader header(1, 2, 3);
        QCOMPARE(header.isNull(), false);
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.channelId(), quint8(1));
        QCOMPARE(header.sequenceNumber(), quint8(2));
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(3));
        QCOMPARE(header.connectionTypeSpecificHeaderItems(), QKnxByteArray {});
        QCOMPARE(header.toString(), QStringLiteral("Size { 0x04 }"
            ", Communication channel ID { 0x01 }, Sequence number { 0x02 }"
            ", Service type specific value { 0x03 }, Connection type specific header items {  }"));
        QCOMPARE(header.size(), quint8(4));
        QCOMPARE(header.bytes(), QKnxByteArray({ 0x04, 0x01, 0x02, 0x03 }));
    }

    void testSetterGetter()
    {
        QKnxNetIpConnectionHeader header;
        QCOMPARE(header.isNull(), true);
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.size(), quint8(0));

        header.setChannelId(1);
        QCOMPARE(header.channelId(), quint8(1));
        QCOMPARE(header.isValid(), true);

        header = {};
        QCOMPARE(header.isNull(), true);
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.size(), quint8(0));

        header.setSequenceNumber(2);
        QCOMPARE(header.sequenceNumber(), quint8(2));
        QCOMPARE(header.isValid(), true);

        header = {};
        QCOMPARE(header.isNull(), true);
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.size(), quint8(0));

        header.setServiceTypeSpecificValue(3);
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(3));
        QCOMPARE(header.isValid(), true);

        header = {};
        QCOMPARE(header.isNull(), true);
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.size(), quint8(0));

        header.setConnectionTypeSpecificHeaderItems(QKnxByteArray { 0x01, 0x02, 0x03, 0x04, 0x05 });
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.size(), quint8(9));
    }

    void testFromBytes()
    {
        QKnxNetIpConnectionHeader header(1, 2);
        header.setConnectionTypeSpecificHeaderItems(QKnxByteArray { 0x01, 0x02, 0x03, 0x04, 0x05 });

        auto headerBytes = header.bytes();
        auto headerFromBytes = QKnxNetIpConnectionHeader::fromBytes(headerBytes, 0);

        QCOMPARE(headerFromBytes.isValid(), true);
        QCOMPARE(headerFromBytes.channelId(), quint8(1));
        QCOMPARE(headerFromBytes.sequenceNumber(), quint8(2));
        QCOMPARE(headerFromBytes.serviceTypeSpecificValue(), quint8(0));
        QCOMPARE(headerFromBytes.connectionTypeSpecificHeaderItems(),
            QKnxByteArray({ 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(headerFromBytes.bytes(), headerBytes);

        auto headerBytesStream = QKnxByteArray { 0x01, 0x02, 0x03, 0x04, 0x05 } + header.bytes()
            + QKnxByteArray { 0x06, 0x07, 0x08, 0x09, 0xaa };
        headerFromBytes = QKnxNetIpConnectionHeader::fromBytes(headerBytesStream, 5);

        QCOMPARE(headerFromBytes.isValid(), true);
        QCOMPARE(headerFromBytes.channelId(), quint8(1));
        QCOMPARE(headerFromBytes.sequenceNumber(), quint8(2));
        QCOMPARE(headerFromBytes.serviceTypeSpecificValue(), quint8(0));
        QCOMPARE(headerFromBytes.connectionTypeSpecificHeaderItems(),
            QKnxByteArray({ 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(headerFromBytes.bytes(), headerBytes);
    }

    void testDebugStream()
    {
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectionHeader)

#include "tst_qknxnetipconnectionheader.moc"
