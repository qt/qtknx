add_executable(tst_qknxnetiphpai tst_qknxnetiphpai.cpp)
target_link_libraries(tst_qknxnetiphpai PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiphpai COMMAND tst_qknxnetiphpai)
