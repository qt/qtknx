add_executable(tst_qknxnetipconnectionstateresponse tst_qknxnetipconnectionstateresponse.cpp)
target_link_libraries(tst_qknxnetipconnectionstateresponse PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipconnectionstateresponse COMMAND tst_qknxnetipconnectionstateresponse)
