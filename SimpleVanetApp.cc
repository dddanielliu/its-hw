//
// 簡化的 VANET 應用程式實現
//

#include <omnetpp.h>
#include "CustomSafetyMessage_m.h"

using namespace omnetpp;

class SimpleVanetApp : public cSimpleModule
{
private:
    cMessage *sendBsmEvt = nullptr;
    int vehicleId;
    bool isVehicle;
    
public:
    virtual ~SimpleVanetApp();
    
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    void sendBSM();
    void handleBSM(CustomSafetyMessage *bsm);
};

Define_Module(SimpleVanetApp);

SimpleVanetApp::~SimpleVanetApp()
{
    cancelAndDelete(sendBsmEvt);
}

void SimpleVanetApp::initialize()
{
    vehicleId = getId();
    isVehicle = strcmp(getParentModule()->getName(), "vehicle") == 0;
    
    EV_INFO << "初始化 " << (isVehicle ? "車輛" : "RSU") 
            << " ID=" << vehicleId << endl;
    
    if (isVehicle) {
        // 只有車輛發送 BSM
        sendBsmEvt = new cMessage("sendBSM");
        scheduleAt(simTime() + uniform(0.1, 1.0), sendBsmEvt);
    }
}

void SimpleVanetApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == sendBsmEvt) {
            sendBSM();
            scheduleAt(simTime() + 1.0, sendBsmEvt); // 每秒發送一次
        }
    } else {
        // 接收到的消息
        CustomSafetyMessage *bsm = dynamic_cast<CustomSafetyMessage*>(msg);
        if (bsm) {
            handleBSM(bsm);
        }
        delete msg;
    }
}

void SimpleVanetApp::sendBSM()
{
    CustomSafetyMessage *bsm = new CustomSafetyMessage("BSM");
    bsm->setVehicle(vehicleId);
    bsm->setSpeed(uniform(5, 25)); // 隨機速度 5-25 m/s
    bsm->setPosX(uniform(0, 1000)); // 隨機位置
    bsm->setPosY(uniform(0, 1000));
    bsm->setAcl(uniform(-2, 2)); // 隨機加速度
    
    EV_INFO << "[車輛 " << vehicleId << "] 發送 BSM - "
            << "速度=" << bsm->getSpeed() << " m/s, "
            << "位置=(" << bsm->getPosX() << ", " << bsm->getPosY() << ")" << endl;
    
    // 廣播給所有其他節點（簡化的實現）
    cModule *network = getSystemModule();
    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule *node = *it;
        if (node != getParentModule()) {
            cModule *app = node->getSubmodule("app");
            if (!app) app = node; // 如果是 RSU
            if (app && app != this) {
                CustomSafetyMessage *copy = bsm->dup();
                sendDirect(copy, app, "directIn");
            }
        }
    }
    
    delete bsm;
}

void SimpleVanetApp::handleBSM(CustomSafetyMessage *bsm)
{
    EV_INFO << "[" << (isVehicle ? "車輛" : "RSU") << " " << vehicleId 
            << "] 接收到 BSM 來自車輛 " << bsm->getVehicle()
            << " - 速度=" << bsm->getSpeed() << " m/s, "
            << "位置=(" << bsm->getPosX() << ", " << bsm->getPosY() << ")" << endl;
}

void SimpleVanetApp::finish()
{
    EV_INFO << "模擬結束 - " << (isVehicle ? "車輛" : "RSU") 
            << " ID=" << vehicleId << endl;
}
