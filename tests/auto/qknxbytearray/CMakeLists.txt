add_executable(tst_qknxbytearray tst_qknxbytearray.cpp)
target_link_libraries(tst_qknxbytearray PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Test
)
add_test(NAME tst_qknxbytearray COMMAND tst_qknxbytearray)
