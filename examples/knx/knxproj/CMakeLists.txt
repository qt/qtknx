qt_add_executable(knxproj MACOSX_BUNDLE
    main.cpp
)
target_include_directories(knxproj PRIVATE
    .
)

target_link_libraries(knxproj PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
)

install(TARGETS knxproj
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

