//
// Created by ytech on 2021/8/11.
//

#include "device.h"

#include "instance.h"
#include "physicalDevice.h"

Device::Device(PhysicalDevice* gpu, VkSurfaceKHR surface, const char* extensions):
    m_gpu(gpu),
    m_resourceCache(*this)
{
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = VK_NULL_HANDLE;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = VK_NULL_HANDLE;
    m_queueIndex = gpu->queueIndex(VK_QUEUE_GRAPHICS_BIT);
    queueInfo.queueFamilyIndex = m_queueIndex;
    queueInfo.queueCount = 1;
    float devicePriorities = 1.0f;
    queueInfo.pQueuePriorities = &devicePriorities;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    VkPhysicalDeviceFeatures features = {};
    vkGetPhysicalDeviceFeatures(gpu->handle(), &features);
    deviceInfo.pEnabledFeatures = &features;
    // 获取设备层扩展
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(gpu->handle(), NULL, &extensionCount, NULL);
    std::vector<const char*> extNames;
    std::vector<VkExtensionProperties> extProps(extensionCount);
    vkEnumerateDeviceExtensionProperties(gpu->handle(), NULL, &extensionCount, extProps.data());
    for (uint32_t i = 0; i < extensionCount; i++)
    {
        extNames.push_back(extProps[i].extensionName);
    }
    deviceInfo.enabledExtensionCount = extNames.size();
    deviceInfo.ppEnabledExtensionNames = extNames.data();
    /// deviceInfo.enabledLayerCount, deviceInfo.ppEnabledLayerNames 这两个东西在VK2.0已经被忽略了
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;
    VK_CHECK(vkCreateDevice(gpu->handle(), &deviceInfo, gpu->instance().allocator(), &m_handle));
}

Device::~Device()
{
    m_resourceCache.clear();
    if (m_handle != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_handle, nullptr);
    }
}
