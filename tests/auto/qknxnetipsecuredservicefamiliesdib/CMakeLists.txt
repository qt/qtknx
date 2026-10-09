add_executable(tst_qknxnetipsecuredservicefamiliesdib tst_qknxnetipsecuredservicefamiliesdib.cpp)
target_link_libraries(tst_qknxnetipsecuredservicefamiliesdib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipsecuredservicefamiliesdib COMMAND tst_qknxnetipsecuredservicefamiliesdib)
