# Generated from qknxnetiprouter.pro.

#####################################################################
## tst_qknxnetiproutinginterface Test:
#####################################################################

qt_add_test(tst_qknxnetiproutinginterface
    SOURCES
        tst_qknxnetiprouter.cpp
    LIBRARIES
        Qt::KnxPrivate
    PUBLIC_LIBRARIES
        Qt::Knx
        Qt::Network
)
