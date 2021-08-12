//
// Created by william on 2021/6/20.
//

#ifndef CROSS_PLATFORM_DEMO_PHYSICALDEVICEOBJECT_H
#define CROSS_PLATFORM_DEMO_PHYSICALDEVICEOBJECT_H
#include "base.h"

class InstanceObject;

class PhysicalDeviceObject
{
public:
    explicit PhysicalDeviceObject(std::shared_ptr<InstanceObject>& instance);
    ~PhysicalDeviceObject();
    /// 不可复制
    PhysicalDeviceObject(const PhysicalDeviceObject&) = delete;
    /// 不可移动
    PhysicalDeviceObject(PhysicalDeviceObject&&) = delete;
    /// 不可赋值
    PhysicalDeviceObject& operator=(const PhysicalDeviceObject&) = delete;
    PhysicalDeviceObject& operator=(PhysicalDeviceObject&&) = delete;

    inline VkPhysicalDevice handle() const { return m_handler; }
    inline std::shared_ptr<InstanceObject>& instance() { return m_instance; }
    inline VkPhysicalDeviceFeatures& features() { return m_features; }
    inline VkPhysicalDeviceMemoryProperties& memoryProperties() { return m_memoryProperties; }

    uint32_t queueIndex(VkQueueFlagBits queueFlags);

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
#endif //CROSS_PLATFORM_DEMO_PHYSICALDEVICEOBJECT_H
