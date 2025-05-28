#include "GlobalMyVeinsApp.h"

using namespace veins;

Define_Module(MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        EV_INFO << "Initializing MyVeinsApp for "
                << par("appName").stringValue() << endl;
    }
    else if (stage == 1) {
        // ★ Only vehicles should send BSM, not RSUs
        if (traciVehicle) {  // This will be null for RSUs
            sendBsmEvt = new cMessage("sendBSM");
            scheduleAt(simTime() + 1, sendBsmEvt);
            EV_INFO << "Vehicle initialized - will start sending BSM" << endl;
        } else {
            EV_INFO << "RSU initialized - will only receive messages" << endl;
        }
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

    EV_INFO << "[Vehicle " << beacon->getVehicle() << "] Send BSM - Speed=" 
            << beacon->getSpeed() << " m/s, Position=(" << beacon->getPosX() 
            << ", " << beacon->getPosY() << ")" << endl;
}

void MyVeinsApp::onWSM(BaseFrame1609_4* wsm)
{
    auto* enc = wsm->getEncapsulatedPacket();
    auto* data = dynamic_cast<CustomSafetyMessage*>(enc);

    if (data) {
        // Check if this is an RSU receiving the message
        if (!traciVehicle) {  // RSU doesn't have traciVehicle
            EV_INFO << "[RSU] Received BSM from Vehicle ID=" << data->getVehicle()
                    << ", Speed=" << data->getSpeed() << " m/s"
                    << ", Position=(" << data->getPosX() << ", " << data->getPosY() << ")" << endl;
        } else {
            // Vehicle receiving message
            EV_INFO << "[Vehicle " << getParentModule()->getId() << "] Received BSM from Vehicle ID=" << data->getVehicle()
                    << ", Speed=" << data->getSpeed() << " m/s"
                    << ", Position=(" << data->getPosX() << ", " << data->getPosY() << ")" << endl;
        }
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
