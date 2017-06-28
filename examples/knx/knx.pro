requires(qtHaveModule(network))

TEMPLATE = subdirs


SUBDIRS += discoverer tunnelclient

qtHaveModule(widgets) {
    SUBDIRS += knxeditor
}
