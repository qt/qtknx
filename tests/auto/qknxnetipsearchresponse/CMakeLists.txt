add_executable(tst_qknxnetipsearchresponse tst_qknxnetipsearchresponse.cpp)
target_link_libraries(tst_qknxnetipsearchresponse PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipsearchresponse COMMAND tst_qknxnetipsearchresponse)
