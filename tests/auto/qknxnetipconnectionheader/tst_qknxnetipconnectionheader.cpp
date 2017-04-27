/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpConnectionHeader : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor()
    {
        QKnxNetIpConnectionHeader header;
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.communicationChannelId(), quint8(0));
        QCOMPARE(header.sequenceCounter(), quint8(0));
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(0));
        QCOMPARE(header.connectionTypeSpecificHeaderItems(), std::vector<quint8> {});
        QCOMPARE(header.toString(), QStringLiteral("Size { 0x00 }"
            ", Communication channel ID { 0x00 }, Sequence counter { 0x00 }"
            ", Service type specific value { 0x00 }, Connection type specific header items {  }"));
        QCOMPARE(header.size(), quint16(0));
        QCOMPARE(header.bytes(), std::vector<quint8> {});
    }

    void testConstructorWithArguments()
    {
        QKnxNetIpConnectionHeader header(1, 2, 3);
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.communicationChannelId(), quint8(1));
        QCOMPARE(header.sequenceCounter(), quint8(2));
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(3));
        QCOMPARE(header.connectionTypeSpecificHeaderItems(), std::vector<quint8> {});
        QCOMPARE(header.toString(), QStringLiteral("Size { 0x04 }"
            ", Communication channel ID { 0x01 }, Sequence counter { 0x02 }"
            ", Service type specific value { 0x03 }, Connection type specific header items {  }"));
        QCOMPARE(header.size(), quint16(4));
        QCOMPARE(header.bytes(), std::vector<quint8> ({ 4, 1, 2, 3}));
    }

    void testSetterGetter()
    {
        QKnxNetIpConnectionHeader header;
        QCOMPARE(header.isValid(), false);

        header.setCommunicationChannelId(1);
        QCOMPARE(header.communicationChannelId(), quint8(1));
        QCOMPARE(header.isValid(), false);

        header.setSequenceCounter(2);
        QCOMPARE(header.sequenceCounter(), quint8(2));
        QCOMPARE(header.isValid(), false);

        header.setServiceTypeSpecificValue(3);
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(3));
        QCOMPARE(header.isValid(), true);

        header = QKnxNetIpConnectionHeader();
        QCOMPARE(header.isValid(), false);

        header.setConnectionTypeSpecificHeaderItems(std::vector<quint8> { 1, 2, 3, 4, 5 });
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.size(), quint16(9));

        header.setServiceTypeSpecificValue(3);
        QCOMPARE(header.serviceTypeSpecificValue(), quint8(3));
        QCOMPARE(header.isValid(), false);

        header.setSequenceCounter(2);
        QCOMPARE(header.sequenceCounter(), quint8(2));
        QCOMPARE(header.isValid(), false);

        header.setCommunicationChannelId(1);
        QCOMPARE(header.communicationChannelId(), quint8(1));
        QCOMPARE(header.isValid(), true);
    }

    void testFromBytes()
    {
        QKnxNetIpConnectionHeader header(1, 2);
        header.setConnectionTypeSpecificHeaderItems(std::vector<quint8> { 1, 2, 3, 4, 5 });

        auto headerBytes = header.bytes();
        auto headerFromBytes = QKnxNetIpConnectionHeader::fromBytes(headerBytes, 0);

        QCOMPARE(headerFromBytes.isValid(), true);
        QCOMPARE(headerFromBytes.communicationChannelId(), quint8(1));
        QCOMPARE(headerFromBytes.sequenceCounter(), quint8(2));
        QCOMPARE(headerFromBytes.serviceTypeSpecificValue(), quint8(0));
        QCOMPARE(headerFromBytes.connectionTypeSpecificHeaderItems(),
            std::vector<quint8> ({ 1, 2, 3, 4, 5 }));
        QCOMPARE(headerFromBytes.bytes(), headerBytes);

        auto headerBytesStream = QByteArray::fromHex("0102030405") + header.bytes<QByteArray>()
            + QByteArray::fromHex("06070809aa");
        headerFromBytes = QKnxNetIpConnectionHeader::fromBytes(headerBytesStream, 5);

        QCOMPARE(headerFromBytes.isValid(), true);
        QCOMPARE(headerFromBytes.communicationChannelId(), quint8(1));
        QCOMPARE(headerFromBytes.sequenceCounter(), quint8(2));
        QCOMPARE(headerFromBytes.serviceTypeSpecificValue(), quint8(0));
        QCOMPARE(headerFromBytes.connectionTypeSpecificHeaderItems(),
            std::vector<quint8> ({ 1, 2, 3, 4, 5 }));
        QCOMPARE(headerFromBytes.bytes(), headerBytes);
    }

    void testDebugStream()
    {
    }

    void testDataStream()
    {
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectionHeader)

#include "tst_qknxnetipconnectionheader.moc"
