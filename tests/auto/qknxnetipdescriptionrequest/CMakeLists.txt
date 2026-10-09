add_executable(tst_qknxnetipdescriptionrequest tst_qknxnetipdescriptionrequest.cpp)
target_link_libraries(tst_qknxnetipdescriptionrequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipdescriptionrequest COMMAND tst_qknxnetipdescriptionrequest)
