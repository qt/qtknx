add_executable(tst_qknxnetipsessionrequest tst_qknxnetipsessionrequest.cpp)
target_link_libraries(tst_qknxnetipsessionrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipsessionrequest COMMAND tst_qknxnetipsessionrequest)
