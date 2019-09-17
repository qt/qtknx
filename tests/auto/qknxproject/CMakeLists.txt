add_qt_test(tst_qknxproject
    GUI
    SOURCES
        tst_qknxproject.cpp
    LIBRARIES
        Qt::KnxPrivate
    PUBLIC_LIBRARIES
        Qt::Knx
)

# Resources:
set(project_resource_files
    "data/buildings.xml"
    "data/groupaddresses.xml"
    "data/locations.xml"
    "data/projectinformation.xml"
    "data/topology.xml"
    "data/trades-busaccess-splitinfos.xml"
    "data/unassigneddevices.xml"
)

add_qt_resource(tst_qknxproject "project"
    PREFIX
        "/"
    FILES
        ${project_resource_files}
)
