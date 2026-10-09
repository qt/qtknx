add_executable(tst_qknxnetiproutingbusy tst_qknxnetiproutingbusy.cpp)
target_link_libraries(tst_qknxnetiproutingbusy PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiproutingbusy COMMAND tst_qknxnetiproutingbusy)
