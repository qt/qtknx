add_qt_test(tst_qknxnetipconnectrequest
    GUI
    SOURCES
        tst_qknxnetipconnectrequest.cpp
    PUBLIC_LIBRARIES
        Qt::Knx
        Qt::Network
)
