add_executable(tst_qknxnetipsessionresponse tst_qknxnetipsessionresponse.cpp)
target_link_libraries(tst_qknxnetipsessionresponse PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipsessionresponse COMMAND tst_qknxnetipsessionresponse)
