add_executable(tst_qknxnetipcurrentconfigdib tst_qknxnetipcurrentconfigdib.cpp)
target_link_libraries(tst_qknxnetipcurrentconfigdib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipcurrentconfigdib COMMAND tst_qknxnetipcurrentconfigdib)
