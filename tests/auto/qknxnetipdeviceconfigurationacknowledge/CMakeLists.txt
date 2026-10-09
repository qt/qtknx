add_executable(tst_qknxnetipdeviceconfigurationacknowledge tst_qknxnetipdeviceconfigurationacknowledge.cpp)
target_link_libraries(tst_qknxnetipdeviceconfigurationacknowledge PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdeviceconfigurationacknowledge COMMAND tst_qknxnetipdeviceconfigurationacknowledge)
