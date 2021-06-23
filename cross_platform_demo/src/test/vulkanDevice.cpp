//
// Created by william on 2021/6/20.
//

#include "vulkanDevice.h"
VulkanDevice::VulkanDevice(VkPhysicalDevice* gpu)
{
}

VulkanDevice::~VulkanDevice()
{
}

VkResult VulkanDevice::createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
    return VK_ERROR_INITIALIZATION_FAILED;
}

void VulkanDevice::destroyDevice()
{
}

void VulkanDevice::getPhysicalDeviceQueuesAndProperties()
{
}

uint32_t VulkanDevice::getGraphicsQueueHandle()
{
    return 0;
}
