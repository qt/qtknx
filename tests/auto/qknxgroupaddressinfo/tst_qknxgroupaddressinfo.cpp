/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
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
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtKnx/qknxgroupaddressinfos.h>
#include <QtKnx/qknxgroupaddressinfo.h>
#include <QtTest/qtest.h>

class tst_QKnxGroupAddressInfos : public QObject
{
    Q_OBJECT

private slots:
    void groupAddressInfo();
    void groupAddressInfosFromXml();
    void groupAddressInfosFromZip();

private:
    QVector<QKnxGroupAddressInfo> initGroupAddressInfos(const QString &install = {});
};

QVector<QKnxGroupAddressInfo> tst_QKnxGroupAddressInfos::initGroupAddressInfos(const QString &i)
{
    return QVector<QKnxGroupAddressInfo> {
        { i, "Living room Ceiling light switching", 0x0900, "DPST-1-1", "Living room Ceiling light" },
        { i, "Living room Desk light switching", 0x0901, "DPST-1-1", "Living room Desk light" },
        { i, "Living room Socket switching", 0x0902, "DPST-1-1", "Living room Socket" },
        { i, "Kitchen Ceiling light switching", 0x0908, "DPST-1-1", "Kitchen Ceiling light" },
        { i, "Kitchen Working light switching", 0x0909, "DPST-1-1", "Kitchen Working light" },
        { i, "Kitchen Dining corner switching", 0x090a, "DPST-1-1", "Kitchen Dining corner" },
        { i, "Bedroom Ceiling light switching", 0x0910, "DPST-1-1", "Bedroom Ceiling light" },
        { i, "Bedroom Bed left switching", 0x0911, "DPST-1-1", "Bedroom Bed left" },
        { i, "Bedroom Bed right switching", 0x0912, "DPST-1-1", "Bedroom Bed right" },
        { i, "Bath room Ceiling light switching", 0x0918, "DPST-1-1", "Bath room Ceiling light" },
        { i, "Bath room Mirror switching", 0x0919, "DPST-1-1", "Bath room Mirror" },
        { i, "Corridor Ceiling light switching", 0x091f, "DPST-1-1", "Corridor Ceiling light" },
        { i, "Terrace/Balcony Wall light switching", 0x0920, "DPST-1-1", "Terrace/Balcony Wall light" },
        { i, "Living room Ceiling light status", 0x0c00, "DPST-1-1", "Living room Ceiling light" },
        { i, "Living room Desk light status", 0x0c01, "DPST-1-1", "Living room Desk light" },
        { i, "Living room Socket status", 0x0c02, "DPST-1-1", "Living room Socket" },
        { i, "Kitchen Ceiling light status", 0x0c08, "DPST-1-1", "Kitchen Ceiling light" },
        { i, "Kitchen Working light status", 0x0c09, "DPST-1-1", "Kitchen Working light" },
        { i, "Kitchen Dining corner status", 0x0c0a, "DPST-1-1", "Kitchen Dining corner" },
        { i, "Bedroom Ceiling light status", 0x0c10, "DPST-1-1", "Bedroom Ceiling light" },
        { i, "Bedroom Bed left status", 0x0c11, "DPST-1-1", "Bedroom Bed left" },
        { i, "Bedroom Bed right status", 0x0c12, "DPST-1-1", "Bedroom Bed right" },
        { i, "Bath room Ceiling light status", 0x0c18, "DPST-1-1", "Bath room Ceiling light" },
        { i, "Bath room Mirror status", 0x0c19, "DPST-1-1", "Bath room Mirror" },
        { i, "Corridor Ceiling light status", 0x0c1f, "DPST-1-1", "Corridor Ceiling light" },
        { i, "Terrace/Balcony Wall light status", 0x0c20, "DPST-1-1", "Terrace/Balcony Wall light" },
        { i, "Living room Ceiling light dimming", 0x0a00, "DPST-3-7", "Living room Ceiling light" },
        { i, "Living room Desk light dimming", 0x0a01, "DPST-3-7", "Living room Desk light" },
        { i, "Kitchen Ceiling light dimming", 0x0a07, "DPST-3-7", "Kitchen Ceiling light" },
        { i, "Bedroom Ceiling light dimming", 0x0a0d, "DPST-3-7", "Bedroom Ceiling light" },
        { i, "Bath room Ceiling light dimming", 0x0a13, "DPST-3-7", "Bath room Ceiling light" },
        { i, "Living room Ceiling light value", 0x0b00, "DPST-5-1", "Living room Ceiling light" },
        { i, "Living room Desk light value", 0x0b01, "DPST-5-1", "Living room Desk light" },
        { i, "Kitchen Ceiling light value", 0x0b07, "DPST-5-1", "Kitchen Ceiling light" },
        { i, "Bedroom Ceiling light value", 0x0b0d, "DPST-5-1", "Bedroom Ceiling light" },
        { i, "Bath room Ceiling light value", 0x0b13, "DPST-5-1", "Bath room Ceiling light" },
        { i, "Switch light central", 0x0800, "DPST-1-1", "Apartment Central light" },
        { i, "Dawn switch", 0x0801, "", "" },
        { i, "Dimming light central", 0x0802, "", "Apartment Central light" },
        { i, "Value light central", 0x0803, "", "Apartment Central light" },
        { i, "Living room Light room switch", 0x080a, "DPST-1-1", "Living room Light room switch" },
        { i, "Living room Light room dimming", 0x080b, "", "Living room Light room switch" },
        { i, "Living room Light room value", 0x080c, "", "Living room Light room switch" },
        { i, "Kitchen Light room switch", 0x080d, "DPST-1-1", "Kitchen Light room switch" },
        { i, "Bedroom Light room switch", 0x080e, "DPST-1-1", "Bedroom Light room switch" },
        { i, "Bath room Light room switch", 0x080f, "DPST-1-1", "Bath room Light room switch" },
        { i, "Wind alert", 0x1001, "DPST-1-5", "Living room Window 1" },
        { i, "Rain alert", 0x1002, "DPST-1-5", "Living room Window 1" },
        { i, "Central movement", 0x1000, "DPST-1-8", "Apartment Central movement" },
        { i, "Central step/stop", 0x1003, "DPST-1-7", "Apartment Central movement" },
        { i, "Living room Room movement", 0x100a, "DPST-1-8", "Living room Room movement" },
        { i, "Living room Room step/stop", 0x100b, "DPST-1-7", "Living room Room movement" },
        { i, "Kitchen Room movement", 0x100c, "DPST-1-8", "Kitchen Room movement" },
        { i, "Kitchen Room step/stop", 0x100d, "DPST-1-7", "Kitchen Room movement" },
        { i, "Living room Window 1 movement", 0x1100, "DPST-1-8", "Living room Window 1" },
        { i, "Living room Window 2 movement", 0x1101, "DPST-1-8", "Living room Window 2" },
        { i, "Kitchen Window 1 movement", 0x1107, "DPST-1-8", "Kitchen Window 1" },
        { i, "Kitchen Window 2 movement", 0x1108, "DPST-1-8", "Kitchen Window 2" },
        { i, "Bedroom Window 1 movement", 0x110e, "DPST-1-8", "Bedroom Window 1" },
        { i, "Bath room Window movement", 0x110f, "DPST-1-8", "Bath room Window" },
        { i, "Terrace/Balcony Blind movement", 0x1110, "DPST-1-8", "Terrace/Balcony Blind" },
        { i, "Living room Window 1 step/stop", 0x1200, "DPST-1-7", "Living room Window 1" },
        { i, "Living room Window 2 step/stop", 0x1201, "DPST-1-7", "Living room Window 2" },
        { i, "Kitchen Window 1 step/stop", 0x1207, "DPST-1-7", "Kitchen Window 1" },
        { i, "Kitchen Window 2 step/stop", 0x1208, "DPST-1-7", "Kitchen Window 2" },
        { i, "Bedroom Window 1 step/stop", 0x120e, "DPST-1-7", "Bedroom Window 1" },
        { i, "Bath room Window step/stop", 0x120f, "DPST-1-7", "Bath room Window" },
        { i, "Terrace/Balcony Blind step/stop", 0x1210, "DPST-1-7", "Terrace/Balcony Blind" },
        { i, "Living room Floor variable", 0x1b00, "", "Living room Floor" },
        { i, "Kitchen Floor variable", 0x1b01, "", "Kitchen Floor" },
        { i, "Bedroom Floor variable", 0x1b02, "", "Bedroom Floor" },
        { i, "Bath room Radiator variable", 0x1b03, "", "Bath room Radiator" },
        { i, "Bath room Floor variable", 0x1b04, "", "Bath room Floor" },
        { i, "Corridor Floor variable", 0x1b0a, "", "Corridor Floor" },
        { i, "Living room heating current temperature", 0x1900, "DPST-9-1", "Living room Floor" },
        { i, "Kitchen heating current temperature", 0x1901, "DPST-9-1", "Kitchen Floor" },
        { i, "Bedroom heating current temperature", 0x1902, "DPST-9-1", "Bedroom Floor" },
        { i, "Bath room heating current temperature", 0x1903, "DPST-9-1", "Bath room Floor" },
        { i, "Corridor heating current temperature", 0x1904, "DPST-9-1", "Corridor Floor" },
        { i, "Living room heating setpoint temperature", 0x1a00, "DPST-9-1", "Living room Floor" },
        { i, "Kitchen heating setpoint temperature", 0x1a01, "DPST-9-1", "Kitchen Floor" },
        { i, "Bedroom heating setpoint temperature", 0x1a02, "DPST-9-1", "Bedroom Floor" },
        { i, "Bath room heating setpoint temperature", 0x1a03, "DPST-9-1", "Bath room Floor" },
        { i, "Corridor heating setpoint temperature", 0x1a04, "DPST-9-1", "Corridor Floor" },
        { i, "Living room heating operation mode", 0x1c00, "DPST-20-102", "Living room Floor" },
        { i, "Kitchen heating operation mode", 0x1c01, "DPST-20-102", "Kitchen Floor" },
        { i, "Bedroom heating operation mode", 0x1c02, "DPST-20-102", "Bedroom Floor" },
        { i, "Bath room heating operation mode", 0x1c03, "DPST-20-102", "Bath room Floor" },
        { i, "Corridor heating operation mode", 0x1c04, "DPST-20-102", "Corridor Floor" },
        { i, "Living room heating window contact", 0x1d00, "DPST-1-19", "Living room Floor" },
        { i, "Kitchen heating window contact", 0x1d01, "DPST-1-19", "Kitchen Floor" },
        { i, "Bedroom heating window contact", 0x1d02, "DPST-1-19", "Bedroom Floor" },
        { i, "Bath room heating window contact", 0x1d03, "DPST-1-19", "Bath room Floor" },
        { i, "Corridor heating window contact", 0x1d04, "DPST-1-19", "Corridor Floor" },
        { i, "Central operation mode switch", 0x1800, "DPST-20-102", "Apartment Central operation mode switch" }
    };
}

