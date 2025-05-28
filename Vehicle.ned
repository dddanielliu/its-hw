//
// 簡化的車輛節點定義
//

module Vehicle
{
    parameters:
        @display("i=device/car;is=s");
        string vehicleId = default("vehicle");
        double x = default(0);
        double y = default(0);
        double speed = default(10); // m/s
    submodules:
        app: SimpleVanetApp {
            @display("p=50,50");
        }
}
