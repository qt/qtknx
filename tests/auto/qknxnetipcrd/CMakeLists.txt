add_executable(tst_qknxnetipcrd tst_qknxnetipcrd.cpp)
target_link_libraries(tst_qknxnetipcrd PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipcrd COMMAND tst_qknxnetipcrd)
