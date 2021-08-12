#include "instanceObject.h"
#include "physicalDeviceObject.h"
#include "deviceObject.h"
int main()
{
    auto instance = std::make_shared<InstanceObject>();
    auto physicalDevice = std::make_shared<PhysicalDeviceObject>(instance);
    auto device = std::make_shared<DeviceObject>(physicalDevice);
    return 0;
}