add_executable(tst_qknxnetipdisconnectrequest tst_qknxnetipdisconnectrequest.cpp)
target_link_libraries(tst_qknxnetipdisconnectrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdisconnectrequest COMMAND tst_qknxnetipdisconnectrequest)
