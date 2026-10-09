add_executable(tst_qknxadditionalinfo tst_qknxadditionalinfo.cpp)
target_link_libraries(tst_qknxadditionalinfo PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Test
)
add_test(NAME tst_qknxadditionalinfo COMMAND tst_qknxadditionalinfo)
