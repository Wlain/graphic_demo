//
// Created by william on 2021/6/20.
//

#include "vulkanApplication.h"
#include "physicalDevice.h"

std::unique_ptr<VulkanApplication> VulkanApplication::s_instance;
std::once_flag VulkanApplication::s_onlyOnce;

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;

VulkanApplication::VulkanApplication()
{
    m_instanceObj.m_layerExtension.getInstanceLayerProperties();
    m_deviceObj = nullptr;
}

VulkanApplication::~VulkanApplication() = default;

VkResult VulkanApplication::createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName)
{
    return m_instanceObj.createInstance(layers, extensions, applicationName);
}

void VulkanApplication::initialize()
{
    const char* titles = "Hello World!!!";
    createVulkanInstance(layerNames, instanceExtensionNames, titles);
    std::vector<VkPhysicalDevice> gpuList;
    enumeratePhysicalDevices(gpuList);
    if (!gpuList.empty())
    {
        handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
    }
}

void VulkanApplication::prepare()
{
}
void VulkanApplication::update()
{
}
bool VulkanApplication::render()
{
    return false;
}

VulkanApplication* VulkanApplication::getInstance()
{
    return nullptr;
}
void VulkanApplication::destroy()
{
}

/// 创建逻辑设备。
/// 逻辑设备的创建过程需要以下步骤：
/// 1.获取物理设备的特定层和相应的扩展[可选]。
/// 2.创建用户定义的VulkanDevice对象
/// 3.提供需要在该物理设备中启用的层和扩展列表。
/// 4.获取物理设备或GPU属性
/// 5.从物理设备或GPU获取内存属性
/// 6. 查询物理设备暴露的队列和相关属性
/// 7. 获取图形队列的句柄
/// 8. 创建逻辑设备，将其连接到图形队列上。

VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
    m_deviceObj = Device(gpu);

    return VK_ERROR_INITIALIZATION_FAILED;
}
VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpus)
{
    return VK_ERROR_INITIALIZATION_FAILED;
}
