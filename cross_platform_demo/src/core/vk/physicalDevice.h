//
// Created by william on 2021/6/20.
//

#ifndef CROSS_PLATFORM_DEMO_PHYSICALDEVICE2_H
#define CROSS_PLATFORM_DEMO_PHYSICALDEVICE2_H
#include "commonDefine_vk.h"

class Instance;

class PhysicalDevice
{
public:

    PhysicalDevice(Instance& instance, VkPhysicalDevice& physicalDevice);
    explicit PhysicalDevice(Instance& instance);
    ~PhysicalDevice();
    /// 不可复制
    PhysicalDevice(const PhysicalDevice&) = delete;
    /// 不可移动
    PhysicalDevice(PhysicalDevice&&) = delete;
    /// 不可赋值
    PhysicalDevice& operator=(const PhysicalDevice&) = delete;
    PhysicalDevice& operator=(PhysicalDevice&&) = delete;
    inline operator VkPhysicalDevice() { return m_handle; }
    inline operator const VkPhysicalDevice() const { return m_handle; }
    inline VkPhysicalDevice handle() { return m_handle; }
    inline const VkPhysicalDevice handle() const { return m_handle; }
    inline Instance& instance() { return m_instance; }
    inline VkPhysicalDeviceFeatures& mutableFeatures() { return m_features; }
    inline const VkPhysicalDeviceFeatures& features() const { return m_features; }
    inline VkPhysicalDeviceMemoryProperties& memoryProperties() { return m_memoryProperties; }
    inline const VkPhysicalDeviceProperties properties() const { return m_properties; }
    inline const std::vector<VkQueueFamilyProperties>& queueFamilyProperties() const { return m_queueFamilyProperties; }
    VkBool32 isPresentSupported(VkSurfaceKHR surface, uint32_t queueFamilyIndex) const;

    uint32_t queueIndex(VkQueueFlagBits queueFlags);
    uint32_t queueFamilyPerformanceQueryPasses(const VkQueryPoolPerformanceCreateInfoKHR* perfQueryCreateInfo) const;
    /**
     * 列举一个物理设备的队列系列上可用的性能查询计数器
     * @param queueFamilyIndex 想得到的物理设备的队列簇的索引
     * @param count 指向与可用或查询的计数器数量有关的整数的指针
     * @param counters 是NULL或者一个指向VkPerformanceCounterKHR结构数组的指针
     * @param descriptions 是NULL或者一个指向VkPerformanceCounterDescriptionKHR结构数组的指针
     */
    void enumerateQueueFamilyPerformanceQueryCounters(uint32_t queueFamilyIndex, uint32_t* count, VkPerformanceCounterKHR* counters, VkPerformanceCounterDescriptionKHR* descriptions) const;
    void* extensionFeatureChain() const;

public:
    /// 实例对象句柄
    Instance& m_instance;
    /// vulkan 物理设备句柄
    VkPhysicalDevice m_handle = VK_NULL_HANDLE;
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
    void* m_lastRequestedExtensionFeature = VK_NULL_HANDLE;
};
#endif //CROSS_PLATFORM_DEMO_PHYSICALDEVICE2_H
