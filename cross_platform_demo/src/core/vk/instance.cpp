//
// Created by william on 2021/6/19.
//

#include "instance.h"

#include "logger.h"

namespace graphicEngine::vk
{
bool validateLayers(const std::vector<const char*>& required, const std::vector<VkLayerProperties>& available)
{
    for (auto& layer : required)
    {
        bool found = std::any_of(available.begin(), available.end(), [&](VkLayerProperties properties) {
            return std::strcmp(properties.layerName, layer) == 0;
        });
        if (!found)
        {
            RAS_INFO("Validation Layer {} not found", layer);
            return false;
        }
    }
    return true;
}

std::vector<const char*> getOptimalValidationLayers(const std::vector<VkLayerProperties>& supportedInstanceLayers)
{
    std::vector<std::vector<const char*>> validationLayerPriorityList = {
        // The preferred validation layer is "VK_LAYER_KHRONOS_validation"
        { "VK_LAYER_KHRONOS_validation" },
        // Otherwise we fallback to using the LunarG meta layer
        { "VK_LAYER_LUNARG_standard_validation" },

        // Otherwise we attempt to enable the individual layers that compose the LunarG meta layer since it doesn't exist
        {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_GOOGLE_unique_objects",
        },

        // Otherwise as a last resort we fallback to attempting to enable the LunarG core layer
        { "VK_LAYER_LUNARG_core_validation" }
    };

    for (auto& validationLayers : validationLayerPriorityList)
    {
        if (validateLayers(validationLayers, supportedInstanceLayers))
        {
            return validationLayers;
        }
        RAS_WARN("Couldn't enable validation layers (see log for error) - falling back");
    }
    return {};
}

Instance::~Instance()
{
//    if (m_debugUtilsMessenger != VK_NULL_HANDLE)
//    {
//        vkDestroyDebugUtilsMessengerEXT(m_handle, m_debugUtilsMessenger, nullptr);
//    }
//    if (m_debugReportCallback != VK_NULL_HANDLE)
//    {
//        vkDestroyDebugReportCallbackEXT(m_handle, m_debugReportCallback, nullptr);
//    }

    if(m_handle != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_handle, nullptr);
    }
}

Instance::Instance(const std::string& applicationName, const std::unordered_map<const char*, bool>& requiredExtensions, const std::vector<const char*>& requiredValidationLayers, bool headless, uint32_t apiVersion)
{
//    VkResult result =
}

Instance::Instance(VkInstance instance)
{
}

void Instance::queryGPUs()
{
}

//PhysicalDevice& Instance::getSuitableGPU(VkSurfaceKHR surface)
//{
//    return m_handle;
//}

//PhysicalDevice& Instance::getFirstGPU()
//{
//    return {};
//}

bool Instance::isEnabled(const char* extension) const
{
    return false;
}

VkInstance Instance::getHandle()
{
    return nullptr;
}

const std::vector<const char*>& Instance::getExtensions()
{
    return {};
}

} // namespace graphicEngine::vk
