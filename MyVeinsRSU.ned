//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//

import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

// RSU應用層模組定義
simple MyVeinsRSU extends DemoBaseApplLayer
{
    @class(MyVeinsApp);
    string appName = default("MyVeinsRSU");
}
