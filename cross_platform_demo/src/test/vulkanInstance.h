//
// Created by william on 2021/6/19.
//

#ifndef CROSS_PLATFORM_DEMO_VULKANINSTANCE_H
#define CROSS_PLATFORM_DEMO_VULKANINSTANCE_H
#include "base.h"
#include "vulkanLayerExternsionDefine.h"

class VulkanInstance

{
public:
    VulkanInstance();
    ~VulkanInstance();
    // 用来创建和删除Vulkan实例的函数
    VkResult createInstance(std::vector<const char*>& layers, std::vector<const char*>& extensionNames, const char* appName);
    // 销毁Vulkan实例
    void destroyInstance() const;

public:
    // vulkan实例对象的变量
    VkInstance m_instance{};
    // Vulkan实例相关的层和扩展
    VulkanLayerAndExtension m_layerAndExtension{};
};

#endif //CROSS_PLATFORM_DEMO_VULKANINSTANCE_H
