add_qt_test(tst_qknxgroupaddressinfo
    GUI
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

add_qt_resource(tst_qknxgroupaddressinfo "project"
    PREFIX
        "/"
    FILES
        ${project_resource_files}
)
