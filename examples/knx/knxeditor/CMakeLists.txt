qt_add_executable(knxeditor WIN32 MACOSX_BUNDLE
    localdevicemanagement.cpp localdevicemanagement.h localdevicemanagement.ui
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    tunneling.cpp tunneling.h tunneling.ui
    tunnelingfeatures.cpp tunnelingfeatures.h tunnelingfeatures.ui
)
target_include_directories(knxeditor PRIVATE
    .
)

target_link_libraries(knxeditor PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Knx
    Qt::Network
    Qt::Widgets
)

install(TARGETS knxeditor
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

