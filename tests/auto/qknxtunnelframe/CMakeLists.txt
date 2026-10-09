add_executable(tst_qknxtunnelframe tst_qknxtunnelframe.cpp)
target_link_libraries(tst_qknxtunnelframe PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Test
)
add_test(NAME tst_qknxtunnelframe COMMAND tst_qknxtunnelframe)
