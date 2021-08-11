//
// Created by william on 2021/6/20.
//

#ifndef CROSS_PLATFORM_DEMO_DEVICEOBJECT_H
#define CROSS_PLATFORM_DEMO_DEVICEOBJECT_H
#include "base.h"

class InstanceObject;

class DeviceObject
{
public:
    explicit DeviceObject(std::shared_ptr<InstanceObject>& instance);
    ~DeviceObject();
    /// 不可复制
    DeviceObject(const DeviceObject&) = delete;
    /// 不可移动
    DeviceObject(DeviceObject&&) = delete;
    /// 不可赋值
    DeviceObject& operator=(const DeviceObject&) = delete;
    DeviceObject& operator=(DeviceObject&&) = delete;

    inline VkPhysicalDevice handle() const
    {
        return m_handler;
    }
    inline std::shared_ptr<InstanceObject>& instance()
    {
        return m_instance;
    }
    inline VkPhysicalDeviceFeatures& features()
    {
        return m_features;
    }
    inline VkPhysicalDeviceMemoryProperties& memoryProperties()
    {
        return m_memoryProperties;
    }

public:
    // 这个类对外公开的主要函数
    VkResult createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions);
    void destroyDevice();
    // Get the avaialbe queues exposed by the physical devices
    void getPhysicalDeviceQueuesAndProperties();
    // Query physical device to retrive queue properties
    uint32_t getGraphicsQueueHandle();

public:
    /// 实例对象句柄
    std::shared_ptr<InstanceObject> m_instance;
    /// vulkan 物理设备句柄
    VkPhysicalDevice m_handler = VK_NULL_HANDLE;
    /// gpu支持的设备特性
    VkPhysicalDeviceFeatures m_features{};
    /// gpu 属性
    VkPhysicalDeviceProperties m_properties{};
    /// gpu内存属性
    VkPhysicalDeviceMemoryProperties m_memoryProperties;
    /// gpu 队列FamilyProperties
    std::vector<VkQueueFamilyProperties> m_queueFamilyProperties;


    /// 内存类型
    std::vector<VkMemoryType> m_allowedMemoryTypes;
    std::vector<VkMemoryHeap> m_allowedHeapTypes;




};

#endif //CROSS_PLATFORM_DEMO_DEVICEOBJECT_H
