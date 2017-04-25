/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpServiceFamiliesDIB : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArgument();
    void testConstructorWithTwoArguments();
    void testConstructorWithThreeArguments();
    void testAddFunctions();
    void testDebugStream()
    {
        // TODO: Implement.
    }
    void testDataStream()
    {
        // TODO: Implement.
    }
};

void tst_QKnxNetIpServiceFamiliesDIB::testDefaultConstructor()
{
    QKnxNetIpServiceFamiliesDIB serviceFamiliesDIB;
    QCOMPARE(serviceFamiliesDIB.isValid(), true);
    QCOMPARE(serviceFamiliesDIB.size(), quint16(2));
    QCOMPARE(serviceFamiliesDIB.bytes<QByteArray>(), QByteArray::fromHex("0202"));
    QCOMPARE(serviceFamiliesDIB.payload().size(), quint16(0));
    QCOMPARE(serviceFamiliesDIB.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(serviceFamiliesDIB.toString(), QString::fromLatin1("Total size { 0x02 }, "
        "Code { 0x02 }, Bytes {  }"));
    QCOMPARE(serviceFamiliesDIB.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDIB::testConstructorWithOneArgument()
{
    QMap<QKnxNetIpServiceFamiliesDIB::ServiceFamilieId, quint8> families;
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 9);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 10);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer, 15);
    QKnxNetIpServiceFamiliesDIB serviceFamiliesDIB(families);

    QCOMPARE(serviceFamiliesDIB.isValid(), true);
    QCOMPARE(serviceFamiliesDIB.size(), quint16(16));
    QCOMPARE(serviceFamiliesDIB.bytes<QByteArray>(),
        QByteArray::fromHex("10020209030A040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDIB.payload().size(), quint16(14));
    QCOMPARE(serviceFamiliesDIB.payload().bytes<QByteArray>(),
        QByteArray::fromHex("0209030A040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDIB.toString(), QString::fromLatin1("Total size { 0x10 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a, 0x04, 0x0b, 0x05, 0x0c, 0x06, 0x0d, "
        "0x07, 0x0e, 0x08, 0x0f }"));
    QCOMPARE(serviceFamiliesDIB.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDIB::testConstructorWithTwoArguments()
{
    QKnxNetIpServiceFamiliesDIB families(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 10);

    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(4));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("0402020A"));
    QCOMPARE(families.size(), quint16(4));
    QCOMPARE(families.payload().bytes<QByteArray>(), QByteArray::fromHex("020A"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x04 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x0a }"));
    QCOMPARE(families.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);

}

void tst_QKnxNetIpServiceFamiliesDIB::testConstructorWithThreeArguments()
{
    QVector<QKnxNetIpServiceFamiliesDIB::ServiceFamilieId> ids;
    ids.append(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core);
    ids.append(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement);
    ids.append(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling);
    ids.append(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting);

    const QVector<quint8> versions = { 9, 10, 11, 12 };
    QKnxNetIpServiceFamiliesDIB families(ids, versions);

    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(10));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("0a020209030A040B050C"));
    QCOMPARE(families.payload().size(), quint16(8));
    QCOMPARE(families.payload().bytes<QByteArray>(), QByteArray::fromHex("0209030A040B050C"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x0a }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a, 0x04, 0x0b, 0x05, 0x0c }"));
    QCOMPARE(families.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDIB::testAddFunctions()
{
    QKnxNetIpServiceFamiliesDIB families(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 9);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(04));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("04020209"));
    QCOMPARE(families.payload().size(), quint16(2));
    QCOMPARE(families.payload().bytes<QByteArray>(), QByteArray::fromHex("0209"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x04 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09 }"));
    QCOMPARE(families.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);

    families.add(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 10);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(6));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("06020209030A"));
    QCOMPARE(families.payload().size(), quint16(4));
    QCOMPARE(families.payload().bytes<QByteArray>(),
        QByteArray::fromHex("0209030A"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x06 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a }"));
    QCOMPARE(families.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);

    QMap<QKnxNetIpServiceFamiliesDIB::ServiceFamilieId, quint8> familiesMap;
    familiesMap.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
    families.add(familiesMap);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(8));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("08020209030A040B"));
    QCOMPARE(families.payload().size(), quint16(6));
    QCOMPARE(families.payload().bytes<QByteArray>(), QByteArray::fromHex("0209030A040B"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x08 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a, 0x04, 0x0b }"));
    QCOMPARE(families.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);

    QVector<QKnxNetIpServiceFamiliesDIB::ServiceFamilieId> ids;
    ids.append(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting);
    const QVector<quint8> versions = { { 12 } };
    families.add(ids, versions);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(10));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("0a020209030A040B050C"));
    QCOMPARE(families.payload().size(), quint16(8));
    QCOMPARE(families.payload().bytes<QByteArray>(), QByteArray::fromHex("0209030A040B050C"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x0a }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a, 0x04, 0x0b, 0x05, 0x0c }"));
    QCOMPARE(families.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies);
}
QTEST_APPLESS_MAIN(tst_QKnxNetIpServiceFamiliesDIB)

#include "tst_qknxnetipservicefamiliesdib.moc"
