add_executable(tst_qknxnetiptunnelingfeature tst_qknxnetiptunnelingfeature.cpp)
target_link_libraries(tst_qknxnetiptunnelingfeature PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiptunnelingfeature COMMAND tst_qknxnetiptunnelingfeature)
