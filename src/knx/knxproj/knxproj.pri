INCLUDEPATH += $$PWD

qtConfig(system-zlib) {
    QMAKE_USE_PRIVATE += zlib
} else {
    QT_PRIVATE += zlib-private
}

SOURCES += \
    $$PWD/qknxbuildings.cpp \
    $$PWD/qknxdeviceinstance.cpp \
    $$PWD/qknxgroupaddresses.cpp \
    $$PWD/qknxgroupaddressinfo.cpp \
    $$PWD/qknxgroupaddressinfos.cpp \
    $$PWD/qknxinstallation.cpp \
    $$PWD/qknxprojectinformation.cpp \
    $$PWD/qknxprojectroot.cpp \
    $$PWD/qknxprojectutils.cpp \
    $$PWD/qknxtopology.cpp \
    $$PWD/qzip.cpp

PRIVATE_HEADERS += \
    $$PWD/qknxbuildings_p.h \
    $$PWD/qknxdeviceinstance_p.h \
    $$PWD/qknxgroupaddresses_p.h \
    $$PWD/qknxinstallation_p.h \
    $$PWD/qknxprojectinformation_p.h \
    $$PWD/qknxprojectroot_p.h \
    $$PWD/qknxprojectutils_p.h \
    $$PWD/qknxtopology_p.h \
    $$PWD/qzipreader_p.h \
    $$PWD/qzipwriter_p.h

HEADERS += \
    $$PWD/qknxgroupaddressinfo.h \
    $$PWD/qknxgroupaddressinfos.h
