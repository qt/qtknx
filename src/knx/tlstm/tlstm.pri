INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/qknxtransportlayerclosedstate.cpp \
    $$PWD/qknxtransportlayerconnectingstate.cpp \
    $$PWD/qknxtransportlayeropenidlestate.cpp \
    $$PWD/qknxtransportlayeropenwaitstate.cpp \
    $$PWD/qknxtransportlayerstate.cpp \
    $$PWD/qknxtransportlayerstatemachine.cpp

    PRIVATE_HEADERS += \
    $$PWD/qknxtransportlayerclosedstate_p.h \
    $$PWD/qknxtransportlayerconnectingstate_p.h \
    $$PWD/qknxtransportlayeropenidlestate_p.h \
    $$PWD/qknxtransportlayeropenwaitstate_p.h \
    $$PWD/qknxtransportlayerstate_p.h \
    $$PWD/qknxtransportlayerstatemachine_p.h
