add_executable(tst_qknxgroupaddressinfo tst_qknxgroupaddressinfo.cpp)
target_link_libraries(tst_qknxgroupaddressinfo PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)


# Resources:
set(project_resource_files
    "data/0.xml"
    "data/qt.io.knxproj"
)

qt_add_resources(tst_qknxgroupaddressinfo "project"
    PREFIX
        "/"
    FILES
        ${project_resource_files}
)
add_test(NAME tst_qknxgroupaddressinfo COMMAND tst_qknxgroupaddressinfo)
