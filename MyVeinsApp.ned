//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//

package homework;

import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

// 自定義應用層模組定義
simple MyVeinsApp extends DemoBaseApplLayer
{
    @class(MyVeinsApp);
    string appName = default("MyVeinsApp");
}

// 網路配置
import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Scenario;

network homework extends Scenario
{
    submodules:
        // 設置一個RSU
        rsu[1]: RSU {
            @display("p=400,300;i=veins/sign/yellowdiamond;is=vs");
            // 配置RSU使用我們的應用層
            appl.typename = "MyVeinsApp";
            appl.appName = "MyRSUApp";
        }
}
