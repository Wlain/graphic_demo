#include "instanceObject.h"
#include "deviceObject.h"
int main()
{
//    TriangleVk triangle;
//    triangle.run();
//    triangle.cleanup();
    auto instance = std::make_shared<InstanceObject>();
    auto device = std::make_shared<DeviceObject>(instance);

    return 0;
}