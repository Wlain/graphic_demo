//
// Created by william on 2021/6/20.
//

#ifndef CROSS_PLATFORM_DEMO_VULKANAPPLICATIONS_H
#define CROSS_PLATFORM_DEMO_VULKANAPPLICATIONS_H
#include "core/vk/instanceObject.h"
#include "vulkanLayerExternsionDefine.h"

class VulkanApplications
{
public:
    VulkanApplications();

public:
    ~VulkanApplications();

public:
    VkResult createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName);
    void initialize();   // Initialize and allocate resources
    void prepare();      // Prepare resource
    void update();       // Update data
    bool render();       // Render primitives

private:
    InstanceObject m_instanceObj;
};

#endif //CROSS_PLATFORM_DEMO_VULKANAPPLICATIONS_H
