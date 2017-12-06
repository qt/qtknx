requires(qtHaveModule(network))

TEMPLATE = subdirs


SUBDIRS += discoverer tunnelclient demo

qtHaveModule(widgets) {
    SUBDIRS += knxeditor
}
