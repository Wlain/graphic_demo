//
// Created by william on 2021/9/7.
//

#ifndef CROSS_PLATFORM_DEMO_DEVICE_H
#define CROSS_PLATFORM_DEMO_DEVICE_H
#include "commonDefine_vk.h"
#include "layerAndExtension.h"

class Device
{
public:
    Device(VkPhysicalDevice* gpu);
    ~Device();

    inline operator VkDevice() { return m_device; }
    inline operator const VkDevice() const { return m_device; }

    VkResult createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions);
    void destroyDevice();
    /// 获取物理设备所暴露的可用队列
    void getPhysicalDeviceQueuesAndProperties();

    /// 查询队列句柄
    uint32_t getGraphicsQueueHandle();

public:
    VkDevice m_device{};                     // 逻辑设备
    VkPhysicalDevice* m_gpu;                 // 物理设备
    VkPhysicalDeviceProperties m_gpuProps{}; // 物理设备属性
    VkPhysicalDeviceMemoryProperties m_memoryProperties{};

    VkQueue m_queue{};
    std::vector<VkQueueFamilyProperties> m_queueFamilyProps;
    uint32_t m_graphicsQueueHandle{};
    uint32_t m_graphicsQueueWithPresentHandle{};
    uint32_t m_queueFamilyCount{};

    // Layer and extensions
    LayerAndExtension m_layerExtension;
};

#endif //CROSS_PLATFORM_DEMO_DEVICE_H
