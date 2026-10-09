add_executable(tst_qknxnetipdescriptionresponse tst_qknxnetipdescriptionresponse.cpp)
target_link_libraries(tst_qknxnetipdescriptionresponse PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdescriptionresponse COMMAND tst_qknxnetipdescriptionresponse)
