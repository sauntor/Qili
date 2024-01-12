 function Component()
 {
     // default constructor
 }

 Component.prototype.createOperations = function()
 {
     component.createOperations();
    console.log("systemInfo", systemInfo.kernelType, systemInfo.productType, systemInfo);
     if (systemInfo.kernelType === "winnt") {
         component.addOperation("CreateShortcut", "@TargetDir@\\bin\\Qili.exe", "@StartMenuDir@/Qili.lnk",
             "workingDirectory=@TargetDir@", "description=@ProductName@");
     }
     else if (systemInfo.kernelType === "linux") {
        component.addOperation("CreateDesktopEntry", "me.sauntor.qili.desktop",
             `Name=Qili
Name[zh_CN]=Qili弹幕姬
Name[zh_TW]=Qili彈幕姬
GenericName=@ProductName@
GenericName[zh_CN]=B站直播间弹幕朗读工具
GenericName[zh_TW]=B站直播間彈幕朗讀者
Categories=Network;AudioVideo;
Encoding=UTF-8
Exec=@TargetDir@/bin/Qili
X-KDE-StartupNotify=true
StartupNotify=true
Terminal=false
Type=Application
Icon=@TargetDir@/share/pixmaps/qili.png
`
             );
     }
 }
