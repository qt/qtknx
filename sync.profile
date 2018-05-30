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
    "qknxnetiptunnelinginfodib.h" => "QKnxNetIpTunnelingSlotInfo"
);
