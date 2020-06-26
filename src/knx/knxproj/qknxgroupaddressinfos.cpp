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
#include "qknxgroupaddresses_p.h"
#include "qknxprojectroot_p.h"

#include "qzipreader_p.h"

QT_BEGIN_NAMESPACE

// -- KnxProjectInfo

struct KnxProjectInfo final
{
    QString name;
    QHash<QString, QList<QKnxGroupAddressInfo>> installations;

    bool operator==(const KnxProjectInfo &other) const
    {
        return name == other.name && installations == other.installations;
    }
    inline bool operator!=(const KnxProjectInfo &other) const { return !operator==(other); }
};


// -- QKnxGroupAddressInfosPrivate

class QKnxGroupAddressInfosPrivate final : public QSharedData
{
public:
    bool parseData(const QByteArray &data);
    bool readProject(const QKnxProject &project);
    QList<QKnxGroupAddressInfo> readRange(const QKnxGroupRange &range, const QString &install);

    QString projectFile;
    QString errorString;
    QHash<QString, KnxProjectInfo> projects;

    QKnxGroupAddressInfos::Status status = QKnxGroupAddressInfos::Status::NoError;
};

/*!
    \internal
*/
bool QKnxGroupAddressInfosPrivate::parseData(const QByteArray &data)
{
    if (data.isEmpty()) {
        status = QKnxGroupAddressInfos::Status::FileError;
        errorString = QKnxGroupAddressInfos::tr("Could not read project file.");
        return false;
    }

    QXmlStreamReader reader(data);
    if (reader.hasError() || !reader.readNextStartElement()) {
        errorString = reader.errorString();
        status = QKnxGroupAddressInfos::Status::ParseError;
        return false;
    }

    if (reader.name() != QStringLiteral("KNX")) {
        status = QKnxGroupAddressInfos::Status::ParseError;
        errorString = QKnxGroupAddressInfos::tr("Not a valid KNX project file.");
        return false;
    }

    QKnxProjectRoot root;
    if (!root.parseElement(&reader, true)) {
        errorString = reader.errorString();
        status = QKnxGroupAddressInfos::Status::ParseError;
        return false;
    }

    if (root.Project.size() < 1) {
        status = QKnxGroupAddressInfos::Status::ProjectError;
        errorString = QKnxGroupAddressInfos::tr("The project file did not contain a KNX project.");
        return false;
    }

    for (const auto &project : qAsConst(root.Project)) {
        if (readProject(project))
            continue;
        projects.clear();
        return false; // early return on error
    }
    return true;
}

/*!
    \internal
*/
bool QKnxGroupAddressInfosPrivate::readProject(const QKnxProject &project)
{
    if (projects.contains(project.Id)) {
        status = QKnxGroupAddressInfos::Status::ProjectError;
        errorString = QKnxGroupAddressInfos::tr("Project '%1' exists more than once.")
            .arg(project.Id);
        return false;
    }

    if (project.Installations.size() > 0) {
        KnxProjectInfo info;
        for (const auto &install : qAsConst(project.Installations)) {
            if (info.installations.contains(install.Name)) {
                status = QKnxGroupAddressInfos::Status::ProjectError;
                errorString = QKnxGroupAddressInfos::tr("Installation '%1' exists more than once.")
                    .arg(install.Name.isEmpty() ? QStringLiteral("<empty>") : install.Name);
                return false;
            }
            QList<QKnxGroupAddressInfo> addressInfos;
            for (const auto &addresses : qAsConst(install.GroupAddresses)) {
                for (const auto &range : qAsConst(addresses.GroupRanges))
                    addressInfos.append(readRange(range, install.Name));
            }
            info.installations.insert(install.Name, addressInfos);
        }
        projects.insert(project.Id, info);
    } else {
        projects.insert(project.Id, { QString(), {} }); // projects can have empty installations
    }

    return status == QKnxGroupAddressInfos::Status::NoError;
}

/*!
    \internal
*/
QList<QKnxGroupAddressInfo>
    QKnxGroupAddressInfosPrivate::readRange(const QKnxGroupRange &range, const QString &install)
{
    QList<QKnxGroupAddressInfo> addressInfos;
    for (const auto &groupRange : qAsConst(range.GroupRange))
        addressInfos.append(readRange(groupRange, install));

    for (const auto &address : qAsConst(range.GroupAddress)) {
        addressInfos.append({ install, address.Name, quint16(address.Address),
            address.DatapointType, address.Description });
    }
    return addressInfos;
}


