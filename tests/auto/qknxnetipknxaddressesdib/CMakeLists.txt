add_executable(tst_qknxnetipknxaddressesdib tst_qknxnetipknxaddressesdib.cpp)
target_link_libraries(tst_qknxnetipknxaddressesdib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipknxaddressesdib COMMAND tst_qknxnetipknxaddressesdib)
