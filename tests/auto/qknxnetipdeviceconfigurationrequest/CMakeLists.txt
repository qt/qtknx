add_executable(tst_qknxnetipdeviceconfigurationrequest tst_qknxnetipdeviceconfigurationrequest.cpp)
target_link_libraries(tst_qknxnetipdeviceconfigurationrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdeviceconfigurationrequest COMMAND tst_qknxnetipdeviceconfigurationrequest)
