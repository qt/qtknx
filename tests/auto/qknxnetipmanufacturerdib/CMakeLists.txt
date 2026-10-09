add_executable(tst_qknxnetipmanufacturerdib tst_qknxnetipmanufacturerdib.cpp)
target_link_libraries(tst_qknxnetipmanufacturerdib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipmanufacturerdib COMMAND tst_qknxnetipmanufacturerdib)
