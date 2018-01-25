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
    $$PWD/qknxinstallation.cpp \
    $$PWD/qknxprojectinformation.cpp \
    $$PWD/qknxprojectroot.cpp \
    $$PWD/qknxprojectutils.cpp \
    $$PWD/qknxtopology.cpp \
    $$PWD/qzip.cpp

PRIVATE_HEADERS += $$PWD/qzipreader_p.h
    $$PWD/qzipwriter_p.h

HEADERS += \
    $$PWD/qknxbuildings.h \
    $$PWD/qknxdeviceinstance.h \
    $$PWD/qknxgroupaddresses.h \
    $$PWD/qknxinstallation.h \
    $$PWD/qknxprojectinformation.h \
    $$PWD/qknxprojectroot.h \
    $$PWD/qknxprojectutils.h \
    $$PWD/qknxtopology.h
