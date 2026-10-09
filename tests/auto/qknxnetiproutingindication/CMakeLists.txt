add_executable(tst_qknxnetiproutingindication tst_qknxnetiproutingindication.cpp)
target_link_libraries(tst_qknxnetiproutingindication PRIVATE
    Qt::Core
    Qt::Knx
    Qt::KnxPrivate
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetiproutingindication COMMAND tst_qknxnetiproutingindication)
