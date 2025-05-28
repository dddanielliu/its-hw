#include "MyVeinsApp.h"
#include "CustomSafetyMessage_m.h"
#include "DemoServiceAdvertisement_m.h"
// #include "DemoBaseApplLayer.h"
// #include <omnetpp.h>

using namespace veins;
using omnetpp::cMessage;

Define_Module(veins::MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        EV_INFO << "Initializing MyVeinsApp for "
                << par("appName").stringValue() << endl;
    }
    else if (stage == 1) {
        // ★ Create and schedule the BSM event
        sendBsmEvt = new cMessage("sendBSM");
        scheduleAt(simTime() + 1, sendBsmEvt);
    }
}

void MyVeinsApp::handleSelfMsg(cMessage* msg)
{
    if (msg == sendBsmEvt) {
        if (traciVehicle)  // Confirm vehicle exists in SUMO
            sendBSMMessage();

        scheduleAt(simTime() + 1, sendBsmEvt);  // Reschedule
        return;
    }

    // Let parent handle other messages
    DemoBaseApplLayer::handleSelfMsg(msg);
}

void MyVeinsApp::sendBSMMessage()
{
    auto* beacon = new CustomSafetyMessage("bsm");

    beacon->setVehicle(getParentModule()->getId());

    double speed = traciVehicle ? traciVehicle->getSpeed() : 0;
    double acl   = traciVehicle ? traciVehicle->getAcceleration() : 0;
    beacon->setSpeed(speed);
    beacon->setAcl(acl);

    Coord pos = mobility->getPositionAt(simTime());
    beacon->setPosX(pos.x);
    beacon->setPosY(pos.y);

    auto* wsm = new BaseFrame1609_4();
    wsm->encapsulate(beacon);
    populateWSM(wsm);

    sendDown(wsm);

    EV_INFO << "Send BSM from id=" << beacon->getVehicle() << endl;
    EV_INFO << "Speed=" << beacon->getSpeed() << endl;
    EV_INFO << "PosX=" << beacon->getPosX() << endl;
    EV_INFO << "PosY=" << beacon->getPosY() << endl;
}

void MyVeinsApp::onWSM(BaseFrame1609_4* wsm)
{
    auto* enc = wsm->getEncapsulatedPacket();
    auto* data = dynamic_cast<CustomSafetyMessage*>(enc);

    if (data) {
        EV_INFO << "Received BSM: ID="  << data->getVehicle()
                << ", Speed="           << data->getSpeed()
                << ", PosX="            << data->getPosX()
                << ", PosY="            << data->getPosY() << endl;
    }
}

void MyVeinsApp::onMyBSM(CustomSafetyMessage* bsm)
{
    EV_INFO << "onMyBSM called (beacon) – vehicle=" << bsm->getVehicle() << endl;
}

void MyVeinsApp::onBSM(DemoSafetyMessage* bsm)
{
    // This is for the original veins DemoSafetyMessage
    EV_INFO << "onBSM called (standard veins beacon)" << endl;
}

void MyVeinsApp::onWSA(DemoServiceAdvertisment*)
{
    // Not used for now
}

void MyVeinsApp::handlePositionUpdate(omnetpp::cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
}

void MyVeinsApp::finish()
{
    DemoBaseApplLayer::finish();
}
