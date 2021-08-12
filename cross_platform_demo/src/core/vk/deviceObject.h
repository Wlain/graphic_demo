//
// Created by ytech on 2021/8/11.
//

#ifndef CROSS_PLATFORM_DEMO_DEVICEOBJECT_H
#define CROSS_PLATFORM_DEMO_DEVICEOBJECT_H
#include "physicalDeviceObject.h"
// Device 主要是提供一个命令队列，指定想启用的扩展。
class DeviceObject : private std::enable_shared_from_this<PhysicalDeviceObject>
{
public:
    DeviceObject(std::shared_ptr<PhysicalDeviceObject> physicalDeviceObject);
    ~DeviceObject();
    inline VkQueue& queue() { return m_queue; }

private:
    std::shared_ptr<PhysicalDeviceObject> m_physicalDevice;
    VkDevice m_handle = VK_NULL_HANDLE;
    VkQueue m_queue = VK_NULL_HANDLE;
    uint32_t m_queueIndex = 1;
};

#endif //CROSS_PLATFORM_DEMO_DEVICEOBJECT_H
