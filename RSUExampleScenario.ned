//
// VANET RSU 場景 - completely self-contained
//

network RSUExampleScenario
{
    parameters:
        @display("bgb=2500,2500");
    
    submodules:
        rsu[1]: SimpleRSU {
            @display("p=2000,2000;i=veins/sign/rsu");
        }
        node[1]: SimpleVehicle {
            @display("p=500,500;i=veins/node/car");
        }
    connections allowunconnected:
}
