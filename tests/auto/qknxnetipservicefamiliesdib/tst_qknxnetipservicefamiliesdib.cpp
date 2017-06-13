/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpServiceFamiliesDIB : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArgument();
    void testConstructorWithTwoArguments();
    void testAddFunctions();
    void testGetFunction();
    void testDebugStream();
    void testDataStream();
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
    QCOMPARE(serviceFamiliesDIB.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDIB::testConstructorWithOneArgument()
{
    QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions families;
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 9);
    families.insertMulti(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 1);
    families.insertMulti(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 2);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer, 15);
    QKnxNetIpServiceFamiliesDIB serviceFamiliesDIB(families);

    QCOMPARE(serviceFamiliesDIB.isValid(), true);
    QCOMPARE(serviceFamiliesDIB.size(), quint16(18));
    QCOMPARE(serviceFamiliesDIB.bytes<QByteArray>(),
        QByteArray::fromHex("1202020903010302040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDIB.payload().size(), quint16(16));
    QCOMPARE(serviceFamiliesDIB.payload().bytes<QByteArray>(),
        QByteArray::fromHex("020903010302040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDIB.toString(), QString::fromLatin1("Total size { 0x12 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x01, 0x03, 0x02, 0x04, 0x0b, 0x05, 0x0c, 0x06, "
        "0x0d, 0x07, 0x0e, 0x08, 0x0f }"));
    QCOMPARE(serviceFamiliesDIB.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);
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
    QCOMPARE(families.descriptionType(), QKnxNetIp::DescriptionType::SupportedServiceFamilies);
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
    QCOMPARE(families.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);

    families.add(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 10);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(6));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("06020209030A"));
    QCOMPARE(families.payload().size(), quint16(4));
    QCOMPARE(families.payload().bytes<QByteArray>(),
        QByteArray::fromHex("0209030A"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x06 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a }"));
    QCOMPARE(families.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);

    QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions familiesMap;
    familiesMap.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
    families.add(familiesMap);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(8));
    QCOMPARE(families.bytes<QByteArray>(), QByteArray::fromHex("08020209030A040B"));
    QCOMPARE(families.payload().size(), quint16(6));
    QCOMPARE(families.payload().bytes<QByteArray>(), QByteArray::fromHex("0209030A040B"));
    QCOMPARE(families.toString(), QString::fromLatin1("Total size { 0x08 }, "
        "Code { 0x02 }, Bytes { 0x02, 0x09, 0x03, 0x0a, 0x04, 0x0b }"));
    QCOMPARE(families.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDIB::testGetFunction()
{
    QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions families;
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer, 15);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 9);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 10);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    QKnxNetIpServiceFamiliesDIB serviceFamiliesDIB(families);

    auto map = serviceFamiliesDIB.serviceFamilyIdVersions();
    auto it = std::begin(map);
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core);
    QCOMPARE(it.value(),quint8(9));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement);
    QCOMPARE(it.value(),quint8(10));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling);
    QCOMPARE(it.value(),quint8(11));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting);
    QCOMPARE(it.value(),quint8(12));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging);
    QCOMPARE(it.value(),quint8(13));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis);
    QCOMPARE(it.value(),quint8(14));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer);
    QCOMPARE(it.value(),quint8(15));
}

void tst_QKnxNetIpServiceFamiliesDIB::testDebugStream()
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

    qDebug() << QKnxNetIpServiceFamiliesDIB();
    QCOMPARE(s_msg, QString::fromLatin1("0x0202"));

    QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions families;
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 9);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 10);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer, 15);

    qDebug() << QKnxNetIpServiceFamiliesDIB(families);
    QCOMPARE(s_msg, QString::fromLatin1("0x10020209030a040b050c060d070e080f"));

    qDebug() << QKnxNetIpServiceFamiliesDIB(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 10);
    QCOMPARE(s_msg, QString::fromLatin1("0x0402020a"));
}

void tst_QKnxNetIpServiceFamiliesDIB::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpServiceFamiliesDIB();
        QCOMPARE(byteArray, QByteArray::fromHex("0202"));
    }

    {
        QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions families;
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 9);
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::DeviceManagement, 10);
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 11);
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpRouting, 12);
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteLogging, 13);
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
        families.insert(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::ObjectServer, 15);
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpServiceFamiliesDIB(families);
        QCOMPARE(byteArray, QByteArray::fromHex("10020209030A040B050C060D070E080F"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpServiceFamiliesDIB(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::Core, 10);
        QCOMPARE(byteArray, QByteArray::fromHex("0402020A"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpServiceFamiliesDIB)

#include "tst_qknxnetipservicefamiliesdib.moc"
