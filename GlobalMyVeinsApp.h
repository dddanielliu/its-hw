//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//

#pragma once

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "CustomSafetyMessage_m.h"
#include "DemoServiceAdvertisement_m.h"

using namespace omnetpp;

/**
 * @brief
 * Global namespace version of MyVeinsApp for OMNeT++ compatibility
 */

class MyVeinsApp : public veins::DemoBaseApplLayer {
public:
    virtual void initialize(int stage) override;
    virtual void handleSelfMsg(cMessage* msg) override;
    void onMyBSM(veins::CustomSafetyMessage* bsm);
    virtual void onBSM(veins::DemoSafetyMessage* bsm) override;
    virtual void onWSM(veins::BaseFrame1609_4* wsm) override;
    virtual void onWSA(veins::DemoServiceAdvertisment* wsa) override;
    virtual void handlePositionUpdate(cObject* obj) override;
    virtual void finish() override;

protected:
    cMessage* sendBsmEvt = nullptr;
    void sendBSMMessage();
};
