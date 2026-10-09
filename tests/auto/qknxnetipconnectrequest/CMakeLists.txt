add_executable(tst_qknxnetipconnectrequest tst_qknxnetipconnectrequest.cpp)
target_link_libraries(tst_qknxnetipconnectrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipconnectrequest COMMAND tst_qknxnetipconnectrequest)
