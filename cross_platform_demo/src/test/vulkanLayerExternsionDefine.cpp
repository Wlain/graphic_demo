//
// Created by william on 2021/6/19.
//

#include "vulkanLayerExternsionDefine.h"
VulkanLayerAndExtension::VulkanLayerAndExtension() = default;

VulkanLayerAndExtension::~VulkanLayerAndExtension() = default;

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
    // 存储实例层的数目
    uint32_t instanceLayerCount;
    // 通过向量组来存储层的属性
    std::vector<VkLayerProperties> layerProperties;
    // 检查Vulkan API的执行结果
    VkResult result;
    // 查询所有的层
    do
    {
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        if (result)
        {
            return result;
        }
        if (instanceLayerCount == 0)
            return VK_INCOMPLETE; // 获取失败
        layerProperties.resize(instanceLayerCount);
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
    } while (result == VK_INCOMPLETE);
    // 查询每个层的所有扩展信息，并进行保存
    std::cout << "\nInstanced Layers\n"
              << std::endl;
    std::cout << "==================" << std::endl;
    for (auto globalLayerProp : layerProperties)
    {
        // 显示层的名字和描述信息
        std::cout << "\n"
                  << globalLayerProp.description << "\n\t|\n\t|---[Layer Name]-->" << globalLayerProp.layerName << "\n";
        LayerProperties layerProps;
        layerProps.properties = globalLayerProp;
        // 根据属性信息，获取就要实例的扩展
        result = getExtensionProperties(layerProps);
        if (result)
        {
            continue;
        }
        m_layerPropertyList.push_back(layerProps);
        // 显示每个实例层的扩展名称
        for (auto j : layerProps.extensions)
        {
            std::cout << "\t\t|\n\t\t---[Layer Extension]---> " << j.extensionName << "\n";
        }
    }
    return result;
}

VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu)
{
    // 保存每一层的扩展总数
    uint32_t extensionCount;
    VkResult result;
    // 层的名称
    char* layerName = layerProps.properties.layerName;
    do
    {
        // 获取当前层中的扩展总数
        result = gpu ? vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, nullptr) : vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);
        if (result || extensionCount == 0)
        {
            continue;
        }
        layerProps.extensions.resize(extensionCount);
        // 获取所有的扩展属性
        result = gpu ? vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data()) :
                       vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
    } while (result == VK_INCOMPLETE);
    return result;
}

VkResult VulkanLayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu)
{
    return VK_ERROR_INITIALIZATION_FAILED;
}
