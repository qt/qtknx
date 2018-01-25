requires(qtHaveModule(network))

TEMPLATE = subdirs

SUBDIRS += discoverer knxproj tunnelclient

qtHaveModule(widgets) {
    SUBDIRS += knxeditor
}

qtHaveModule(quick) {
    SUBDIRS += demo
}
