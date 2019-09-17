lessThan(QT_MAJOR_VERSION, 5) {
    error("Cannot build current Qt KNX sources with Qt version $${QT_VERSION}.")
}
requires(!wasm)
requires(!integrity)

load(configure)
load(qt_parts)
