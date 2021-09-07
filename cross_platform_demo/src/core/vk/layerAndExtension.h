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

    /// debugger function
    bool isLayersSupported(std::vector<const char*>& layerNames);
    void destroyDebugUtilsMessengerEXT();
    VkResult createDebugUtilsMessengerEXT();
    VKAPI_ATTR VkBool32 VKAPI_CALL debugFunction(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
                                                                    uint64_t srcObject, size_t location, int32_t msgCode,
                                                                    const char *layerPrefix, const char *msg, void *userData);

public:
    std::vector<const char*> m_requestedLayerNames;
    std::vector<const char*> m_requestedExtensionNames;
    std::vector<LayerProperties> m_layerPropertyList;
private:
    /// debug调试信息
    VkDebugUtilsMessengerEXT m_debugUtilsMessenger = VK_NULL_HANDLE;
    /// debug回调report
    VkDebugReportCallbackEXT m_debugReportCallback = VK_NULL_HANDLE;

    PFN_vkCreateDebugReportCallbackEXT m_dbgCreateDebugReportCallback = VK_NULL_HANDLE;
    PFN_vkDestroyDebugReportCallbackEXT m_dbgDestroyDebugReportCallback = VK_NULL_HANDLE;
};

#endif //CROSS_PLATFORM_DEMO_LAYERANDEXTENSION_H
