add_executable(tst_qknxtpdufactory tst_qknxtpdufactory.cpp)
target_link_libraries(tst_qknxtpdufactory PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Test
)
add_test(NAME tst_qknxtpdufactory COMMAND tst_qknxtpdufactory)
