find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Knx Network)

qt_standard_project_setup()

qt_add_executable(discoverer MACOSX_BUNDLE
    main.cpp
)

target_include_directories(discoverer PRIVATE
    .
)

target_link_libraries(discoverer PRIVATE
    Qt::Core
    Qt::Knx
    Qt::Network
)

install(TARGETS discoverer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

