qt_add_executable(group WIN32 MACOSX_BUNDLE
    deviceitem.cpp deviceitem.h
    groupaddressvalidater.cpp groupaddressvalidater.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)
target_include_directories(group PRIVATE
    .
)

target_link_libraries(group PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Knx
    Qt::Network
    Qt::Widgets
)

install(TARGETS group
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

