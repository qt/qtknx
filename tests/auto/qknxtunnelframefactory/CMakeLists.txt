add_executable(tst_qknxtunnelframefactory tst_qknxtunnelframefactory.cpp)
target_link_libraries(tst_qknxtunnelframefactory PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Test
)
add_test(NAME tst_qknxtunnelframefactory COMMAND tst_qknxtunnelframefactory)
