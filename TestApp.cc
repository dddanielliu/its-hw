//
// 非常簡單的測試應用
//

#include "MyVeinsApp.h"

using namespace omnetpp;

class TestApp : public cSimpleModule
{
private:
    cMessage *testEvent = nullptr;
    
public:
    virtual ~TestApp() { cancelAndDelete(testEvent); }
    
protected:
    virtual void initialize() override {
        EV_INFO << "TestApp 初始化 - ID=" << getId() << endl;
        testEvent = new cMessage("test");
        scheduleAt(simTime() + 1.0, testEvent);
    }
    
    virtual void handleMessage(cMessage *msg) override {
        if (msg->isSelfMessage()) {
            EV_INFO << "TestApp " << getId() << " 在時間 " << simTime() << " 發送測試訊息" << endl;
            scheduleAt(simTime() + 1.0, testEvent);
        } else {
            delete msg;
        }
    }
};

Define_Module(TestApp);
