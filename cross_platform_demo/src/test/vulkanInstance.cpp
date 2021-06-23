//
// Created by william on 2021/6/19.
//

#include "vulkanInstance.h"
VulkanInstance::VulkanInstance() = default;

VulkanInstance::~VulkanInstance() = default;

VkResult VulkanInstance::createInstance(std::vector<const char*>& layers, std::vector<const char*>& extensionNames, const char* appName)
{
    // 设置实例相关的层和扩展的信息
    m_layerAndExtension.m_appRequestedExtensionNames = extensionNames;
    m_layerAndExtension.m_appRequestedLayerNames = layers;
    // 定义vulkan应用程序的结构体
    VkApplicationInfo  appInfo{};
    appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext				= nullptr;
    appInfo.pApplicationName	= appName;
    appInfo.applicationVersion	= 1;
    appInfo.pEngineName			= appName;
    appInfo.engineVersion		= 1;
    appInfo.apiVersion			= VK_MAKE_VERSION(1, 0, 0);

    // 定义vulkan实例创建的参数结构体
    VkInstanceCreateInfo instInfo{};
    instInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext					= VK_NULL_HANDLE;
    instInfo.flags					= 0;
    instInfo.pApplicationInfo		= &appInfo;
    VkResult result = vkCreateInstance(&instInfo, nullptr, &m_instance);
    // 需要设置启用层的名称的列表
    instInfo.enabledLayerCount		= (uint32_t)layers.size();
    instInfo.ppEnabledLayerNames	= layers.empty() ? layers.data() : nullptr;

    assert(result == VK_SUCCESS);
    return result;
}

void VulkanInstance::destroyInstance() const
{
    vkDestroyInstance(m_instance, nullptr);
}

