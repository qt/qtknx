add_qt_test(tst_qknxnetiphpai
    GUI
    SOURCES
        tst_qknxnetiphpai.cpp
    PUBLIC_LIBRARIES
        Qt::Knx
        Qt::Network
)
