add_executable(tst_qknxnetiproutingsystembroadcast tst_qknxnetiproutingsystembroadcast.cpp)
target_link_libraries(tst_qknxnetiproutingsystembroadcast PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiproutingsystembroadcast COMMAND tst_qknxnetiproutingsystembroadcast)
