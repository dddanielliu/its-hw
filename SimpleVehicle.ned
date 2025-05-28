//
// Simple Vehicle definition as compound module
//

module SimpleVehicle
{
    parameters:
        @display("i=veins/node/car");
        string applType = default("MyVeinsApp");
    submodules:
        appl: MyVeinsApp {
            @display("p=100,100;i=block/app");
        }
    connections allowunconnected:
}
