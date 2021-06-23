//
// Created by william on 2021/6/20.
//

#include "vulkanApplications.h"

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;


VulkanApplications::VulkanApplications() = default;

VulkanApplications::~VulkanApplications() = default;

VkResult VulkanApplications::createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName)
{
    m_instanceObj.createInstance(layers, extensions, applicationName);
    return VK_SUCCESS;
}

void VulkanApplications::initialize()
{
    const char* titles = "Hello World!!!";
//    createVulkanInstance(layerNames, instanceExtensionNames, titles);
}

void VulkanApplications::prepare()
{
}
void VulkanApplications::update()
{
}
bool VulkanApplications::render()
{
    return false;
}