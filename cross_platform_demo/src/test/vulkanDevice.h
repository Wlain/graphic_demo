//
// Created by william on 2021/6/20.
//

#ifndef CROSS_PLATFORM_DEMO_VULKANDEVICE_H
#define CROSS_PLATFORM_DEMO_VULKANDEVICE_H
#include "base.h"
#include "vulkanLayerExternsionDefine.h"

class VulkanDevice
{
public:
    explicit VulkanDevice(VkPhysicalDevice* gpu);
    ~VulkanDevice();

public:
    // 这个类对外公开的主要函数
    VkResult createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions);
    void destroyDevice();
    // Get the avaialbe queues exposed by the physical devices
    void getPhysicalDeviceQueuesAndProperties();
    // Query physical device to retrive queue properties
    uint32_t getGraphicsQueueHandle();

public:
    // 设备相关的成员变量
    VkDevice m_device;                                   // 逻辑设备
    VkPhysicalDevice* m_gpu;                             // 物理设备
    VkPhysicalDeviceMemoryProperties m_memoryProperties; // 物理设备属性

    // 队列相关的属性
    // vulkan队列对象
    VkQueue m_queue;
    // 保存物理设备暴露的所有队列族
    std::vector<VkQueueFamilyProperties> m_queueFamilyProps;

    // 保存图形队列的索引号
    uint32_t m_graphicsQueueFamilyIndex;
    // 设备中的队列族的数量
    uint32_t m_queueFamilyCount;
    // 设备相关的扩展
    VulkanLayerAndExtension m_layerAndExtension;
};

#endif //CROSS_PLATFORM_DEMO_VULKANDEVICE_H
