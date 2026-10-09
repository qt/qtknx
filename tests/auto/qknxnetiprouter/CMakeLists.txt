add_executable(tst_qknxnetiproutinginterface tst_qknxnetiprouter.cpp)
target_link_libraries(tst_qknxnetiproutinginterface PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiproutinginterface COMMAND tst_qknxnetiproutinginterface)
