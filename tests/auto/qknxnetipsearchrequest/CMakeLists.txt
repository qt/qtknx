add_executable(tst_qknxnetipsearchrequest tst_qknxnetipsearchrequest.cpp)
target_link_libraries(tst_qknxnetipsearchrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipsearchrequest COMMAND tst_qknxnetipsearchrequest)
