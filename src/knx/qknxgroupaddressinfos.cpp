/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknxgroupaddressinfos.h"
#include "qzipreader_p.h"
#include "qknxprojectroot.h"

QT_BEGIN_NAMESPACE

class QKnxGroupAddressInfosPrivate
{
public:
    bool m_filledFromProjectFile = false;

    QString m_projectFileName;
    QString m_projectId;

    QHash<QString, QHash<QKnxAddress, QKnxGroupAddressInfo>>
        m_groupAddressesFullDescriptionPerInstallation;
    QHash<QString, QMultiHash<QKnxDatapointType::Type, QKnxAddress>>
        m_dptTypeGroupAddressesPerInstallation;

    void addEntry(const QKnxAddress &groupAddress, const QKnxDatapointType::Type &info);

    QKnxGroupAddressInfos::ReadStatus readProjectFile();
    QKnxGroupAddressInfos::ReadStatus readZipFile(const QString &projectFileName);
    QKnxGroupAddressInfos::ReadStatus readXmlFile(QXmlStreamReader *reader);
    QKnxGroupAddressInfos::ReadStatus readProject(const QKnxProject &project);
    void fillInfo(const QString &installationIndex, const QKnxGroupAddressInfo &groupAddressInfo);
    void fillMultiHash();
    void readGroupRange(const QKnxGroupRange &groupRange, const QString &installationName);
};

static inline uint readUInt(const uchar *data)
{
    return (data[0]) + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
}

QKnxGroupAddressInfos::ReadStatus QKnxGroupAddressInfosPrivate::readProjectFile()
{
    QFile file(m_projectFileName);
    if (file.open(QIODevice::ReadOnly) != true)
        return QKnxGroupAddressInfos::ReadStatus::FileError;

    uchar tmp[4];
    file.read((char *) tmp, 4);
    if (readUInt(tmp) == 0x04034b50) {
        file.close();
        return readZipFile(m_projectFileName);
    }

    file.seek(0);
    QXmlStreamReader reader(&file);
    if (!reader.hasError())
        return readXmlFile(&reader);
    return QKnxGroupAddressInfos::ReadStatus::FileError;
}

QKnxGroupAddressInfos::ReadStatus
QKnxGroupAddressInfosPrivate::readZipFile(const QString &projectFileName)
{
    QZipReader zipReader(projectFileName);
    int nullXmlId = -1;
    auto fileInfos = zipReader.fileInfoList();
    for (int i = 0; i < fileInfos.count(); ++i) {
        if (fileInfos[i].filePath.endsWith(QStringLiteral("0.xml"))) {
            nullXmlId = i;
            if (nullXmlId >= 0)
                break;
        }
    }
    if (nullXmlId < 0)
        return QKnxGroupAddressInfos::ReadStatus::FileError;

    QXmlStreamReader xmlReader(zipReader.fileData(fileInfos[nullXmlId].filePath));
    return readXmlFile(&xmlReader);
}

QKnxGroupAddressInfos::ReadStatus QKnxGroupAddressInfosPrivate::readXmlFile(QXmlStreamReader *reader)
{
    if (!reader->readNextStartElement())
        return QKnxGroupAddressInfos::ReadStatus::FileReadError;

    if (reader->name() != QStringLiteral("KNX"))
        return QKnxGroupAddressInfos::ReadStatus::FileNameError;

    QKnxProjectRoot root;
    if (!root.parseElement(reader, true))
        return QKnxGroupAddressInfos::ReadStatus::FileProjectReadError;

    if (root.Project.size() < 1)
        return QKnxGroupAddressInfos::ReadStatus::FileProjectSizeError;

    // Choose the right project
    for (int i = 0; i < root.Project.size(); i++) {
        if (root.Project[i].Id == m_projectId) {
            return readProject(root.Project[i]);
        }
    }
    return QKnxGroupAddressInfos::ReadStatus::FileProjectError;
}

