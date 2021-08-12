//
// Created by ytech on 2021/8/11.
//

#include "deviceObject.h"

#include "instanceObject.h"

DeviceObject::DeviceObject(std::shared_ptr<PhysicalDeviceObject> physicalDeviceObject) :
    m_physicalDevice(std::move(physicalDeviceObject))
{
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = VK_NULL_HANDLE;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = VK_NULL_HANDLE;
    m_queueIndex = m_physicalDevice->queueIndex(VK_QUEUE_GRAPHICS_BIT);
    queueInfo.queueFamilyIndex = m_queueIndex;
    queueInfo.queueCount = 1;
    float devicePriorities = 1.0f;
    queueInfo.pQueuePriorities = &devicePriorities;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    VkPhysicalDeviceFeatures features = {};
    vkGetPhysicalDeviceFeatures(m_physicalDevice->handle(), &features);
    /// 需要手动开启
    features.samplerAnisotropy = VK_TRUE;
//    features.sampleRateShading = VK_TRUE;
//    /// 开启几何着色器
//    features.geometryShader = VK_TRUE;
    deviceInfo.pEnabledFeatures = &features;
    // get extension names
    uint32_t _extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(m_physicalDevice->handle(), NULL, &_extensionCount, NULL);
    std::vector<const char *> extNames;
    std::vector<VkExtensionProperties> extProps(_extensionCount);
    vkEnumerateDeviceExtensionProperties(m_physicalDevice->handle(), NULL, &_extensionCount, extProps.data());
    for (uint32_t i = 0; i < _extensionCount; i++) {
        extNames.push_back(extProps[i].extensionName);
    }
    deviceInfo.enabledExtensionCount = extNames.size();
    deviceInfo.ppEnabledExtensionNames = extNames.data();
//    deviceInfo.enabledExtensionCount = m_physicalDevice->instance()->enabledExtensions().size();
//    deviceInfo.ppEnabledExtensionNames = m_physicalDevice->instance()->enabledExtensions().data();
    ///     deviceInfo.enabledLayerCount = 0;
    ///    deviceInfo.ppEnabledLayerNames = VK_NULL_HANDLE;这两个东西在VK2.0已经被忽略了
    deviceInfo.enabledLayerCount = m_physicalDevice->instance()->validationLayers().size();
    deviceInfo.ppEnabledLayerNames = m_physicalDevice->instance()->validationLayers().data();
    VK_CHECK(vkCreateDevice(m_physicalDevice->handle(), &deviceInfo, m_physicalDevice->instance()->allocator(), &m_handle));
}

DeviceObject::~DeviceObject() = default;
