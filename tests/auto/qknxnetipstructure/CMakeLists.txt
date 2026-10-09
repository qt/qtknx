add_executable(tst_qknxnetipstructure tst_qknxnetipstructure.cpp)
target_link_libraries(tst_qknxnetipstructure PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipstructure COMMAND tst_qknxnetipstructure)
