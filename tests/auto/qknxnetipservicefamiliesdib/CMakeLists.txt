add_executable(tst_qknxnetipservicefamiliesdib tst_qknxnetipservicefamiliesdib.cpp)
target_link_libraries(tst_qknxnetipservicefamiliesdib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipservicefamiliesdib COMMAND tst_qknxnetipservicefamiliesdib)
