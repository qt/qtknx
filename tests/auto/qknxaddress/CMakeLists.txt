add_executable(tst_qknxaddress tst_qknxaddress.cpp)
target_link_libraries(tst_qknxaddress PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxaddress COMMAND tst_qknxaddress)
