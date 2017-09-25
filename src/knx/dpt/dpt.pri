INCLUDEPATH += $$PWD

PUBLIC_HEADERS += $$PWD/qknxdatapointtype.h \
    $$PWD/qknxdatapointtypefactory.h \
    $$PWD/qknx1bit.h \
    $$PWD/qknx1bitcontrolled.h \
    $$PWD/qknx8bitset.h \
    $$PWD/qknxtime.h \
    $$PWD/qknxdatetime.h \
    $$PWD/qknxentranceaccess.h \
    $$PWD/qknx2bytefloat.h

PRIVATE_HEADERS += $$PWD/qknxdatapointtype_p.h

SOURCES += $$PWD/qknxdatapointtype.cpp \
    $$PWD/qknxdatapointtypefactory.cpp \
    $$PWD/qknx1bit.cpp \
    $$PWD/qknx1bitcontrolled.cpp \
    $$PWD/qknx8bitset.cpp \
    $$PWD/qknxtime.cpp \
    $$PWD/qknxdatetime.cpp \
    $$PWD/qknxentranceaccess.cpp \
    $$PWD/qknx2bytefloat.cpp
