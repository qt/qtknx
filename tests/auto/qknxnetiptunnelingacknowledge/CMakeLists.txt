add_executable(tst_qknxnetiptunnelingacknowledge tst_qknxnetiptunnelingacknowledge.cpp)
target_link_libraries(tst_qknxnetiptunnelingacknowledge PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiptunnelingacknowledge COMMAND tst_qknxnetiptunnelingacknowledge)
