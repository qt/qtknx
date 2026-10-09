add_executable(tst_qknxnetiptunnelingrequest tst_qknxnetiptunnelingrequest.cpp)
target_link_libraries(tst_qknxnetiptunnelingrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiptunnelingrequest COMMAND tst_qknxnetiptunnelingrequest)
