lessThan(QT_MAJOR_VERSION, 5) {
    error("Cannot build current Qt KNX sources with Qt version $${QT_VERSION}.")
}
requires(!integrity)

load(configure)
load(qt_parts)

include($$OUT_PWD/src/knx/qtknx-config.pri)
QT_FOR_CONFIG += knx-private

requires(qtConfig(compilerdetection))
