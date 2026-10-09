add_executable(tst_qknxproject tst_qknxproject.cpp)
target_link_libraries(tst_qknxproject PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Test
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

qt_add_resources(tst_qknxproject "project"
    PREFIX
        "/"
    FILES
        ${project_resource_files}
)
add_test(NAME tst_qknxproject COMMAND tst_qknxproject)
