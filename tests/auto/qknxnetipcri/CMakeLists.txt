add_executable(tst_qknxnetipcri tst_qknxnetipcri.cpp)
target_link_libraries(tst_qknxnetipcri PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipcri COMMAND tst_qknxnetipcri)
