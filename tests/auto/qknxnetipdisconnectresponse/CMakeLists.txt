add_executable(tst_qknxnetipdisconnectresponse tst_qknxnetipdisconnectresponse.cpp)
target_link_libraries(tst_qknxnetipdisconnectresponse PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdisconnectresponse COMMAND tst_qknxnetipdisconnectresponse)
