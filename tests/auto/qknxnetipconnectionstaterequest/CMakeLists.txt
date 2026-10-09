add_executable(tst_qknxnetipconnectionstaterequest tst_qknxnetipconnectionstaterequest.cpp)
target_link_libraries(tst_qknxnetipconnectionstaterequest PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipconnectionstaterequest COMMAND tst_qknxnetipconnectionstaterequest)
