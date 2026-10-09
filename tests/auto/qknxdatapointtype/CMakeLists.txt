add_executable(tst_qknxdatapointtype tst_qknxdatapointtype.cpp)
target_link_libraries(tst_qknxdatapointtype PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxdatapointtype COMMAND tst_qknxdatapointtype)
