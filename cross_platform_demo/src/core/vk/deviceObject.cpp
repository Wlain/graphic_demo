//
// Created by william on 2021/6/20.
//

#include "deviceObject.h"

#include "instanceObject.h"

DeviceObject::DeviceObject(std::shared_ptr<InstanceObject>& instance) :
    m_instance(instance)
{
    uint32_t physicalDeviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance->handle(), &physicalDeviceCount, VK_NULL_HANDLE));
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance->handle(), &physicalDeviceCount, devices.data()));
    RAS_INFO("numbers of physical devices is %d", devices.size());
    for (auto device : devices)
    {
        vkGetPhysicalDeviceProperties(device, &m_properties);
        vkGetPhysicalDeviceMemoryProperties(device, &m_memoryProperties);
        vkGetPhysicalDeviceFeatures(device, &m_features);
        RAS_INFO("Found GPU %s", m_properties.deviceName);
        m_allowedMemoryTypes = std::vector<VkMemoryType>(m_memoryProperties.memoryTypes,  m_memoryProperties.memoryTypes + m_memoryProperties.memoryTypeCount);
        RAS_INFO("numbers of m_allowedMemoryTypes is %d", m_allowedMemoryTypes.size());
        m_allowedHeapTypes = std::vector<VkMemoryHeap>(m_memoryProperties.memoryHeaps,  m_memoryProperties.memoryHeaps + m_memoryProperties.memoryHeapCount);
        RAS_INFO("numbers of m_allowedHeapTypes is %d", m_allowedHeapTypes.size());
        uint32_t queueFamilyPropertyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertyCount, VK_NULL_HANDLE);
        m_queueFamilyProperties.resize(queueFamilyPropertyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertyCount, m_queueFamilyProperties.data());
        m_handler = device;
        break;
    }
}

DeviceObject::~DeviceObject() = default;

VkResult DeviceObject::createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
    return VK_ERROR_INITIALIZATION_FAILED;
}

void DeviceObject::destroyDevice()
{
}

void DeviceObject::getPhysicalDeviceQueuesAndProperties()
{
}

uint32_t DeviceObject::getGraphicsQueueHandle()
{
    return 0;
}
