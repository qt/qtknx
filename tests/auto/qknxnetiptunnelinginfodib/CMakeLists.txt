add_executable(tst_qknxnetiptunnelinginfodib tst_qknxnetiptunnelinginfodib.cpp)
target_link_libraries(tst_qknxnetiptunnelinginfodib PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiptunnelinginfodib COMMAND tst_qknxnetiptunnelinginfodib)
