find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Knx Network)

qt_standard_project_setup()

qt_add_executable(tunnelclient MACOSX_BUNDLE
    main.cpp
)

target_include_directories(tunnelclient PRIVATE
    .
)

target_link_libraries(tunnelclient PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
)

install(TARGETS tunnelclient
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
