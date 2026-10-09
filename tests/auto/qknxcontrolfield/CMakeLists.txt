add_executable(tst_qknxcontrolfield tst_qknxcontrolfield.cpp)
target_link_libraries(tst_qknxcontrolfield PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxcontrolfield COMMAND tst_qknxcontrolfield)