void tst_QKnxGroupAddressInfos::groupAddressInfo()
{
    QKnxGroupAddressInfo info;
    QCOMPARE(info.isValid(), false);
    QCOMPARE(info.installation(), QString());
    QCOMPARE(info.name(), QString());
    QCOMPARE(info.address(), QKnxAddress());
    QCOMPARE(info.datapointType(), QKnxDatapointType::Type::Unknown);
    QCOMPARE(info.description(), QString());

    info.setAddress(QKnxAddress::Individual::Unregistered);
    QCOMPARE(info.address(), QKnxAddress::Individual::Unregistered);
    QCOMPARE(info.isValid(), false);

    info.setName("Name");
    QCOMPARE(info.name(), QString("Name"));
    QCOMPARE(info.isValid(), false);

    info.setDatapointType(QKnxDatapointType::Type::DptSwitch);
    QCOMPARE(info.datapointType(), QKnxDatapointType::Type::DptSwitch);
    QCOMPARE(info.isValid(), false);

    info.setDescription("Description");
    QCOMPARE(info.description(), QString("Description"));
    QCOMPARE(info.isValid(), false);

    info.setInstallation("Installation");
    QCOMPARE(info.installation(), QString("Installation"));
    QCOMPARE(info.isValid(), false);

    info.setAddress(QKnxAddress::Group::Broadcast);
    QCOMPARE(info.address(), QKnxAddress::Group::Broadcast);
    QCOMPARE(info.isValid(), true);

    QKnxGroupAddressInfo info2 = { QString("Installation"), QString("Name"), 0x0000,
        QKnxDatapointType::Type::DptSwitch, QString("Description") };
    QCOMPARE(info2, info);

    info2 = { QString("Installation"), QString("Name"), 0x0000, QString("DPST-1-1"),
        QString("Description") };
    QCOMPARE(info2, info);

    info2 = { QString("Installation"), QString("Name"), QKnxAddress::Group::Broadcast,
        QKnxDatapointType::Type::DptSwitch, QString("Description") };
    QCOMPARE(info2, info);

    info2 = { QString("Installation"), QString("Name"), QKnxAddress::Group::Broadcast,
        QString("DPST-1-1"), QString("Description") };
    QCOMPARE(info2, info);
}

