requires(qtHaveModule(network))

TEMPLATE = subdirs

SUBDIRS += discoverer knxproj tunnelclient router

qtHaveModule(widgets) {
    SUBDIRS += knxeditor group device feature
}
