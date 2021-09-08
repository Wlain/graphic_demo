//
// Created by william on 2021/6/20.
//

#include "vulkanApplication.h"

#include "device.h"

std::unique_ptr<VulkanApplication> VulkanApplication::s_instance;
std::once_flag VulkanApplication::s_onlyOnce;

extern std::vector<const char*> s_instanceExtensionNames;
extern std::vector<const char*> s_layerNames;
extern std::vector<const char*> s_deviceExtensionNames;

VulkanApplication::VulkanApplication()
{
    m_instanceObj.m_layerExtension.getInstanceLayerProperties();
    m_isDebug = true;
}

VulkanApplication::~VulkanApplication() = default;

VkResult VulkanApplication::createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName)
{
    return m_instanceObj.createInstance(layers, extensions, applicationName);
}

void VulkanApplication::initialize()
{
    const char* titles = "Hello World!!!";
    if(m_isDebug)
    {
        m_instanceObj.m_layerExtension.isLayersSupported(s_layerNames);
    }
    createVulkanInstance(s_layerNames, s_instanceExtensionNames, titles);
    if(m_isDebug)
    {
        m_instanceObj.m_layerExtension.createDebugReportCallback();
    }
    std::vector<VkPhysicalDevice> gpuList;
    enumeratePhysicalDevices(gpuList);
    if (!gpuList.empty())
    {
        handShakeWithDevice(&gpuList[0], s_layerNames, s_deviceExtensionNames);
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
    return true;
}

VulkanApplication* VulkanApplication::getInstance()
{
    std::call_once(s_onlyOnce, []() { s_instance.reset(new VulkanApplication()); });
    return s_instance.get();
}

void VulkanApplication::destroy()
{
    m_deviceObj->destroyDevice();
    if(m_isDebug)
    {
        m_instanceObj.m_layerExtension.destroyDebugReportCallback();
    }
    m_instanceObj.destroyInstance();
}

/// 创建逻辑设备需要以下步骤：
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
    m_deviceObj = new Device(gpu);
    if (!m_deviceObj)
    {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }
    // 1.获取物理设备的特定层和相应的扩展[可选]。
    m_deviceObj->m_layerExtension.getDeviceExtensionProperties(gpu);
    // 4.获取物理设备或GPU属性
    vkGetPhysicalDeviceProperties(*gpu, &m_deviceObj->m_gpuProps);
    vkGetPhysicalDeviceMemoryProperties(*gpu, &m_deviceObj->m_memoryProperties);
    m_deviceObj->getPhysicalDeviceQueuesAndProperties();
    m_deviceObj->getGraphicsQueueHandle();
    return m_deviceObj->createDevice(layers, extensions);
}

void VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuLists)
{
    uint32_t gpuDeviceCount;
    VK_CHECK(vkEnumeratePhysicalDevices(m_instanceObj, &gpuDeviceCount, nullptr));
    ASSERT(gpuDeviceCount);
    gpuLists.resize(gpuDeviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(m_instanceObj, &gpuDeviceCount, gpuLists.data()));
}