QKnxGroupAddressInfos::ReadStatus QKnxGroupAddressInfosPrivate::readProject(const QKnxProject &project)
{
    int numberOfInstallation = project.Installations.size();
    if (numberOfInstallation < 1)
        return QKnxGroupAddressInfos::ReadStatus::FileInstallationSizeError;
    // Read the group address information for each installation
    for (int iInstallation = 0; iInstallation < numberOfInstallation; iInstallation++) {
        QKnxInstallation installation = project.Installations[iInstallation];
        QString installationName = installation.Name;
        // Loop over the different level of the KNX file to fill the information about the group addresses
        if (installation.GroupAddresses.size() < 1)
            return QKnxGroupAddressInfos::ReadStatus::FileGroupAddressesSizeError;
        // Loop over all the group addresses within the current installation
        for (int iGroupAddresses = 0; iGroupAddresses < installation.GroupAddresses.size();
            iGroupAddresses++) {
            QKnxGroupAddresses groupAddresses = installation.GroupAddresses[iGroupAddresses];
            if (groupAddresses.GroupRanges.size() < 1)
                return QKnxGroupAddressInfos::ReadStatus::FileGroupRangesSizeError;
            //Loop over the group ranges with the current group Addresses
            for (int iGroupRanges = 0; iGroupRanges < groupAddresses.GroupRanges.size();
                iGroupRanges++) {
                QKnxGroupRange groupRange = groupAddresses.GroupRanges[iGroupRanges];
                readGroupRange(groupRange, installationName);
            }
        }
    }
    return QKnxGroupAddressInfos::ReadStatus::NoError;
}

void QKnxGroupAddressInfosPrivate::readGroupRange(const QKnxGroupRange &groupRange,
    const QString &installationName)
{
    for (int iGroupAddress = 0; iGroupAddress < groupRange.GroupAddress.size(); iGroupAddress++) {
        QKnxGroupAddress groupAddressRead = groupRange.GroupAddress[iGroupAddress];
        QKnxGroupAddressInfo info(groupAddressRead);
        fillInfo(installationName, info);
    }
    for (int iSubGroupRange = 0; iSubGroupRange < groupRange.GroupRange.size(); iSubGroupRange++) {
        readGroupRange(groupRange.GroupRange[iSubGroupRange], installationName);
    }
}

void QKnxGroupAddressInfosPrivate::fillInfo(const QString &installationName,
    const QKnxGroupAddressInfo &info)
{

    if (!m_groupAddressesFullDescriptionPerInstallation.contains(installationName)) {
        QHash<QKnxAddress, QKnxGroupAddressInfo> groupAddressesFullDescription;
        groupAddressesFullDescription.insert(info.groupAddress(), info);
        m_groupAddressesFullDescriptionPerInstallation.insert(installationName, groupAddressesFullDescription);

        QMultiHash<QKnxDatapointType::Type, QKnxAddress> dptTypeGroupAddresses;
        dptTypeGroupAddresses.insert(info.dataPointType(), info.groupAddress());
        m_dptTypeGroupAddressesPerInstallation.insert(installationName, dptTypeGroupAddresses);
    } else {
        m_groupAddressesFullDescriptionPerInstallation[installationName].insert(
            info.groupAddress(), info);
        m_dptTypeGroupAddressesPerInstallation[installationName].insert(info.dataPointType(),
            info.groupAddress());
    }
}

QKnxGroupAddressInfos::QKnxGroupAddressInfos()
    : d(new QKnxGroupAddressInfosPrivate)
{}

QKnxGroupAddressInfos::QKnxGroupAddressInfos(const QString &knxProjectFile,
    const QString &projectId)
    : d(new QKnxGroupAddressInfosPrivate)
{
    d->m_projectFileName = knxProjectFile;
    d->m_projectId = projectId;
}

QString QKnxGroupAddressInfos::knxProjectFileName() const
{
    return d->m_projectFileName;
}

QString QKnxGroupAddressInfos::knxProjectId() const
{
    return d->m_projectId;
}

QKnxGroupAddressInfos::ReadStatus QKnxGroupAddressInfos::readFile(const QString &knxProjectFile,
    const QString &projectId)
{
    d->m_projectFileName = knxProjectFile;
    d->m_projectId = projectId;
    return readFile();
}

QKnxGroupAddressInfos::ReadStatus QKnxGroupAddressInfos::readFile()
{
    clearInfos();
    QKnxGroupAddressInfos::ReadStatus readStatus = d->readProjectFile();
    if (readStatus == ReadStatus::NoError)
        d->m_filledFromProjectFile = true;
    return readStatus;
}

