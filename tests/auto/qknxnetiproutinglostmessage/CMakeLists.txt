add_executable(tst_qknxnetiproutinglostmessage tst_qknxnetiproutinglostmessage.cpp)
target_link_libraries(tst_qknxnetiproutinglostmessage PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiproutinglostmessage COMMAND tst_qknxnetiproutinglostmessage)
