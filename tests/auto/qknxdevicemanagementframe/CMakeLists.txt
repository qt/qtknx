add_executable(tst_qknxdevicemanagementframe tst_qknxdevicemanagementframe.cpp)
target_link_libraries(tst_qknxdevicemanagementframe PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Test
)
add_test(NAME tst_qknxdevicemanagementframe COMMAND tst_qknxdevicemanagementframe)
