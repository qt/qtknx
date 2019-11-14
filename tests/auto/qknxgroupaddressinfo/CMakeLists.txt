# Generated from qknxgroupaddressinfo.pro.

#####################################################################
## tst_qknxgroupaddressinfo Test:
#####################################################################

qt_add_test(tst_qknxgroupaddressinfo
    SOURCES
        tst_qknxgroupaddressinfo.cpp
    PUBLIC_LIBRARIES
        Qt::Knx
)

# Resources:
set(project_resource_files
    "data/0.xml"
    "data/qt.io.knxproj"
)

qt_add_resource(tst_qknxgroupaddressinfo "project"
    PREFIX
        "/"
    FILES
        ${project_resource_files}
)

