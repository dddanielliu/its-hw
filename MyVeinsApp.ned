//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//

// 自定義應用層模組定義
import veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple MyVeinsApp extends DemoBaseApplLayer
{
    @class(MyVeinsApp);
    string appName = default("MyVeinsApp");
}