void tst_QKnxGroupAddressInfos::groupAddressInfosFromXml()
{
    QKnxGroupAddressInfos infos(QString(":/data/0.xml"));

    QCOMPARE(infos.projectIds(), QVector<QString>());
    QCOMPARE(infos.projectFile(), QString(":/data/0.xml"));
    QCOMPARE(infos.status(), QKnxGroupAddressInfos::Status::NoError);
    QCOMPARE(infos.errorString(), QString());

    QCOMPARE(infos.parse(), true);
    QCOMPARE(infos.projectName(QString("P-03D8")), QString());
    QCOMPARE(infos.projectName(QString("P-03D9")), QString());
    QVERIFY2(infos.projectIds().contains(QString("P-03D8")), "P-03D8" );
    QVERIFY2(infos.projectIds().contains(QString("P-03D9")), "P-03D9" );

    auto installations = infos.installations(QString("P-03D8"));
    QCOMPARE(installations.count(), 2);

    QCOMPARE(installations.indexOf(QString("First")) >= 0, true);
    QCOMPARE(infos.infoCount(QString("P-03D8"), QString("First")), 95);
    QCOMPARE(infos.addressInfos(QString("P-03D8"), QString("First")).size(), 95);

    auto groupAddressInfos = initGroupAddressInfos(QString("First"));
    auto entries = infos.addressInfos(QString("P-03D8"), QString("First"));
    for (const auto &entry : qAsConst(groupAddressInfos))
        QVERIFY2(entries.contains(entry), entry.name().toLatin1());

    QCOMPARE(installations.indexOf(QString("Second")) >= 0, true);
    QCOMPARE(infos.infoCount(QString("P-03D8"), QString("Second")), 95);
    QCOMPARE(infos.addressInfos(QString("P-03D8"), QString("Second")).size(), 95);

    groupAddressInfos = initGroupAddressInfos(QString("Second"));
    entries = infos.addressInfos(QString("P-03D8"), QString("Second"));
    for (const auto &entry : qAsConst(groupAddressInfos))
        QVERIFY2(entries.contains(entry), entry.name().toLatin1());

    installations = infos.installations(QString("P-03D9"));
    QCOMPARE(installations.count(), 2);

    QCOMPARE(installations.indexOf(QString("Third")) >= 0, true);
    QCOMPARE(infos.infoCount(QString("P-03D9"), QString("Third")), 95);
    QCOMPARE(infos.addressInfos(QString("P-03D9"), QString("Third")).size(), 95);

    groupAddressInfos = initGroupAddressInfos(QString("Third"));
    entries = infos.addressInfos(QString("P-03D9"), QString("Third"));
    for (const auto &entry : qAsConst(groupAddressInfos))
        QVERIFY2(entries.contains(entry), entry.name().toLatin1());

    QCOMPARE(installations.indexOf(QString("Fourth")) >= 0, true);
    QCOMPARE(infos.infoCount(QString("P-03D9"), QString("Fourth")), 95);
    QCOMPARE(infos.addressInfos(QString("P-03D9"), QString("Fourth")).size(), 95);

    groupAddressInfos = initGroupAddressInfos(QString("Fourth"));
    entries = infos.addressInfos(QString("P-03D9"), QString("Fourth"));
    for (const auto &entry : qAsConst(groupAddressInfos))
        QVERIFY2(entries.contains(entry), entry.name().toLatin1());

    entries = infos.addressInfos(QString("P-03D9"), QString("Fifth"));
    QCOMPARE(entries.size(), 0); // wrong installation name

    entries = infos.addressInfos(QString("P-03D5"), QString("Fourth"));
    QCOMPARE(entries.size(), 0); // wrong project id
}

