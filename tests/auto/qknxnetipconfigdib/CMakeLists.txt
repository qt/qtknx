add_executable(tst_qknxnetipconfigdib tst_qknxnetipconfigdib.cpp)
target_link_libraries(tst_qknxnetipconfigdib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipconfigdib COMMAND tst_qknxnetipconfigdib)
