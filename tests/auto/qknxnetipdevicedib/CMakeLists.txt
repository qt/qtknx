add_executable(tst_qknxnetipdevicedib tst_qknxnetipdevicedib.cpp)
target_link_libraries(tst_qknxnetipdevicedib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdevicedib COMMAND tst_qknxnetipdevicedib)
