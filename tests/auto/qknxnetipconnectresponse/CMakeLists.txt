add_executable(tst_qknxnetipconnectresponse tst_qknxnetipconnectresponse.cpp)
target_link_libraries(tst_qknxnetipconnectresponse PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipconnectresponse COMMAND tst_qknxnetipconnectresponse)
