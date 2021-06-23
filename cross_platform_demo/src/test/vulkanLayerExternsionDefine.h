//
// Created by william on 2021/6/19.
//

#ifndef CROSS_PLATFORM_DEMO_VULKANLAYEREXTERNSIONDEFINE_H
#define CROSS_PLATFORM_DEMO_VULKANLAYEREXTERNSIONDEFINE_H
#include "base.h"

struct LayerProperties
{
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

class VulkanLayerAndExtension
{
public:
    VulkanLayerAndExtension();
    ~VulkanLayerAndExtension();
    /// 层和对应的扩展列表
    VkResult getInstanceLayerProperties(); // 全局实例
    /// 全局扩展
    VkResult getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu = nullptr);
    /// 基于设备的扩展
    VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);

public:
    std::vector<LayerProperties> m_layerPropertyList;
    // List of layer names requested by the application.
    std::vector<const char*> m_appRequestedLayerNames;
    // List of extension names requested by the application.
    std::vector<const char*> m_appRequestedExtensionNames;
};

#endif //CROSS_PLATFORM_DEMO_VULKANLAYEREXTERNSIONDEFINE_H
