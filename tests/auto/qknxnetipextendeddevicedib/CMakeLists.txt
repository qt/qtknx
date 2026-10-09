add_executable(tst_qknxnetipextendeddevicedib tst_qknxnetipextendeddevicedib.cpp)
target_link_libraries(tst_qknxnetipextendeddevicedib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipextendeddevicedib COMMAND tst_qknxnetipextendeddevicedib)
