//
// Created by william on 2021/9/6.
//

#ifndef CROSS_PLATFORM_DEMO_LAYERANDEXTENSION_H
#define CROSS_PLATFORM_DEMO_LAYERANDEXTENSION_H
#include "commonDefine_vk.h"

struct LayerProperties
{
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

class LayerAndExtension
{
public:
    LayerAndExtension();
    ~LayerAndExtension();
    VkResult getInstanceLayerProperties();
    VkResult getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu = VK_NULL_HANDLE);
    VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);

public:
    std::vector<const char*> m_requestedLayerNames;
    std::vector<const char*> m_requestedExtensionNames;
    std::vector<LayerProperties> m_layerPropertyList;
};

#endif //CROSS_PLATFORM_DEMO_LAYERANDEXTENSION_H
