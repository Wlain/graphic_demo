//
// Created by william on 2021/6/20.
//

#include "physicalDeviceObject.h"

#include "instanceObject.h"

PhysicalDeviceObject::PhysicalDeviceObject(std::shared_ptr<InstanceObject>& instance) :
    m_instance(instance)
{
    uint32_t physicalDeviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance->handler(), &physicalDeviceCount, VK_NULL_HANDLE));
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance->handler(), &physicalDeviceCount, devices.data()));
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

PhysicalDeviceObject::~PhysicalDeviceObject() = default;



uint32_t PhysicalDeviceObject::queueIndex(VkQueueFlagBits queueFlags)
{
    //	1. Get the number of Queues supported by the Physical device
    //	2. Get the properties each Queue type or Queue Family
    //			There could be 4 Queue type or Queue families supported by physical device -
    //			Graphics Queue	- VK_QUEUE_GRAPHICS_BIT
    //			Compute Queue	- VK_QUEUE_COMPUTE_BIT
    //			DMA				- VK_QUEUE_TRANSFER_BIT
    //			Sparse memory	- VK_QUEUE_SPARSE_BINDING_BIT
    //	3. Get the index ID for the required Queue family, this ID will act like a handle index to queue.
    bool found = false;
    // 1. Iterate number of Queues supported by the Physical device
    for (unsigned int i = 0; i < m_queueFamilyProperties.size(); i++){
        // 2. Get the Graphics Queue type
        //		There could be 4 Queue type or Queue families supported by physical device -
        //		Graphics Queue		- VK_QUEUE_GRAPHICS_BIT
        //		Compute Queue		- VK_QUEUE_COMPUTE_BIT
        //		DMA/Transfer Queue	- VK_QUEUE_TRANSFER_BIT
        //		Sparse memory		- VK_QUEUE_SPARSE_BINDING_BIT
        if (m_queueFamilyProperties[i].queueFlags & queueFlags){
            // 3. Get the handle/index ID of graphics queue family.
            found				= true;
            return i;
        }
    }
    // Assert if there is no queue found.
    RAS_ASSERT(found);
    return 0;
}