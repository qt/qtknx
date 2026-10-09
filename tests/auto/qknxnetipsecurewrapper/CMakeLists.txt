add_executable(tst_qknxnetipsecurewrapper tst_qknxnetipsecurewrapper.cpp)
target_link_libraries(tst_qknxnetipsecurewrapper PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
    Qt::Test
)
add_test(NAME tst_qknxnetipsecurewrapper COMMAND tst_qknxnetipsecurewrapper)