void QKnxGroupAddressInfos::clearInfos()
{
    d->m_groupAddressesFullDescriptionPerInstallation.clear();
    d->m_dptTypeGroupAddressesPerInstallation.clear();
    d->m_filledFromProjectFile = false;

}

QKnxGroupAddressInfos::QKnxGroupAddressInfos(const QKnxGroupAddressInfos &o)
{
    if (!d)
        d = new QKnxGroupAddressInfosPrivate;
    d->m_dptTypeGroupAddressesPerInstallation = o.d->m_dptTypeGroupAddressesPerInstallation;
    d->m_groupAddressesFullDescriptionPerInstallation = o.d->m_groupAddressesFullDescriptionPerInstallation;
}

QList<QString> QKnxGroupAddressInfos::installationsName() const
{
    return d->m_groupAddressesFullDescriptionPerInstallation.keys();
}

QKnxGroupAddressInfos::~QKnxGroupAddressInfos()
{
    delete d;
}

QKnxGroupAddressInfos &
QKnxGroupAddressInfos::operator=(const QKnxGroupAddressInfos &other)
{
    if (this != &other) {
        if (!d)
            d = new QKnxGroupAddressInfosPrivate;
        d->m_dptTypeGroupAddressesPerInstallation = other.d->m_dptTypeGroupAddressesPerInstallation;
        d->m_groupAddressesFullDescriptionPerInstallation = other.d->m_groupAddressesFullDescriptionPerInstallation;
    }
    return *this;
}


bool QKnxGroupAddressInfos::addEntry(const QKnxGroupAddressInfo &info, const QString &installationName,
    bool overwrite)
{
    if (d->m_filledFromProjectFile && !overwrite)
        return false;
    d->fillInfo(installationName, info);
    return true;
}


QMultiHash<QKnxAddress, QKnxGroupAddressInfo>
QKnxGroupAddressInfos::groupAddressesFullDescription(const QString &installationName) const
{
    return d->m_groupAddressesFullDescriptionPerInstallation[installationName];
}

QMultiHash<QKnxDatapointType::Type, QKnxAddress>
QKnxGroupAddressInfos::datapointTypeGroupAddresses(const QString &installationName) const
{
    return  d->m_dptTypeGroupAddressesPerInstallation[installationName];
}

QList<QKnxAddress> QKnxGroupAddressInfos::groupAddresses(const QString &installationName) const
{
    return d->m_groupAddressesFullDescriptionPerInstallation[installationName].keys();
}

QList<QKnxAddress> QKnxGroupAddressInfos::groupAddresses(const QKnxDatapointType::Type &type,
    const QString &installationName) const
{
    return d->m_dptTypeGroupAddressesPerInstallation[installationName].values(type);
}

QKnxGroupAddressInfo
QKnxGroupAddressInfos::groupAddressInfo(const QKnxAddress &groupAddress,
    const QString &installationName) const
{
    return d->m_groupAddressesFullDescriptionPerInstallation[installationName].value(groupAddress);
}

QKnxDatapointType::Type QKnxGroupAddressInfos::dptType(const QKnxAddress &groupAddress,
    const QString &installationName) const
{
    return groupAddressInfo(groupAddress, installationName).dataPointType();
}

QDebug operator<<(QDebug debug, const QKnxGroupAddressInfos &groupAddressesInfos)
{
    QDebugStateSaver _(debug);


    auto installations = groupAddressesInfos.installationsName();
    if (installations.size() == 0)
        return debug;

    for (int iInstallation = 0; iInstallation < installations.size();
        iInstallation++) {
        QString name = installations[iInstallation];
        QList<QKnxAddress> allAddresses = groupAddressesInfos.groupAddresses(name);
        if (allAddresses.size() == 0)
            continue;
        QHash<QKnxAddress, QKnxGroupAddressInfo> fullDescription =
            groupAddressesInfos.groupAddressesFullDescription(installations[iInstallation]);
        for (int i = 0; i < allAddresses.size(); ++i) {
            QString description = fullDescription.value(allAddresses[i]).description();
            debug.nospace().noquote() << "{Installation: " << name <<
                "}, {Group Address: " << allAddresses[i] <<
                "}, {Description: " << description <<
                "}, {DPT Type: " << quint32(groupAddressesInfos.dptType(allAddresses[i], name)) << "}";
        }
    }
    return debug;
}

QT_END_NAMESPACE
