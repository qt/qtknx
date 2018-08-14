%modules = (
    "QtKnx" => "$basedir/src/knx",
);

%dependencies = (
        "qtbase" => ""
);

%classnames = (
    "qknxnamespace.h" => "QKnx",
    "qknxnetip.h" => "NetIp,CemiServer",
    "qknxdevicemanagementframefactory.h" => "QKnxDeviceManagementFrameBuilder",
    "qknxlinklayerframebuilder.h" => "QKnxLinkLayerFrameBuilder",
    "qknxnetiptunnelinginfodib.h" => "QKnxNetIpTunnelingSlotInfo",
    "qknxnetipsrp.h" => "QKnxNetIpSrpBuilder",
);

%deprecatedheaders = (
    "QtKnx" => {
        "qknxglobal.h" => "QtKnx/qtknxglobal.h"
    }
);
