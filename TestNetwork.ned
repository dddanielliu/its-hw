//
// 最簡單的測試網絡
//

simple TestApp
{
    parameters:
        @class(TestApp);
}

network TestNetwork
{
    submodules:
        node1: TestApp {
            @display("p=100,100");
        }
        node2: TestApp {
            @display("p=200,100");
        }
}
