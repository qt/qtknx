add_executable(tst_qknxcryptographicengine tst_qknxcryptographicengine.cpp)
target_link_libraries(tst_qknxcryptographicengine PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxcryptographicengine COMMAND tst_qknxcryptographicengine)
