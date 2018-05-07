%modules = (
    "QtKnx" => "$basedir/src/knx",
);

%dependencies = (
        "qtbase" => ""
);

%classnames = (
    "qknxnamespace.h" => "QKnx",
    "qknxnetip.h" => "NetIp,CemiServer",
    "qknxdevicemanagementframefactory.h" => "QKnxDeviceManagementFrameBuilder,FunctionPropertyCommandBuilder,FunctionPropertyStateReadBuilder,PropertyInfoBuilder,PropertyReadBuilder,PropertyWriteBuilder,ResetBuilder",
);
