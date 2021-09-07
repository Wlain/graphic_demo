//
// Created by william on 2021/9/7.
//

#include "device.h"
Device::Device(VkPhysicalDevice* gpu) :
    m_gpu(gpu)
{
}

Device::~Device() = default;

VkResult Device::createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
    m_layerExtension.m_requestedLayerNames = layers;
    m_layerExtension.m_requestedExtensionNames = extensions;
    VkResult result;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.queueFamilyIndex = m_graphicsQueueHandle;
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = VK_NULL_HANDLE;
    queueInfo.queueCount = 1;
    float devicePriorities = 1.0f;
    queueInfo.pQueuePriorities = &devicePriorities;

    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = VK_NULL_HANDLE;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    /// deviceInfo.enabledLayerCount, deviceInfo.ppEnabledLayerNames 这两个东西在VK2.0已经被忽略了
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;
    deviceInfo.enabledExtensionCount = (uint32_t)extensions.size();
    deviceInfo.ppEnabledExtensionNames = !extensions.empty() ? extensions.data() : nullptr;
    deviceInfo.pEnabledFeatures = nullptr;
    result = vkCreateDevice(*m_gpu, &deviceInfo, nullptr, &m_device);
    return result;
}

void Device::destroyDevice()
{
    vkDestroyDevice(m_device, nullptr);
}

/// 查询queueFamilyProps
void Device::getPhysicalDeviceQueuesAndProperties()
{
    vkGetPhysicalDeviceQueueFamilyProperties(*m_gpu, &m_queueFamilyCount, nullptr);
    m_queueFamilyProps.resize(m_queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(*m_gpu, &m_queueFamilyCount, m_queueFamilyProps.data());
}

uint32_t Device::getGraphicsQueueHandle()
{
    bool found = false;
    for (unsigned int i = 0; i < m_queueFamilyCount; i++)
    {
        if (m_queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            found = true;
            m_queueFamilyCount = i;
            break;
        }
    }
    // Assert if there is no queue found.
    assert(found);
    return 0;
}
