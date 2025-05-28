//
// Simple RSU definition as compound module
//

module SimpleRSU
{
    parameters:
        @display("i=veins/sign/rsu");
        string applType = default("MyVeinsApp");
    submodules:
        appl: MyVeinsApp {
            @display("p=100,100;i=block/app");
        }
    connections allowunconnected:
}
