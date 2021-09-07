//
// Created by william on 2021/9/6.
//

#include "layerAndExtension.h"

#include "vulkanApplication.h"
LayerAndExtension::LayerAndExtension() = default;

LayerAndExtension::~LayerAndExtension() = default;

VkResult LayerAndExtension::getInstanceLayerProperties()
{
    uint32_t instanceLayerCount{};
    std::vector<VkLayerProperties> layerProperties;
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

/// 此函数在实例中检索扩展及其属性和设备级别。传递有效的物理设备
/// 指向检索设备级扩展的指针，否则使用 NULL 来检索特定于实例级别的扩展。
VkResult LayerAndExtension::getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu)
{
    uint32_t extensionCount{};
    VkResult result;
    char* layerName = layerProps.properties.layerName;
    do
    {
        if (gpu)
        {
            result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, nullptr);
        }
        else
        {
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);
        }
        if (result || extensionCount == 0)
        {
            continue;
        }
        layerProps.extensions.resize(extensionCount);
        if (gpu)
        {
            result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
        }
        else
        {
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
        }
    } while (result == VK_INCOMPLETE);
    return result;
}

/// 获取设备的扩展
VkResult LayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu)
{
    VkResult result;
    // 对每一个层查询所有扩展信息，并进行保存
    std::cout << "\nDevice extensions\n"
              << std::endl;
    std::cout << "==================" << std::endl;
    VulkanApplication* appObj = VulkanApplication::getInstance();
    // 取地址，避免拷贝
    std::vector<LayerProperties>* instanceLayerProp = &appObj->getInstance()->m_instanceObj.m_layerExtension.m_layerPropertyList;
    for (auto globalLayerProp : *instanceLayerProp)
    {
        LayerProperties layerProps;
        layerProps.properties = globalLayerProp.properties;
        if (result == getExtensionProperties(layerProps, gpu))
        {
            continue;
        }
        std::cout << "\n"
                  << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
        m_layerPropertyList.push_back(layerProps);
        if (!layerProps.extensions.empty())
        {
            for (auto e : layerProps.extensions)
                std::cout << "\t\t|\n\t\t|---[Device Extesion]--> " << e.extensionName << "\n";
        }
        else
        {
            std::cout << "\t\t|\n\t\t|---[Device Extesion]--> No extension found \n";
        }
    }
    return result;
}
