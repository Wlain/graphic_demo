//
// Created by ytech on 2021/8/11.
//

#ifndef CROSS_PLATFORM_DEMO_DEVICE_H
#define CROSS_PLATFORM_DEMO_DEVICE_H
#include "physicalDevice.h"
#include "instance.h"
#include "resourceCache.h"

// Device 主要是提供一个命令队列，指定想启用的扩展。
class Device
{
public:
    Device(PhysicalDevice& gpu, VkSurfaceKHR surface, const char* extensions);
    ~Device();
    /// 不可复制
    Device(const Device&) = delete;
    /// 不可移动
    Device(Device&&) = delete;
    /// 不可赋值
    Device& operator=(const Device&) = delete;
    Device& operator=(Device&&) = delete;

    inline VkQueue& queue() { return m_queue; }
    inline operator VkDevice() { return m_handle; }
    inline operator const VkDevice() const { return m_handle; }
    inline VkDevice handle() { return m_handle; }
    inline VkDevice handle() const { return m_handle; }

private:
    const PhysicalDevice& m_gpu;
    ResourceCache m_resourceCache;
    VkDevice m_handle = VK_NULL_HANDLE;
    VkQueue m_queue = VK_NULL_HANDLE;
    uint32_t m_queueIndex = 1;
};

#endif //CROSS_PLATFORM_DEMO_DEVICE_H