/*!
    \class QKnxGroupAddressInfos

    \inmodule QtKnx
    \ingroup qtknx-general-classes

    \brief The QKnxGroupAddressInfos class provides access to information about
    group addresses used inside a KNX project.

    This class parses the KNX project information specified via the
    setProjectFile() method and provides access to the information defined
    within the file through an API. You can:

    \list
        \li Get information about existing project IDs and, if available, the
            corresponding project names.
        \li Get the installations used inside a given project.
        \li Get information about the number of group addresses used throughout
            an installation inside a given KNX project.
        \li Fetch and set group address information for an installation
            associated with a KNX project.
    \endlist
*/

/*!
    \enum QKnxGroupAddressInfos::Status

    This enum holds the status of parsing the KNX project information.

    \value NoError
    \value FileError
    \value ProjectError
    \value ParseError

    \sa parse()
*/

/*!
    Creates a new empty group address infos object.
*/
QKnxGroupAddressInfos::QKnxGroupAddressInfos()
    : d_ptr(new QKnxGroupAddressInfosPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxGroupAddressInfos::~QKnxGroupAddressInfos()
{}

/*!
    Creates a new empty group address infos object and sets the current project
    file to \a projectFile. Creating the object does not automatically trigger
    parsing the given \a projectFile.

    The function argument can be either a valid KNX \c 0.xml or a \c .knxproj
    project file.

    \sa parse()
*/
QKnxGroupAddressInfos::QKnxGroupAddressInfos(const QString &projectFile)
    : d_ptr(new QKnxGroupAddressInfosPrivate)
{
    setProjectFile(projectFile);
}

/*!
    Returns the current project file from where the group address information
    was collected.
*/
QString QKnxGroupAddressInfos::projectFile() const
{
    return d_ptr->projectFile;
}

/*!
    Sets the current project file to \a projectFile and clears all existing
    information. Calling this function does not automatically trigger parsing
    the given \a projectFile.

    The function argument can be either a valid KNX \c 0.xml or a \c .knxproj
    project file.

    \sa parse()
*/
void QKnxGroupAddressInfos::setProjectFile(const QString &projectFile)
{
    clear();
    d_ptr->projectFile = projectFile;
}

/*!
    \internal
*/
static bool isZipFile(QFile *file)
{
    quint8 tmp[4];
    file->read((char *) tmp, 4);
    file->seek(0u);
    return quint32((tmp[0]) + (tmp[1] << 8) + (tmp[2] << 16) + (tmp[3] << 24)) == 0x04034b50;
}

/*!
    Clears all existing information and parses the KNX project file.

    Returns \c true if parsing was successful; otherwise returns \c false. If
    an error occurs, sets the \l Status, and fills the errorString().
*/
bool QKnxGroupAddressInfos::parse()
{
    auto tmp = d_ptr->projectFile;
    clear();
    d_ptr->projectFile = tmp;

    QFile file(d_ptr->projectFile);
    if (!file.open(QIODevice::ReadOnly)) {
        d_ptr->status = Status::FileError;
        d_ptr->errorString = file.errorString();
        return false;
    }

    if (!isZipFile(&file))
        return d_ptr->parseData(file.readAll());

    QSet<QString> files;
    QZipReader zipReader(&file);
    const auto fileInfos = zipReader.fileInfoList();
    for (const auto &fileInfo : qAsConst(fileInfos)) {
        auto file = fileInfo.filePath;
        file = file.mid(file.lastIndexOf(QLatin1Char('/'), -5) + 1);
        if (file == QStringLiteral("0.xml"))
            files.insert(fileInfo.filePath);
    }

    if (files.isEmpty())
        return d_ptr->parseData({});

    for (auto file : qAsConst(files)) {
        if (d_ptr->parseData(zipReader.fileData(file))) {
            const auto data = zipReader.fileData(file.replace(QStringLiteral("0.xml"),
                QStringLiteral("project.xml")));

            QXmlStreamReader r(data);
            if (r.hasError() || !r.readNextStartElement() || r.name() != QStringLiteral("KNX"))
                continue;

            QKnxProjectRoot root;
            if (!root.parseElement(&r, true))
                continue;
            for (const auto &project : qAsConst(root.Project)) {
                if (d_ptr->projects.contains(project.Id))
                    d_ptr->projects[project.Id].name = project.ProjectInformation.value(0).Name;
            }
        } else {
            d_ptr->projects.clear();
            return false;
        }
    }
    return true;
}

/*!
    Clears all existing information including the KNX project file name.
*/
void QKnxGroupAddressInfos::clear()
{
    d_ptr->projects.clear();
    d_ptr->projectFile.clear();
    d_ptr->errorString.clear();
    d_ptr->status = QKnxGroupAddressInfos::Status::NoError;
}

/*!
    Returns the read status of the current KNX project file.
*/
QKnxGroupAddressInfos::Status QKnxGroupAddressInfos::status() const
{
    return d_ptr->status;
}

/*!
    Returns the human-readable message appropriate to the reported read status.
*/
QString QKnxGroupAddressInfos::errorString() const
{
    return d_ptr->errorString;
}

/*!
    Returns a list of all KNX project ids found in the KNX project file.
*/
QList<QString> QKnxGroupAddressInfos::projectIds() const
{
    return d_ptr->projects.keys().toList();
}

/*!
    Returns the project name associated with the given KNX \a projectId ID.
    Since the project name is an optional field in a KNX project, it can be
    empty.
*/
QString QKnxGroupAddressInfos::projectName(const QString &projectId) const
{
    return d_ptr->projects.value(projectId, { QString(), {} }).name;
}

/*!
    Returns a list of all installation ids for the given KNX \a projectId ID.
*/
QList<QString> QKnxGroupAddressInfos::installations(const QString &projectId) const
{
    return d_ptr->projects.value(projectId, { QString(), {} }).installations.keys().toList();
}

/*!
    Returns the number of group address info objects contained in this class;
    identified by \a projectId and \a installation.
*/
qint32 QKnxGroupAddressInfos::infoCount(const QString &projectId, const QString &installation) const
{
    const auto installations = d_ptr->projects.value(projectId, { QString(), {} }).installations;
    if (installations.isEmpty())
        return -1;

    if (!installations.contains(installation))
        return -1;
    return installations.value(installation).count();
}

/*!
    Returns a list of all available group address infos from a KNX project
    identified by \a projectId and \a installation.
*/
QList<QKnxGroupAddressInfo> QKnxGroupAddressInfos::addressInfos(const QString &projectId,
    const QString &installation) const
{
    if (projectId.isEmpty())
        return {};
    return d_ptr->projects.value(projectId, { QString(), {} }).installations.value(installation);
}

/*!
    Returns a list of all available group address infos from a KNX project
    identified by \a address, \a projectId, and \a installation.
*/
QList<QKnxGroupAddressInfo> QKnxGroupAddressInfos::addressInfos(const QKnxAddress &address,
    const QString &projectId, const QString &installation) const
{
    const auto infos = addressInfos(projectId, installation);
    if (infos.isEmpty())
        return {};

    QList<QKnxGroupAddressInfo> results;
    for (const auto &info : qAsConst(infos)) {
        if (info.address() == address)
            results.append(info);
    }
    return results;
}

/*!
    Returns a list of all available group address infos from a KNX project
    identified by datapoint \a type, \a projectId and \a installation.
*/
QList<QKnxGroupAddressInfo> QKnxGroupAddressInfos::addressInfos(QKnxDatapointType::Type type,
         const QString &projectId, const QString &installation) const
{
    const auto infos = addressInfos(projectId, installation);
    if (infos.isEmpty())
        return {};

    QList<QKnxGroupAddressInfo> results;
    for (const auto &info : qAsConst(infos)) {
        if (info.datapointType() == type)
            results.append(info);
    }
    return results;
}

/*!
    Adds a group address \a info object of a KNX project identified by \a projectId.
    The group address \a info \l QKnxGroupAddressInfo::installation ID is used to
    associate the group address info with an existing KNX project.

    \note The value of \a projectId must not be empty.
    \note The function does not prevent multiple identical entries.
*/
void QKnxGroupAddressInfos::add(const QKnxGroupAddressInfo &info, const QString &projectId)
{
    if (projectId.isEmpty())
        return;
    d_ptr->projects[projectId].installations[info.installation()].append(info);
}

/*!
    Creates a group address info object with values set to \a name, \a address,
    \a type, \a description and \a installation. The created object is associated
    with the given KNX \a projectId ID, and \a installation.

    \note The value of \a projectId must not be empty.
    \note The function does not prevent multiple identical entries.
*/
void QKnxGroupAddressInfos::add(const QString &name, const QKnxAddress &address,
    QKnxDatapointType::Type type, const QString &description, const QString &projectId,
    const QString &installation)
{
    if (projectId.isEmpty())
        return;
    d_ptr->projects[projectId].installations[installation].append({ installation, name,
        address, type, description });
}

/*!
    Creates a group address info object with values set to \a name, \a address,
    \a datapointType, \a description and \a installation. The created object
    is associated with the given KNX \a projectId ID, and \a installation.

    \note The value of \a projectId must not be empty.
    \note The function does not prevent multiple identical entries.
*/
void QKnxGroupAddressInfos::add(const QString &name, const QKnxAddress &address,
    const QString &datapointType, const QString &description, const QString &projectId,
    const QString &installation)
{
    if (projectId.isEmpty())
        return;
    d_ptr->projects[projectId].installations[installation].append({ installation, name,
        address, datapointType, description });
}

/*!
    Removes all the group address info objects from a KNX project identified by
    \a projectId and \a installation ID and with the same KNX group \a address.

    \note The value of \a projectId must not be empty.
*/
void QKnxGroupAddressInfos::remove(const QKnxAddress &address, const QString &projectId,
    const QString &installation)
{
    if (!address.isValid() || projectId.isEmpty() || !d_ptr->projects.contains(projectId))
        return;

    auto &installations = d_ptr->projects[projectId].installations;
    auto &value = installations[installation];
    for (qint32 count = value.count(); count --> 0;) {
        if (value[count].address() == address)
            value.remove(count);
    }
}

/*!
    Removes all the objects equal to group address \a info from a KNX project
    identified by \a projectId. The group address \a info
    \l QKnxGroupAddressInfo::installation ID is used to associate the group
    address info with an existing KNX project.

    \note The value of \a projectId must not be empty.
*/
void QKnxGroupAddressInfos::remove(const QKnxGroupAddressInfo &info, const QString &projectId)
{
    if (projectId.isEmpty() || !d_ptr->projects.contains(projectId))
        return;

    auto &installations = d_ptr->projects[projectId].installations;
    auto &value = installations[info.installation()];
    for (qint32 count = value.count(); count-- > 0;) {
        if (value[count] == info)
            value.remove(count);
    }
}

/*!
    Constructs a copy of \a other.
*/
QKnxGroupAddressInfos::QKnxGroupAddressInfos(const QKnxGroupAddressInfos &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxGroupAddressInfos &QKnxGroupAddressInfos::operator=(const QKnxGroupAddressInfos &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxGroupAddressInfos::QKnxGroupAddressInfos(QKnxGroupAddressInfos &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxGroupAddressInfos &QKnxGroupAddressInfos::operator=(QKnxGroupAddressInfos &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxGroupAddressInfos::swap(QKnxGroupAddressInfos &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxGroupAddressInfos::operator==(const QKnxGroupAddressInfos &other) const
{
    return d_ptr == other.d_ptr || [&]() -> bool {
        return d_ptr->projectFile == other.d_ptr->projectFile
            && d_ptr->errorString == other.d_ptr->errorString
            && d_ptr->projects == other.d_ptr->projects
            && d_ptr->status == other.d_ptr->status;
    }();
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxGroupAddressInfos::operator!=(const QKnxGroupAddressInfos &other) const
{
    return !operator==(other);
}

/*!
    \internal
*/
QKnxGroupAddressInfos::QKnxGroupAddressInfos(QKnxGroupAddressInfosPrivate &dd)
    : d_ptr(new QKnxGroupAddressInfosPrivate(dd))
{}

/*!
    \relates QKnxGroupAddressInfos

    Writes the \a infos object to the \a debug stream and returns a reference to
    the stream.
*/
QDebug operator<<(QDebug debug, const QKnxGroupAddressInfos &infos)
{
    QDebugStateSaver saver(debug);
    debug.resetFormat().nospace().noquote();
    debug << "QKnxGroupAddressInfos (projectFile=" << infos.projectFile()
        << ", errorString=" << infos.errorString()
        << ", status=" << quint8(infos.status())
        << ", projectIds=" << infos.projectIds()
        << ')';
    return debug;
}

QT_END_NAMESPACE