void tst_QKnxGroupAddressInfos::groupAddressInfosFromZip()
{
    QKnxGroupAddressInfos infos;
    QCOMPARE(infos.projectIds(), QVector<QString>());
    QCOMPARE(infos.projectFile(), QString());
    QCOMPARE(infos.status(), QKnxGroupAddressInfos::Status::NoError);
    QCOMPARE(infos.errorString(), QString());

    QCOMPARE(infos.parse(), false);
    QCOMPARE(infos.status(), QKnxGroupAddressInfos::Status::FileError);
    QCOMPARE(infos.errorString(), QString("No file name specified"));

    infos.clear();
    QCOMPARE(infos.status(), QKnxGroupAddressInfos::Status::NoError);
    QCOMPARE(infos.errorString(), QString());

    infos.setProjectFile(QStringLiteral(":/data/nofile.xml"));
    QCOMPARE(infos.projectFile(), QStringLiteral(":/data/nofile.xml"));
    QCOMPARE(infos.parse(), false);
    QCOMPARE(infos.status(), QKnxGroupAddressInfos::Status::FileError);
    QCOMPARE(infos.errorString(), QString("No such file or directory"));

    infos.clear();
    QCOMPARE(infos.projectFile(), QString());
    QCOMPARE(infos.status(), QKnxGroupAddressInfos::Status::NoError);
    QCOMPARE(infos.errorString(), QString());

    infos.setProjectFile(QStringLiteral(":/data/qt.io.knxproj"));
    QCOMPARE(infos.projectFile(), QStringLiteral(":/data/qt.io.knxproj"));
    QCOMPARE(infos.parse(), true);

    QCOMPARE(infos.projectIds().count(), 1);
    QCOMPARE(infos.projectIds().first(), QString("P-03D9"));
    QCOMPARE(infos.projectName(QString("P-03D9")), QString("qt.io.test"));

    auto installations = infos.installations(QString("P-03D9"));
    QCOMPARE(installations.count(), 1);

    auto installation = installations.first();
    QCOMPARE(installation, QString());
    QCOMPARE(infos.infoCount(QString("P-03D9"), installation), 95);
    QCOMPARE(infos.addressInfos(QString("P-03D9"), installation).size(), 95);

    const auto groupAddressInfos = initGroupAddressInfos();
    const auto entries = infos.addressInfos(QString("P-03D9"), installation);
    for (const auto &entry : qAsConst(groupAddressInfos))
        QVERIFY2(entries.contains(entry), entry.name().toLatin1());

    infos.clear();
    infos.add(groupAddressInfos.first(), QString("P-03D9"));
    QCOMPARE(infos.addressInfos(QString("P-03D9"), installation).first(), groupAddressInfos.first());

    infos.remove(groupAddressInfos.last(), QString("P-03D9"));
    QCOMPARE(infos.addressInfos(QString("P-03D9"), installation).first(), groupAddressInfos.first());

    infos.remove(groupAddressInfos.first(), QString("P-03D9"));
    QCOMPARE(infos.infoCount(QString("P-03D9"), ""), 0);
}

QTEST_MAIN(tst_QKnxGroupAddressInfos)

#include "tst_qknxgroupaddressinfo.moc"
