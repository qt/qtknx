add_executable(tst_qknxnetipconnectionheader tst_qknxnetipconnectionheader.cpp)
target_link_libraries(tst_qknxnetipconnectionheader PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Test
)
add_test(NAME tst_qknxnetipconnectionheader COMMAND tst_qknxnetipconnectionheader)
