qt_add_executable(feature WIN32 MACOSX_BUNDLE
    deviceitem.cpp deviceitem.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)
target_include_directories(feature PRIVATE
    .
)

target_link_libraries(feature PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Knx
    Qt::Network
    Qt::Widgets
)

install(TARGETS feature
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

